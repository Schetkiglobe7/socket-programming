#include <netinet/in.h>

//tipo transazione
typedef struct{
    struct sockaddr_in mittente; //ip e porta del mittente
    int ammontare; //l'ammontare della transazione
    struct sockaddr_in destinatario; //ip e porta del destinatario
}transazione;

//tipo blocco della blockchain
 typedef struct{
    int nBlocco; //numero di blocco progressivo
    int attesaR; //attesa random per l'inserimento nella blockchain
    transazione transaction; //transazione nel blocco
}blocco;

//tipo blockchain, implementata come una lista a doppio puntatore.
struct blockchain{
    blocco bloccoData;
    struct blockchain* next;
    struct blockchain* prev;
};
typedef struct blockchain Blockchain;
