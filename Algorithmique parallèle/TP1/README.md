# Exercice 1
## a)
Partie séquentielle
Processus 6 parmi 8
Processus 5 parmi 8
Processus 7 parmi 8
Processus 2 parmi 8
Processus 0 parmi 8
Processus 4 parmi 8
Processus 1 parmi 8
Processus 3 parmi 8
Partie séquentielle finale.

Sur 8 "thread".

## b)
Partie séquentielle
Processus 0 parmi 1

Pas de paraléllisme.

## c)
Partie séquentielle
Processus 1 parmi 12
Processus 3 parmi 12
Processus 4 parmi 12
Processus 7 parmi 12
Processus 9 parmi 12
Processus 0 parmi 12
Processus 8 parmi 12
Processus 2 parmi 12
Processus 11 parmi 12
Processus 5 parmi 12
Processus 10 parmi 12
Processus 6 parmi 12
Partie séquentielle finale.

L'argument force 12 threads.

## d) 
Partie séquentielle
Processus 0 parmi 3
Processus 2 parmi 3
Processus 1 parmi 3
Partie séquentielle finale.

Avec ou sans argument on utilise que 3 threads.

# Exercice 2
## a)
Processus 0
Proc 0 calcul itération 0
Proc 0 calcul itération 1
Proc 0 calcul itération 2
Proc 0 calcul itération 3
Proc 0 calcul itération 4
Processus 1
Proc 1 calcul itération 5
Proc 1 calcul itération 6
Proc 1 calcul itération 7
Proc 1 calcul itération 8
Proc 1 calcul itération 9
Processus 2
Proc 2 calcul itération 10
Proc 2 calcul itération 11
Proc 2 calcul itération 12
Proc 2 calcul itération 13
Proc 2 calcul itération 14
Processus 3
Proc 3 calcul itération 15
Proc 3 calcul itération 16
Proc 3 calcul itération 17
Proc 3 calcul itération 18
Proc 3 calcul itération 19
Cumul = 20 / 0.000804s

5 itérations sur chacun des 4 processus.

## b)
Processus 1
Proc 1 calcul itération 2
Proc 1 calcul itération 3
Proc 1 calcul itération 10
Proc 1 calcul itération 11
Proc 1 calcul itération 18
Proc 1 calcul itération 19
Processus 3
Proc 3 calcul itération 6
Proc 3 calcul itération 7
Proc 3 calcul itération 14
Proc 3 calcul itération 15
Processus 2
Proc 2 calcul itération 4
Proc 2 calcul itération 5
Proc 2 calcul itération 12
Proc 2 calcul itération 13
Processus 0
Proc 0 calcul itération 0
Proc 0 calcul itération 1
Proc 0 calcul itération 8
Proc 0 calcul itération 9
Proc 0 calcul itération 16
Proc 0 calcul itération 17
Cumul = 20 / 0.000667s

Distribution par bloc.

## c)
Processus 2
Processus 0
Processus 1
Processus 3
Cumul = 315632 / 0.003991s

Avec 1.000.000 d'itérations seulement 315632 opérations sont effectués, car les processus écrivent en même temps sur la variable cumul avec l'instruction "cumul++".

## d)
Processus 1
Processus 3
Processus 2
Processus 0
Cumul = 1000000 / 0.025210s

Problème résolu, cumul devient atomic plus aucune écriture simultanée possible.

## e)
Processus 0
Processus 2
Processus 1
Processus 3
Cumul = 1000000 / 0.000809s

Le problème est également résolu en utilisant la réduction et c'est bien plus rapide.

# Exercice 3
## a)
Proc 3 termine en 2.821220s
Proc 0 termine en 2.821274s
Proc 1 termine en 2.821279s
Proc 2 termine en 2.821279s

## b)
Proc 0 termine en 0.405036s
Proc 1 termine en 1.296994s
Proc 2 termine en 2.089688s
Proc 3 termine en 2.859114s

Les processeurs ne s'attendent plus, et finissent donc l'un après l'autre.

## c)
Proc 0 termine en 1.644343s
Proc 3 termine en 1.690604s
Proc 1 termine en 1.703097s
Proc 2 termine en 1.705428s

On utilise une distribution a la volée, ainsi c'est comme si on avait un processus courant qui distribue les itérations aux processeurs disponnibles.

# Exercice 4
## a)
Thread de niveau 1 : 0
  Thread de niveau 2 : 0 (0)
Thread de niveau 1 : 1
    Thread de niveau 3 : 0 (0 0)
  Thread de niveau 2 : 0 (1)
    Thread de niveau 3 : 0 (1 0)

Les Threads ne créé pas le bon nombres de sous Threads.

