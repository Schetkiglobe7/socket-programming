#include "./headers/Wrapper.h"
#include "./headers/UtilityTypes.h"
#include "./headers/UtilityFuncNodoN.h"
#include<pthread.h>
#define MAXLINE 1024
//funzione generatrice dei blocchi per la blockchain
    
void *fun(void *args);
 Blockchain *b;
int main(int argc,char *argv[])
{
    int status;
    pthread_t tread;
    //variabili server
    int list_fd,conn_fd;
    struct sockaddr_in serv_addr,client_addr;
    char bufferIp[MAXLINE];
    socklen_t len;
    pid_t pid;
    int enable =1;
    struct hostent *rdata;
    //controllo sugli argomenti, è necessario passare l'ip e la porta della macchina che si vuole raggiungere
    b =(Blockchain *)malloc(sizeof(Blockchain ));
    pthread_create(&tread,NULL,fun,NULL);
   //mentre il thread crea i blocchi della blockchain, il processo è occupato a comunicare questi blocchi al blockServer
    //gestione connessioni
    //inizio protocollo comunicazione
    list_fd = Socket(AF_INET,SOCK_STREAM,0);
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port = htons(1025);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    setsockopt(list_fd,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int));
    Bind(list_fd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));
    Listen(list_fd,1024);
     //Inizio funzione protocollo applicazione
    while(1)
    {
        len = sizeof(client_addr);
        conn_fd = Accept(list_fd,(struct sockaddr *)&client_addr,&len);
        inet_ntop(AF_INET,&client_addr.sin_addr,bufferIp,sizeof(bufferIp));
        printf("\nrequest from:\nIP:%s\tPORT:%d\n",bufferIp,client_addr.sin_port);
        pid = fork();
        if(pid == 0){//sei il figlio
            close(list_fd);
            ProtocolloApplicazione(conn_fd,b);
            //quando esci dal protocollo applicazione, si chiude la connessione
            printf("\nchiudo connessione\n");
            close(conn_fd);
            exit(0);
        }else{//sei il padre, ho il compito di tornare indietro nel while ed accettare altre connessioni
            close(conn_fd);
            
        }
    }
    //il server nodoN termina appena esce dal while
    return 0;
}
void *fun(void *args)
{
     blockchainGeneration(&b);
}

