from sys import argv

import numpy as np
import numpy.fft as FFT
from matplotlib import pyplot as plt
from scipy.io.wavfile import read, write

def enregistrer_signal(signal, signal_modif, frequence_echantillonage):
    temps = np.arange(len(signal)) / float(frequence_echantillonage)

    # Créer une figure avec deux sous-figures empilées verticalement
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), sharex=True)

    # Plot du premier signal
    ax1.plot(temps, signal)
    ax1.set_ylabel('Amplitude')
    ax1.set_title('Signal récupéré')

    # Plot du second signal
    ax2.plot(temps, signal_modif)
    ax2.set_ylabel('Amplitude')
    ax2.set_title('Signal modifié')

    ax1.xaxis.set_visible(False)
    ax2.xaxis.set_visible(False)
    ax1.yaxis.set_visible(False)
    ax2.yaxis.set_visible(False)

    # Afficher les deux plots
    plt.savefig("Diagrammes/spectre_signal_" + argv[1][:-4] +".png")

def afficher_spectre_amplitude(spectres_amplitude):
    plt.imshow(spectres_amplitude, cmap='gray', aspect='auto', interpolation='nearest')
    plt.xlabel('Temps')
    plt.ylabel('Fréquence')
    plt.show()
    plt.close()

# Ouvre le wav et affiche les informations du fichier
def ouverture_wav(fichier):
    # Ouvrir le fichier wav
    frequence_enchantillonage, valeurs_signal = read(fichier)

    # Afficher les informations
    nb_echantillon = valeurs_signal.shape[0]
    duree_ms = 1000 * nb_echantillon / frequence_enchantillonage
    print("Fréquence d'échantillonnage : " + str(frequence_enchantillonage) + " Hz\nTaille du fichier : " + str(
        nb_echantillon) + " échantillons, soit " + str(duree_ms) + " ms")

    return frequence_enchantillonage, valeurs_signal, nb_echantillon


def fenetrageHamming(size):
    return np.hamming(size)


# fonction qui calcule le spectre d'amplitude
def spectreamplitude(spectre, fftsize):
    return np.abs(spectre[:fftsize])


def spectrephase(spectre, fftsize):
    return np.angle(spectre[:fftsize])


def spectrereconstruction(spectre_amplitude, spectre_phase):
    return spectre_amplitude * np.exp(1j * spectre_phase)


def spectreAmplitudeDebruitage(spectre_amplitude, spectre_amplitude_bruit, fftsize):
    alpha = 1
    beta = 20
    gamma = 0
    spectre_amplitude_debruite = np.zeros(fftsize)
    for i in range(fftsize):
        if spectre_amplitude[i] > beta * spectre_amplitude_bruit[i]:
            spectre_amplitude_debruite[i] = (spectre_amplitude[i] ** alpha - beta * spectre_amplitude_bruit[i] ** alpha) ** (1 / alpha)
        else:
            spectre_amplitude_debruite[i] = gamma * spectre_amplitude_bruit[i]
    return spectre_amplitude_debruite


# Étape 1 : Récupération du signal
frequence_echantillonage, valeurs_signal, nb_echantillon = ouverture_wav(argv[1])

# Étape 2 : Boucle OLA
fenetre_signal = int(frequence_echantillonage * 0.032)  # 32 ms
m = int(frequence_echantillonage * 0.008)  # 8 ms
nb_fenetres = int((len(valeurs_signal) - fenetre_signal) / m) + 1
tableau_temporaire_32ms = np.zeros((nb_fenetres, fenetre_signal))
# Récupération de la fenêtre à l’instant i et de taille m (2)
for i in range(nb_fenetres):
    debut_fenetre = i * m
    fin_fenetre = debut_fenetre + fenetre_signal
    tableau_temporaire_32ms[i] = valeurs_signal[debut_fenetre:fin_fenetre]

# Fenêtrage Hamming (2.)
fftsize = 1024
signal_modif = np.zeros(len(valeurs_signal))
somme_hamming = np.zeros(len(valeurs_signal))
spectres_amplitude_log = []
moyenne_amplitude_bruit = np.zeros(fftsize)
for i in range(len(tableau_temporaire_32ms)):
    tableau_temporaire_32ms[i] = tableau_temporaire_32ms[i] * fenetrageHamming(fenetre_signal)
    fourier = (FFT.fft(tableau_temporaire_32ms[i], fftsize))

    # Spectre d’amplitude (4.)
    spectre_amplitude = spectreamplitude(fourier, fftsize)
    spectre_amplitude_log = 20 * np.log10(spectre_amplitude)
    # Ajouter le spectre d'amplitude au tableau de spectres pour l'affichage
    spectres_amplitude_log.append(spectre_amplitude_log)

    # Spectre de phase (6.)
    spectre_phase = spectrephase(fourier, fftsize)

    # Traitement sur le spectre d’amplitude (8. 9.)
    if i < 5:
        moyenne_amplitude_bruit += spectre_amplitude
    elif i == 5:
        moyenne_amplitude_bruit = moyenne_amplitude_bruit / 5

    spectre_amplitude_debruite = spectreAmplitudeDebruitage(spectre_amplitude, moyenne_amplitude_bruit, fftsize)

    # Reconstruction du spectre (7.)
    spectre_reconstruit = spectrereconstruction(spectre_amplitude_debruite, spectre_phase)

    # Fourier inverse (3.)
    fourier_inverse = np.real(FFT.ifft(spectre_reconstruit, fftsize))

    # Reconstruction du signal (2.) : ajout de la fenêtre au signal final
    signal_modif[i * m:i * m + fftsize] += fourier_inverse[:signal_modif[i * m:i * m + fftsize].shape[0]]
    somme_hamming[i * m:i * m + fftsize] += fenetrageHamming(signal_modif[i * m:i * m + fftsize].shape[0])

# 5. Sauvegarde du spectre d’amplitude
plt.imshow(np.array(spectres_amplitude_log).transpose()[:fftsize // 2], aspect='auto')
plt.colorbar()
plt.savefig("Diagrammes/spectre_amplitude_" + argv[1][:-4] +".png")
plt.close()

# Reconstruction du signal (2.) : division par la somme des fenêtres de Hamming
for i in range(len(somme_hamming)):
    if somme_hamming[i] == 0:
        somme_hamming[i] = 1
signal_modif = signal_modif / somme_hamming*2

# Création du fichier wav
write("resultat_" + argv[1], frequence_echantillonage, np.int16(signal_modif))

# On enregistre les diagrammes
enregistrer_signal(valeurs_signal, signal_modif, frequence_echantillonage)