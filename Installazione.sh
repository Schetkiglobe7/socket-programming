#!/bin/bash
#SCRIPT DI INSTALLAZIONE
#PROGETTO DI ANTONIO PILATO 0124001307

#se non sono stati creati, creo i file di memorizzazione dei dati
touch "blockchain".dat
touch "blocchi".dat

gcc nodoN.c -o nodoN -lpthread
gcc BlockServer.c -o BlockServer
gcc BlockClient.c -o BlockClient


#mi raccomando, se il file blockchain.dat è vuoto, lasciar generare una decina di blocchi poichè, per la simulazione è necessario iniziare con una blockchain non vuota.
#se si riscontra permession denied durante l'esecuzione del suddetto script, dare i permessi di esecuzione al file Installazione.sh
