import numpy as np
import matplotlib.pyplot as plt
import math


def adl(X, y):
    indicesRouge = X[y != 1, :]
    indicesBleu = X[y == 1, :]
    moyenneBleue = np.mean(indicesBleu, axis=0)
    moyenneRouge = np.mean(indicesRouge, axis=0)
    CBleu = np.cov(indicesBleu.T)
    CRouge = np.cov(indicesRouge.T)
    r = len(indicesRouge) - 1
    b = len(indicesBleu) - 1
    C = (r * CRouge + b * CBleu) / (r + b)
    w = np.linalg.solve(C, moyenneBleue - moyenneRouge)
    b = np.log(b / r) - 0.5 * (moyenneRouge + moyenneBleue) @ w

    return w, b


def adlpredict(X, w, b):
    return np.sign(X @ w + b)


#### programme principal à implémenter dans cette fonction ####
def monprogramme(Xapp, Yapp):
    """
        Programme pour l'ADL (lancé avec ESPACE)

        Xapp, Yapp : base d'apprentissage générée avec la souris
    """
    print("Apprentissage lancé avec " + str(len(Xapp)) + " points ")

    # apprendre les paramètres w et b du classifieur...

    (w, b) = adl(Xapp, Yapp)
    print("w = ", w, "; b = ", b)

    # création d'une grille de points de test
    r1 = np.arange(-5, 5, 0.2)
    Xtest = np.zeros((len(r1) * len(r1), 2))
    i = 0
    for x1 in r1:
        for x2 in r1:
            Xtest[i, :] = [x1, x2]
            i += 1

    # Prédire la catégorie pour tous les points de test...
    Ypred = adlpredict(Xtest, w, b)

    # ... et tracer le résultat pour visualiser la frontière
    plt.plot(Xtest[Ypred == 1, 0], Xtest[Ypred == 1, 1], '.b', alpha=0.3)
    plt.plot(Xtest[Ypred == -1, 0], Xtest[Ypred == -1, 1], '.r', alpha=0.3)

    # pour réellement mettre à jour le graphique: (à garder en fin de fonction)
    fig.canvas.draw()


##### Gestion de l'interface graphique ########


Xplot = np.zeros((0, 2))
Yplot = np.zeros(0)
plotvariance = 0

C = 1
sigma = 1


def onclick(event):
    global Xplot, newY
    global Yplot

    if plotvariance == 0:
        newX = np.array([[event.xdata, event.ydata]])
    else:
        newX = math.sqrt(plotvariance) * np.random.randn(10, 2) + np.ones((10, 1)).dot(
            np.array([[event.xdata, event.ydata]]))

    print("Ajout de " + str(len(newX)) + " points en (" + str(event.xdata) + ", " + str(event.ydata) + ")")

    Xplot = np.concatenate((Xplot, newX))
    if event.button == 1 and event.key is None:
        plt.plot(newX[:, 0], newX[:, 1], '.b')
        newY = np.ones(len(newX)) * 1
    elif event.button == 3 and event.key is None:
        plt.plot(newX[:, 0], newX[:, 1], '.r')
        newY = np.ones(len(newX)) * -1
    Yplot = np.concatenate((Yplot, newY))

    fig.canvas.draw()


def onscroll(event):
    global plotvariance
    if event.button == "up":
        plotvariance = round(plotvariance + 0.2, 1)
    elif event.button == "down" and plotvariance > 0.1:
        plotvariance = round(plotvariance - 0.2, 1)
    print("Variance = ", plotvariance)


def onkeypress(event):
    if event.key == " ":
        monprogramme(Xplot, Yplot)


fig = plt.figure()

plt.axis([-5, 5, -5, 5])

cid = fig.canvas.mpl_connect("button_press_event", onclick)
cid2 = fig.canvas.mpl_connect("scroll_event", onscroll)
cid3 = fig.canvas.mpl_connect("key_press_event", onkeypress)

print("Utilisez la souris pour ajouter des points à la base d'apprentissage :")
print(" clic gauche : points bleus")
print(" clic droit : points rouges")
print("\nMolette : +/- variance ")
print("   si variance = 0  => ajout d'un point")
print("   si variance > 0  => ajout de points selon une loi gaussienne")
print("\n ESPACE pour lancer la fonction monprogramme(Xapp,Yapp)\n")

plt.show()
