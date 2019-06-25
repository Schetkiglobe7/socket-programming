#include "./UtilityTypes.h"
#include "./Wrapper.h"
#define MAXLINE 256

void insertBlock(blocco blockToInsert);
int readfromFile();
transazione Last5Transactions(int indice);
int TotalAmount();
int specificAmount(int porta, int indirizzoIp);
void ProtocolloApplicazione(int sockClient,blocco b,int sockServer)
{
    int conn_fd;
    char bufferIp[1024];
    fd_set setLettura,setScrittura;
    int maxd,numero,ritSel,n;
    pid_t pid;
    socklen_t len;
    struct sockaddr_in clientaddr;
    blocco blocc;
    transazione Transaction;
    srand(time(NULL));
    int i=0,j;
    int h;
    int iterazioni,q;
    int sincro=0;
    int ip,porta;
    
    while(1){
        //implementazione di server ibrido(iterativo per l'utilizzo della select e concorrente per mezzo della creazione dei figli per la gestione del canale di comunicaizone dei client con il server)
        FD_ZERO(&setLettura);
        FD_ZERO(&setScrittura);
        FD_SET(sockServer,&setLettura);
        FD_SET(sockClient,&setScrittura);

        if(sockServer > sockClient){
            maxd = sockServer+1;
        }else{
            maxd = sockClient+1;
        }
        if(select(maxd,&setLettura,&setScrittura,NULL,NULL)<0){
            perror("errore sulla select");
            exit(-1);
        }
        //inizio sezione operazioni di lettura e scrittura sulla socket
        if(FD_ISSET(sockClient,&setScrittura)){
            //invia il numero di blocco che si vuole ricevere
            i=readfromFile();
            //printf("\ni prima della richiesta: %d\n",i);
            FullWrite(sockClient,&i,1);
            i++;
            //legge il blocco inviatogli dal nodoN
            FullRead(sockClient,&blocc,sizeof(blocco));
            //printf("\nnBlocco letto da nodoN: %d\n",blocc.nBlocco);
            //Si procede a salvare il blocco in locale
            insertBlock(blocc);
            sleep(2);
        }
        if(FD_ISSET(sockServer,&setLettura)){
            //quando leggo il blocco(dato che il il nodoN che ho implementato utilizza un thread che genera blocchi in continuazione) non uso meccanismi di sincronizzazione per evitare una race condition sulla lettura degli ultimi blocchi(mentre leggo il blocco 5 come ultimo blocco, il thread del nodoN può generare altri blocchi,6,7,8 etc, e quindi il blocco 5 non è più l'ultimo blocco come dice il blockServer RACE CONDITION E.S: prenotaizone volo aereo)
            conn_fd = Accept(sockServer,(struct sockaddr *)&clientaddr,&len);
            len = sizeof(clientaddr);
            inet_ntop(AF_INET,&clientaddr.sin_addr,bufferIp,sizeof(bufferIp));
            printf("\nAccetto la connessione del client\n-IP:%s\t-PORTA: %d\n",bufferIp,clientaddr.sin_port);
            pid =Fork();
            if(pid ==0){//sono il figlio
                do{
              
                close(sockServer);
              
                FullRead(conn_fd,&sincro,sizeof(int));
              
                FullWrite(conn_fd,&sincro,sizeof(int));
                printf("\nsincro: %d\n",sincro);
                if(sincro == 1){//funzione numero 1: inviare le utlime 5 transazioni
                    //i = ultima transazione effettuata
                  
                    i=readfromFile();
                    FullRead(conn_fd,&iterazioni,sizeof(int));
                    printf("\ni: %d\n",i);
                    printf("\niterazioni: %d\n",iterazioni);
                    i=i-iterazioni;
                    if(i<0){
                        i=0;
                        printf("\nnon sono state ancora memorizzate %d iterazioni si provvede a mostrare le utlime iterazioni prodotte\n",iterazioni);
                    }
                    for(q=0;q<iterazioni;q++){
                        Transaction = Last5Transactions(i);
                        printf("\nporta mittente: %d\n",Transaction.mittente.sin_port);
                        printf("\nindirizzo ip mittente: %d\n",Transaction.mittente.sin_addr.s_addr);
                        printf("\nammontare: %d\n",Transaction.ammontare);
                        printf("\nporta destinatario: %d",Transaction.destinatario.sin_port);
                        printf("\nindirizzo ip destinatario: %d\n",Transaction.destinatario.sin_addr.s_addr);
                        FullWrite(conn_fd,&Transaction,sizeof(Transaction));
                        i++;
                    }                      
                }
                    if(sincro == 2){//funzione numero2: inviare una transazione specifica
                        //ricevo il numero progressivo che identifica la transazione
                        FullRead(conn_fd,&j,sizeof(int));
                        printf("\nnumero transazione: %d\n",j);
                        Transaction = Last5Transactions(j);
                    
                        printf("\nporta mittente: %d\n",Transaction.mittente.sin_port);
                        printf("\nindirizzo ip mittente: %d\n",Transaction.mittente.sin_addr.s_addr);
                        printf("\nammontare: %d\n",Transaction.ammontare);
                        printf("\nporta destinatario: %d",Transaction.destinatario.sin_port);
                        printf("\nindirizzo ip destinatario: %d\n",Transaction.destinatario.sin_addr.s_addr);
                        FullWrite(conn_fd,&Transaction,sizeof(Transaction));
                    }
                    if(sincro == 3){//funzione numero3: inviare il totale del valore di tutte le transazioni
                        h= TotalAmount();
                        printf("\ntotale: %d\n",h);
                        FullWrite(conn_fd,&h,sizeof(int));
                    }
                    if(sincro == 4){
                        FullRead(conn_fd,&ip,sizeof(int));
                        FullRead(conn_fd,&porta,sizeof(int));
                        h = specificAmount(porta,ip);
                        printf("\ntotale: %d\n",h);
                        FullWrite(conn_fd,&h,sizeof(int));
                    }
                }while(sincro!=0);
                printf("\nchiudo la connessione\n");
                close(conn_fd);
                exit(0);
            }else{//sono il padre
                close(conn_fd);
            }
          
        }
    }
}