## b)
Thread de niveau 1 : 0
Thread de niveau 1 : 1
  Thread de niveau 2 : 0 (0)
  Thread de niveau 2 : 1 (0)
  Thread de niveau 2 : 2 (0)
  Thread de niveau 2 : 0 (1)
  Thread de niveau 2 : 1 (1)
  Thread de niveau 2 : 2 (1)
    Thread de niveau 3 : 3 (0 0)
    Thread de niveau 3 : 2 (0 0)
    Thread de niveau 3 : 1 (0 0)
    Thread de niveau 3 : 0 (0 0)
    Thread de niveau 3 : 2 (0 1)
    Thread de niveau 3 : 3 (0 1)
    Thread de niveau 3 : 0 (0 1)
    Thread de niveau 3 : 1 (0 1)
    Thread de niveau 3 : 1 (0 2)
    Thread de niveau 3 : 3 (0 2)
    Thread de niveau 3 : 0 (0 2)
    Thread de niveau 3 : 2 (0 2)
    Thread de niveau 3 : 1 (1 0)
    Thread de niveau 3 : 3 (1 0)
    Thread de niveau 3 : 0 (1 0)
    Thread de niveau 3 : 2 (1 0)
    Thread de niveau 3 : 1 (1 2)
    Thread de niveau 3 : 3 (1 2)
    Thread de niveau 3 : 0 (1 2)
    Thread de niveau 3 : 1 (1 1)
    Thread de niveau 3 : 0 (1 1)
    Thread de niveau 3 : 2 (1 2)
    Thread de niveau 3 : 2 (1 1)
    Thread de niveau 3 : 3 (1 1)

Chaque Thread créé le bon nombre de sous thread.

# Exercice 5
## a)
Thread 0 calcule le travail 0 en 0.067148s
Thread 2 calcule le travail 0 en 0.067289s
Thread 3 calcule le travail 0 en 0.069865s
Thread 0 calcule le travail 1 en 0.130026s
Thread 1 calcule le travail 1 en 0.133296s
Thread 3 calcule le travail 1 en 0.131268s
Thread 2 calcule le travail 1 en 0.136365s
Thread 0 calcule le travail 2 en 0.193931s
Thread 1 calcule le travail 2 en 0.198146s
Thread 3 calcule le travail 2 en 0.201212s
Thread 2 calcule le travail 2 en 0.206200s
Thread 0 calcule le travail 3 en 0.257939s
Thread 1 calcule le travail 3 en 0.258401s
Thread 3 calcule le travail 3 en 0.265425s
Thread 2 calcule le travail 3 en 0.262275s
Thread 0 calcule le travail 4 en 0.328321s
Thread 3 calcule le travail 4 en 0.327764s
Thread 1 calcule le travail 4 en 0.342798s
Thread 2 calcule le travail 4 en 0.335420s
Thread 0 calcule le travail 5 en 0.383020s
Thread 3 calcule le travail 5 en 0.401993s
Thread 1 calcule le travail 5 en 0.402796s
Thread 2 calcule le travail 5 en 0.397932s
Thread 0 calcule le travail 6 en 0.457712s
Thread 1 calcule le travail 6 en 0.464143s
Thread 3 calcule le travail 6 en 0.471186s
Thread 2 calcule le travail 6 en 0.468953s
Thread 0 calcule le travail 7 en 0.515205s
Thread 2 calcule le travail 7 en 0.518413s
Thread 1 calcule le travail 7 en 0.530115s
Thread 3 calcule le travail 7 en 0.538499s
Thread 0 calcule le travail 8 en 0.574364s
Thread 2 calcule le travail 8 en 0.572314s
Thread 1 calcule le travail 8 en 0.597361s
Thread 3 calcule le travail 8 en 0.608434s
Thread 0 calcule le travail 9 en 0.644885s
Thread 2 calcule le travail 9 en 0.660904s
Thread 1 calcule le travail 9 en 0.659969s
Thread 3 calcule le travail 9 en 0.642190s
Thread 0 calcule le travail 10 en 0.706088s
Thread 2 calcule le travail 10 en 0.724372s
Thread 1 calcule le travail 10 en 0.734470s
Thread 3 calcule le travail 10 en 0.745240s
Thread 0 calcule le travail 11 en 0.768052s
Thread 2 calcule le travail 11 en 0.812697s
Thread 1 calcule le travail 11 en 0.786437s
Thread 3 calcule le travail 11 en 0.795398s
Thread 0 calcule le travail 12 en 0.831122s
Thread 3 calcule le travail 12 en 0.833765s
Thread 2 calcule le travail 12 en 0.876223s
Thread 1 calcule le travail 12 en 0.869797s
Thread 0 calcule le travail 13 en 0.899196s
Thread 3 calcule le travail 13 en 0.931359s
Thread 1 calcule le travail 13 en 0.927442s
Thread 2 calcule le travail 13 en 0.946258s
Thread 0 calcule le travail 14 en 0.972965s
Thread 3 calcule le travail 14 en 1.000449s
Thread 1 calcule le travail 14 en 1.004394s
Thread 2 calcule le travail 14 en 0.994117s
Thread 0 calcule le travail 15 en 1.033907s
Thread 2 calcule le travail 15 en 1.042128s
Thread 3 calcule le travail 15 en 1.063568s
Thread 1 calcule le travail 15 en 1.073748s
Thread 0 calcule le travail 16 en 1.087034s
Thread 3 calcule le travail 16 en 1.131960s
Thread 2 calcule le travail 16 en 1.157899s
Thread 1 calcule le travail 16 en 1.136940s
Thread 0 calcule le travail 17 en 1.171063s
Thread 3 calcule le travail 17 en 1.208090s
Thread 1 calcule le travail 17 en 1.194681s
Thread 2 calcule le travail 17 en 1.242612s
Thread 0 calcule le travail 18 en 1.218282s
Thread 3 calcule le travail 18 en 1.253460s
Thread 2 calcule le travail 18 en 1.227659s
Thread 1 calcule le travail 18 en 1.270356s
Thread 0 calcule le travail 19 en 1.269386s
Thread 3 calcule le travail 19 en 1.311626s
Thread 1 calcule le travail 19 en 1.342710s
Thread 2 calcule le travail 19 en 1.363519s
Calcul terminé en 14.014765s

