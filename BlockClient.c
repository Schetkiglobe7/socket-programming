#include "./headers/UtilityFuncBlockClient.h"
#define MAXLINE 250


int main(int argc,char *argv[])
{
    //variabili client
    int sock;
    char buff[MAXLINE];
    char *addr;
    char **alias;
    struct sockaddr_in serv_addr;
    struct hostent *data;
    
    //controllo inserimento ip e porta da riga di comando
    if(argc < 3){
        perror("usage: %s <indirizzo IP><PORTA connessione server>");
        exit(-1);
    }
    //inizio protocollo comunicazione client
    sock = Socket(AF_INET,SOCK_STREAM,0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    data = gethostbyname(argv[1]);
    
    if(data == NULL){
        herror("Errore di risoluzione");
        exit(-1);
    }
    alias = data->h_addr_list;
    addr = (char *)inet_ntop(data->h_addrtype, *alias, buff, sizeof(buff));
    if (inet_pton(AF_INET, addr, &serv_addr.sin_addr) < 0) {
        fprintf(stderr,"inet_pton error for %s\n", argv[1]);
        exit (1);
    }
    Connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr));
    //inizio protocollo applicazione
    ProtocolloApplicazione(sock);
    return 0;
}