void insertBlock(blocco blockToInsert)
{
   
    int file;
    //crea il file se non esiste
    file = open("blocchi.dat",O_APPEND);
    //possibili errori durante l'apertura del file
    if(file <0){
        perror("open file error");
        exit(-1);
    }
    //scrivo sul file

    write(file,&blockToInsert,sizeof(blocco));
   
    close(file);
}
int readfromFile()
{
 
    FILE *file_des;
    int file_des_2;
    blocco blockTemp;
    
    file_des = fopen("blocchi.dat","r+");
 
    int size,tot;
    int i=0;
    
    if(file_des == NULL){
        perror("open file error");
        exit(-1);
    }else{
        if (NULL != file_des) {
           
            fseek (file_des, 0, SEEK_END);
      
            size = ftell(file_des);
            
            fseek(file_des,0,SEEK_SET);
        
	    fclose(file_des);
            if (size >0) {
		file_des_2 = open("blocchi.dat",O_RDONLY);
                int tot = size;
                while(tot > 0){
                  
                    read(file_des_2,&blockTemp,sizeof(blocco));
                    tot = tot - sizeof(blocco);
                    i++;
                    if(feof(file_des)){
                        break;
                    }
                }
                close(file_des_2);
            }
            
            //fclose(file_des);
            return i;
        }
    }
    return 0;
}
transazione Last5Transactions(int indice)
{
   
    FILE *file_des;
    int file_des_2;
    blocco blockTemp;
 
    file_des = fopen("blocchi.dat","r+");
   
    int size,tot;
    int i=0;
    transazione transact;
    
   
    
    if(file_des == NULL){
        perror("open file error");
        exit(-1);
    }else{
        if (NULL != file_des) {
        
            fseek (file_des, 0, SEEK_END);
           
            size = ftell(file_des);
            
            fseek(file_des,0,SEEK_SET);
    
	    fclose(file_des);
            if (size >0) {
		file_des_2 = open("blocchi.dat",O_RDONLY);
       
                int tot = size;
                while(tot > 0){
               
                    read(file_des_2,&blockTemp,sizeof(blocco));
                 
                    tot = tot - sizeof(blocco);
                  
                    if(blockTemp.nBlocco == indice){
                        printf("\nnBlocco: %d\n",blockTemp.nBlocco);
                        printf("\nindice: %d\n",indice);
                        printf("\nmittente.porta: %d\n",blockTemp.transaction.mittente.sin_port);
                        
                        return blockTemp.transaction;
                    }
                    if(feof(file_des)){
                        break;
                    }
                }
                close(file_des_2);
            }
            
            //fclose(file_des);
        }
    }
}
int TotalAmount()
{
    FILE *file_des;
    int file_des_2;
    blocco blockTemp;
    file_des = fopen("blocchi.dat","r+");
    int size,tot;
    int i=0;
    
    if(file_des == NULL){
        perror("open file error");
        exit(-1);
    }else{
        if (NULL != file_des) {
            fseek (file_des, 0, SEEK_END);
            size = ftell(file_des);
            fseek(file_des,0,SEEK_SET);
	    fclose(file_des);
            if (size >0) {
		file_des_2 = open("blocchi.dat",O_RDONLY);
                int tot = size;
                while(tot > 0){
                    read(file_des_2,&blockTemp,sizeof(blocco));
                    tot = tot - sizeof(blocco);
                    i += blockTemp.transaction.ammontare;
                    if(feof(file_des)){
                        break;
                    }
                }
                close(file_des_2);
            }
            
            //fclose(file_des);
            return i;
        }
    }
    return 0;
}
int specificAmount(int porta, int indirizzoIp)
{
    FILE *file_des;
    int file_des_2;
    blocco blockTemp;
    file_des = fopen("blocchi.dat","r+");
    int size,tot;
    int i=0;
    
    if(file_des == NULL){
        perror("open file error");
        exit(-1);
    }else{
        if (NULL != file_des) {
            fseek (file_des, 0, SEEK_END);
            size = ftell(file_des);
            fseek(file_des,0,SEEK_SET);
	    fclose(file_des);
            if (size >0) {
		file_des_2 = open("blocchi.dat",O_RDONLY);
                int tot = size;
                while(tot > 0){
                    read(file_des_2,&blockTemp,sizeof(blocco));
                    tot = tot - sizeof(blocco);
                    if(porta == blockTemp.transaction.mittente.sin_port && indirizzoIp == blockTemp.transaction.mittente.sin_addr.s_addr){
                        printf("\nporta: %d\tportaB: %d\n",porta,blockTemp.transaction.mittente.sin_port);
                        printf("\nip: %d\tipB: %d\n",indirizzoIp,blockTemp.transaction.mittente.sin_addr.s_addr);
                        i += blockTemp.transaction.ammontare;
                    }
                    
                    if(feof(file_des)){
                        break;
                    }
                }
                close(file_des_2);
            }
            
            //fclose(file_des);
            return i;
        }
    }
    return 0;
}