## b)
Thread 1 calcule le travail 0 en 0.063215s
Thread 1 calcule le travail 1 en 0.128860s
Thread 1 calcule le travail 2 en 0.186507s
Thread 1 calcule le travail 3 en 0.251252s
Thread 1 calcule le travail 4 en 0.312982s
Thread 1 calcule le travail 5 en 0.397713s
Thread 1 calcule le travail 6 en 0.427902s
Thread 1 calcule le travail 7 en 0.526596s
Thread 1 calcule le travail 8 en 0.589961s
Thread 1 calcule le travail 9 en 0.647550s
Thread 1 calcule le travail 10 en 0.671366s
Thread 1 calcule le travail 11 en 0.789081s
Thread 1 calcule le travail 12 en 0.837155s
Thread 1 calcule le travail 13 en 0.843579s
Thread 1 calcule le travail 14 en 0.945583s
Thread 1 calcule le travail 15 en 0.984815s
Thread 1 calcule le travail 16 en 1.071592s
Thread 1 calcule le travail 17 en 1.089960s
Thread 1 calcule le travail 18 en 1.162960s
Thread 1 calcule le travail 19 en 1.220378s

Un seul Thread utilisé.

## c)
Thread 1 calcule le travail 0 en 0.066944s
Thread 2 calcule le travail 1 en 0.132251s
Thread 3 calcule le travail 2 en 0.201030s
Thread 0 calcule le travail 3 en 0.266373s
Thread 1 calcule le travail 4 en 0.342865s
Thread 2 calcule le travail 5 en 0.418218s
Thread 3 calcule le travail 6 en 0.488678s
Thread 0 calcule le travail 7 en 0.545758s
Thread 1 calcule le travail 8 en 0.620189s
Thread 2 calcule le travail 9 en 0.674559s
Thread 3 calcule le travail 10 en 0.741598s
Thread 0 calcule le travail 11 en 0.769276s
Thread 1 calcule le travail 12 en 0.891475s
Thread 2 calcule le travail 13 en 0.922897s
Thread 3 calcule le travail 14 en 0.981364s
Thread 0 calcule le travail 15 en 1.018756s
Thread 1 calcule le travail 16 en 1.127025s
Thread 2 calcule le travail 17 en 1.179082s
Thread 3 calcule le travail 18 en 1.233526s
Thread 0 calcule le travail 19 en 1.270121s
Calcul terminé en 3.870724s

On utilise les 4 Threads pour faire les calculs, mais ils se partagent les tâches.

# Exercice section
## a)
Retour à la section parallèle commune après 5.001149s : 2
Retour à la section parallèle commune après 5.001166s : 0
Retour à la section parallèle commune après 5.001213s : 1

## b)
Retour à la section parallèle commune après 2.000340s : 1
Retour à la section parallèle commune après 4.001055s : 2
Retour à la section parallèle commune après 5.000810s : 0

## c)
Retour à la section parallèle commune après 5.000802s : 0
Retour à la section parallèle commune après 5.000859s : 1
Retour à la section parallèle commune après 5.000844s : 2

La barrière à "désactiver" le nowait.

## d)
Retour à la section parallèle commune après 5.000763s : 1
Retour à la section parallèle commune après 5.000805s : 2
Retour à la section parallèle commune après 5.000820s : 0

Single fait une barrière implicite.

## e)
Retour à la section parallèle commune après 3.001285s : 2
Retour à la section parallèle commune après 4.001217s : 0
Retour à la section parallèle commune après 5.000614s : 1

nowait désactive la barrière implicite du single.

## f)
Proc 1 effectue la section 2
Retour à la section parallèle commune après 2.000460s : 2
Proc 0 effectue la section 1
Retour à la section parallèle commune après 4.000864s : 1
Proc␣0␣fait␣le␣single
Retour à la section parallèle commune après 6.001404s : 0