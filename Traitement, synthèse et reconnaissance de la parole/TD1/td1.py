from scipy.io.wavfile import read
import matplotlib.pyplot as plt
import numpy as np


# Ouvre le wav et affiche les informations du fichier
def ouverture_wav():
    # Ouvrir le fichier wav
    fichier = 'test_seg.wav'
    frequence_enchantillonage, valeurs_signal = read(fichier)

    # Afficher les informations
    nb_echantillon = valeurs_signal.shape[0]
    duree_ms = 1000 * nb_echantillon / frequence_enchantillonage
    print("Fréquence d'échantillonnage : " + str(frequence_enchantillonage) + " Hz\nTaille du fichier : " + str(
        nb_echantillon) + " échantillons, soit " + str(duree_ms) + " ms")

    return frequence_enchantillonage, valeurs_signal, nb_echantillon


def calculer_energie(signal, m, N):
    nb_fenetres = int((len(signal) - N) / m) + 1
    energies = np.zeros(nb_fenetres)
    # Calculer l'énergie de chaque fenêtre
    for i in range(nb_fenetres):
        debut_fenetre = i * m
        fin_fenetre = debut_fenetre + N
        fenetre = signal[debut_fenetre:fin_fenetre]
        energie = np.sum(np.square(np.float32(fenetre)))  # Calcul de l'énergie
        energie /= N  # On normalise
        energie_db = 10 * np.log10(energie + 1e-10)  # Conversion en dB
        energies[i] = energie_db
    return energies


def afficher_signal_et_energie(valeurs_signal, nb_echantillon, energies):
    fig, (ax1, ax2) = plt.subplots(2, figsize=(20, 10))

    # Signal
    ax1.plot(valeurs_signal)
    ax1.set_xlabel('Temps (echt)')
    ax1.set_xlim(0, nb_echantillon)  # On limite entre 0 et le max

    # Courbe énergie
    ax2.plot(energies)
    ax2.set_ylabel('Énergie (dB)')
    ax2.set_xlim(0, len(energies))  # On limite entre 0 et le max

    plt.tight_layout()
    plt.show()


# Ouverture du .wav
frequence_enchantillonage, valeurs_signal, nb_echantillon = ouverture_wav()

m = int(frequence_enchantillonage * 0.004)  # 4 ms
N = int(frequence_enchantillonage * 0.01)  # 10 ms

# Calcule de l'énergie
energies = calculer_energie(valeurs_signal, m, N)

# Affichage du signal et de la courbe d'énergie
afficher_signal_et_energie(valeurs_signal, nb_echantillon, energies)
