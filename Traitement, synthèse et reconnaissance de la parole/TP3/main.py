import librosa
import numpy as np


def pre_accentuation(signal):
    signaldec = np.roll(signal, 1)
    signaldec[0] = 0
    output = signal - 0.97 * signaldec
    return output


def extraire_coefficients_MFCC():
    with open("Ref_NON.txt", "r") as f:
        for line in f:
            extraire_coefficient_MFCC(line.strip())

    with open("Ref_OUI.txt", "r") as f:
        for line in f:
            extraire_coefficient_MFCC(line.strip())

    with open("Test.txt", "r") as f:
        for line in f:
            extraire_coefficient_MFCC(line.strip())


def extraire_coefficient_MFCC(fichier):
    # Charger le fichier audio
    fichier_audio = "./FichiersAudio/" + fichier + ".wav"
    signal_audio, frequence_echantillonage = librosa.load(fichier_audio)

    # Définir la taille de la fenêtre et la période
    taille_fenetre = int(frequence_echantillonage * 0.025)  # 25ms
    hop_length = int(frequence_echantillonage * 0.01)  # 10ms
    n_mfcc = 13  # Nombre de coefficients MFCC
    window = np.hamming(taille_fenetre)

    # Calcul des coefficients MFCC
    mfcc = librosa.feature.mfcc(y=pre_accentuation(signal_audio),
                                sr=frequence_echantillonage,
                                win_length=taille_fenetre,
                                n_mfcc=n_mfcc,
                                hop_length=hop_length,
                                window=window)

    # Calcule de delta (dérivée) et delta-delta (accélération)
    delta_mfcc = librosa.feature.delta(mfcc)
    delta2_mfcc = librosa.feature.delta(mfcc, order=2)

    # Récupération des 12 coefficients (1 à 12) en enlevant le premier (numéro 0)
    mfcc_features = np.concatenate((mfcc[1:], delta_mfcc[1:], delta2_mfcc[1:]), axis=0)

    # Sauvegarde des coefficients MFCC dans un fichier
    mfcc_file = "./FichiersMFCC/" + fichier + ".mfcc"
    with open(mfcc_file, 'w') as f:
        # Nombre de vecteurs
        num_vectors = mfcc_features.shape[1]
        f.write(str(num_vectors) + '\n')
        # Écriture des coefficients
        for i in range(num_vectors):
            f.write('Vecteur {} : {}\n'.format(i + 1, ' '.join(str(x) for x in mfcc_features[:, i])))


# Calcule la distance euclidienne entre deux réels
def d(x, y):
    # Ajuster la taille des vecteurs pour qu'ils aient la même taille.
    diff = x.shape[0] - y.shape[0]
    if diff > 0:
        y = np.pad(y, (0, diff), 'constant', constant_values=0)
    else:
        x = np.pad(x, (0, -diff), 'constant', constant_values=0)
    # Calcul de la somme des carrés des différences entre les coordonnées de x et y.
    sum_diff_squares = np.sum((x - y) ** 2)
    # Calcul du produit des sommes des carrés des coordonnées de x et y.
    prod_sum_squares = np.sum(x ** 2) * np.sum(y ** 2)
    # Calcul de la distance locale d
    return sum_diff_squares / prod_sum_squares


# Calcule le score d'alignement
def calcul_score_alignement(test_features, ref_features):
    X = np.array(test_features)
    Y = np.array(ref_features)
    n, m = len(X), len(Y)

    M = np.zeros((n + 1, m + 1))

    for i in range(1, n + 1):
        M[i, 0] = 0

    for j in range(1, m + 1):
        M[0, j] = 0

    for i in range(1, n + 1):
        for j in range(1, m + 1):
            M[i][j] = min(M[i - 1][j - 1], M[i][j - 1], M[i - 1][j]) + d(X[i - 1], Y[j - 1])

    return M[n][m]


extraire_coefficients_MFCC()

ref_oui_features = []
ref_non_features = []

with open("Test.txt", "r") as f:
    for fichier in f:

        with open("Ref_OUI.txt", "r") as f2:
            for line2 in f2:
                if fichier != line2:
                    with open("./FichiersMFCC/" + line2[:-1] + ".mfcc", 'r') as a:
                        num_vectors = int(a.readline().strip())
                        features = np.zeros((36, num_vectors))
                        for i in range(num_vectors):
                            line2 = a.readline().strip().split(' ')
                            features[:, i] = np.array(line2[3:], dtype=np.float32)
                        ref_oui_features.append(features)

        with open("Ref_NON.txt", "r") as f2:
            for line2 in f2:
                if fichier != line2:
                    with open("./FichiersMFCC/" + line2[:-1] + ".mfcc", 'r') as a:
                        num_vectors = int(a.readline().strip())
                        features = np.zeros((36, num_vectors))
                        for i in range(num_vectors):
                            line2 = a.readline().strip().split(' ')
                            features[:, i] = np.array(line2[3:], dtype=np.float32)
                        ref_non_features.append(features)

        with open("./FichiersMFCC/" + fichier[:-1] + ".mfcc", 'r') as a:
            num_vectors = int(a.readline().strip())
            test_features = np.zeros((36, num_vectors))
            for i in range(num_vectors):
                line = a.readline().strip().split(' ')
                test_features[:, i] = np.array(line[3:], dtype=np.float32)

        oui_scores = []
        for ref_features in ref_oui_features:
            score = calcul_score_alignement(test_features, ref_features)
            oui_scores.append(score)

        non_scores = []
        for ref_features in ref_non_features:
            score = calcul_score_alignement(test_features, ref_features)
            non_scores.append(score)

        mean_oui_score = np.mean(oui_scores)
        mean_non_score = np.mean(non_scores)
        min_oui_score = np.min(oui_scores)
        min_non_score = np.min(non_scores)

        print("Résultats pour " + str(fichier[:-1]) + " : \n")

        # Print results
        print("Moyenne des scores pour les fichiers de référence OUI : {:.4f}".format(mean_oui_score))
        print("Score minimum pour les fichiers de référence OUI : {:.4f}".format(min_oui_score))
        print("Moyenne des scores pour les fichiers de référence NON : {:.4f}".format(mean_non_score))
        print("Score minimum pour les fichiers de référence NON : {:.4f}".format(min_non_score))

        if min_oui_score < min_non_score:
            test_result = 'OUI'
        else:
            test_result = 'NON'

        print("Résultat du test avec minimum : ", test_result)

        if mean_oui_score < mean_non_score:
            test_result = 'OUI'
        else:
            test_result = 'NON'

        print("Résultat du test avec moyenne : ", test_result + "\n")
