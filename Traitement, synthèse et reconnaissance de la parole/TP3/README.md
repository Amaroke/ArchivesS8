# <center>Rapport Projet Parole 3</center>
## <center>Reconnaissance de la parole (MFCC, DTW)</center>
### <center>Hugo MATHIEU STEINBACH & Guillaume ZIMOL<center>

## Fonctionnement du programme

Pour exécuter le programme un simple python3 main.py
Il faut cependant avoir au-préalable exécuter les commandes suivantes :<br>
- pip install numpy
- pip install librosa

Le code commence par appeler la fonction extraire_coefficients_MFCC() pour extraire les coefficients MFCC de tous les fichiers de référence et de test.

Ensuite, pour chaque fichier de test, le code parcourt tous les fichiers de référence pour extraire leurs coefficients MFCC et calculer le score d'alignement avec le fichier de test en utilisant la fonction calcul_score_alignement(). Le score d'alignement est calculé pour chaque fichier de référence. Les scores moyens et minimaux pour les fichiers de référence "OUI" et "NON" sont également calculés.

Enfin, le code compare les scores minimaux pour les fichiers de référence "OUI" et "NON" pour déterminer si le fichier de test correspond à une réponse "OUI" ou "NON". Le même processus est répété avec les scores moyens.

## Phase de test

Ici, nous répondons aux différentes questions du sujet, notons que comme les résultats sont de faible qualité (du fait de l'enregistrement trop "rapide" pour les oui comparé à ceux de références), nous avons évalué les oui et non de référence en les retirant à chaque fois de notre base de référence.
<br>Un fichier contenant la sortie du programme `sortie.txt` est disponible.
Pour répondre au question nous utilisons le score moyen et non pas le minimum et nous utilisons les fichiers `_guillaume`.
1.  Distance à "oui" avec nuit : 18% <br>
    Distance à "non" avec nuit : 32% <br>
    Distance à "oui" avec buis : 8% <br>
    Distance à "non" avec buis : 13% <br>
    Distance à "oui" avec bon : 7% <br>
    Distance à "non" avec bon : 4% <br>
    Distance à "oui" avec don : 31% <br>
    Distance à "non" avec don : 17% <br>

2. Pour rejeter un fichier qui n'est ni "OUI" ni "NON", le code peut être modifié pour inclure une autre condition qui teste si le score minimum pour les deux réponses est supérieur au seuil calculé. Si cette condition est satisfaite, cela signifie que les scores pour les réponses "oui" et "non" sont tous deux supérieurs au seuil, ce qui indique que le fichier n'a pas été correctement classé comme étant "oui" ou "non". Dans ce cas, le résultat du test peut être défini comme "REJET" ou un message d'erreur peut être généré pour indiquer que le fichier n'a pas pu être classé avec précision. 

3. Comme on peut le constater avec notre fichier de sortie, le pourcentage de réussite avec les fichiers de référence est de 100%. (8 + 8) / 16 = 1<br>
Avec nos fichiers, nous sommes à 75% (3/4) avec le score moyen et 50% (2/4) avec le score utilisant le minimum.