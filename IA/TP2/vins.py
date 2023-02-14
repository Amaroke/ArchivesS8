import numpy as np


### fonctions ADL à copier ici

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


### programme principal ###

# chargement des données
X = np.loadtxt("vinsX.dat")
y = np.loadtxt("vinsY.dat")

# Découpage des données en une base d'apprentissage de m exemples
# et une base de test
indices = np.random.permutation(len(y))
m = 4000
Xapp = X[indices[0:m], :]
yapp = y[indices[0:m]]

Xtest = X[indices[m:], :]
ytest = y[indices[m:]]

# Apprentissage...

(w, b) = adl(Xapp, yapp)
Ypred = adlpredict(Xtest, w, b)
print("Pourcentage d'erreur : " + str(round(np.mean(Ypred != ytest) * 100, 5)) + "%")
