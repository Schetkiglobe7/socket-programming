#include "./Wrapper.h"
#include "./UtilityTypes.h"

void ProtocolloApplicazione(int sockClient)
{
    fd_set setLettura,setScrittura;
    int maxd;
    int risposta;
    int scelta;
    int j,k;
    int h;
    int porta;
    int iterazioni,n;
    int ip;
    transazione Transaction;
    Transaction.ammontare=0;
    Transaction.destinatario.sin_addr.s_addr =htonl(0);
    Transaction.destinatario.sin_port = htons(0);
    Transaction.mittente.sin_addr.s_addr = htonl(0);
    Transaction.mittente.sin_port = htons(0);
    
    while(1){
        FD_ZERO(&setLettura);
        FD_ZERO(&setScrittura);
        FD_SET(sockClient,&setLettura);
        maxd = sockClient+1;
        
        if(select(maxd,&setLettura,&setLettura,NULL,NULL)<0){
            perror("errore sulla select");
            exit(-1);
        }
        //inizio sezione di lettura e scrittura sulla socket
        if(FD_ISSET(sockClient,&setLettura)){
            printf("\nBenvenuto nel programma di gestione della BlockChain\n");
            printf("hai a disposizione 4 funzionalita'\n");
            printf("prego inserire:\n0)per uscire\n");
            printf("1)per vedere le ultime N transazioni\n");
            printf("2)per vedere una transazione specifica\n");
            printf("3)per vedere la somma di valore di tutte le transazioni\n");
            printf("4)per vedere il numero di transazioni in cui e' coinvolto un indirizzo specifico(ip-porta)\n");
            
            scanf("%d",&scelta);
            printf("\nscelta: %d\n",scelta);
            FullWrite(sockClient,&scelta,sizeof(int));
            FullRead(sockClient,&risposta,sizeof(int));
            printf("\nriposta: %d\n",risposta);
            if(scelta == 1){ //ricezione delle ultime 5 transazioni e visualizzazione a display
                printf("\nquante transazioni vuoi visualizzare?\n");
                scanf("%d",&iterazioni);
                FullWrite(sockClient,&iterazioni,sizeof(int));
                for(n=0;n<iterazioni;n++){
                    FullRead(sockClient,&Transaction,sizeof(Transaction));
                    printf("\n---------------------------------------------------\n");
                    printf("\nporta mittente: %d\n",Transaction.mittente.sin_port);
                    printf("\nindirizzo ip mittente: %d\n",Transaction.mittente.sin_addr.s_addr);
                    printf("\nammontare: %d\n",Transaction.ammontare);
                    printf("\nporta destinatario: %d",Transaction.destinatario.sin_port);
                    printf("\nindirizzo ip destinatario: %d\n",Transaction.destinatario.sin_addr.s_addr);
                    printf("\n---------------------------------------------------\n");
                }
            }
            if(scelta == 2){//ricezione di una transazione specifica
                printf("\ninserire il numero di transazione che si vuole visualizzare\n");
                scanf("%d",&k);
                FullWrite(sockClient,&k,sizeof(int));
                FullRead(sockClient,&Transaction,sizeof(Transaction));
                printf("\n---------------------------------------------------\n");
                printf("\nporta mittente: %d\n",Transaction.mittente.sin_port);
                printf("\nindirizzo ip mittente: %d\n",Transaction.mittente.sin_addr.s_addr);
                printf("\nammontare: %d\n",Transaction.ammontare);
                printf("\nporta destinatario: %d",Transaction.destinatario.sin_port);
                printf("\nindirizzo ip destinatario: %d\n",Transaction.destinatario.sin_addr.s_addr);
                printf("\n---------------------------------------------------\n");
            }
            if(scelta == 3){//ricezione della somma dei valori di tutte le transazioni
                FullRead(sockClient,&h,sizeof(int));
                printf("\ntotale ricevuto: %d\n",h);
            }
            if(scelta == 4){//ricezione della somma dei valori di tutte le transazioni relative ad un indirizzo ip(ip/porta)
                printf("\ninserire ip da monitorare\n");
                scanf("%d",&ip);
                printf("\nip: %d\n",ip);
                fflush(stdin);
                printf("\ninserire porta da monitorare\n");
                scanf("%d",&porta);
                printf("\nporta: %d\n",porta);
                FullWrite(sockClient,&ip,sizeof(int));
                FullWrite(sockClient,&porta,sizeof(int));
                FullRead(sockClient,&h,sizeof(int));
                printf("\ntotale ricevuto: %d\n",h);
            }
            if(scelta == 0){
                printf("\ntermino\n");
                close(sockClient);
                exit(0);
            }
        }
    }
}
