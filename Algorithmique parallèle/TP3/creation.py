#!/bin/python3
# -*- coding: utf-8 -*-

# Programme MPI simple
# Execution : invocation directe dans un terminal ou via python3

# Import de MPI
import mpi4py.MPI as MPI


# Fonction principale
def main():
    # Initialisation MPI
    comm = MPI.COMM_WORLD  # Groupe de communication
    idP = comm.Get_rank()  # Identifiant du processus
    nbP = comm.Get_size()  # Nombre de processus

    if idP != 0:
        source = (idP - 1 + nbP) % nbP
        message = comm.recv(source=source, tag=0)
        print("Je suis le processus " + str(idP) + " j'ai reçu " + message)
    else:
        print("Je suis le processus 0")

    if idP != nbP - 1:
        destination = (idP + 1) % nbP
        comm.send("Message de processus %d" % idP, dest=destination, tag=0)


# Programme principal
if __name__ == "__main__":
    main()
