#include "./headers/UtilityFuncBlockServer.h"
//variabili server
int list_fd,conn_fd;
struct sockaddr_in servaddr;
int enable = 1;
struct hostent *rdata;
char buff_serv[1024];
//variabile client
 struct sockaddr_in serv_add;

int main(int argc,char *argv[])
{
    //variabili client
    int sock;
    char buff[MAXLINE];
    char *addr;
    char **alias;
    struct hostent *data;
    blocco b;
    
    //controllo inserimento ip e porta da riga di comando
    if(argc < 4){
        perror("usage: %s <indirizzo IP><PORTA connessione server><PORTA inizializzazione server>");
        exit(-1);
    }
    //inizio protocollo comunicazione client
    sock = Socket(AF_INET,SOCK_STREAM,0);
    serv_add.sin_family = AF_INET;
    serv_add.sin_port = htons(atoi(argv[2]));
    data = gethostbyname(argv[1]);
    
    if (data == NULL) {
        herror("Errore di risoluzione");
        exit(1);
    }
    alias = data->h_addr_list;
    addr = (char *)inet_ntop(data->h_addrtype, *alias, buff, sizeof(buff));
    if (inet_pton(AF_INET, addr, &serv_add.sin_addr) < 0) {
        fprintf(stderr,"inet_pton error for %s\n", argv[1]);
        exit (1);
    }
    //inizio protocollo comunicazione server
    list_fd = Socket(AF_INET,SOCK_STREAM,0);
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(atoi(argv[3]));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    setsockopt(list_fd,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int));
    setsockopt(list_fd,SOL_SOCKET,SO_REUSEPORT,&enable,sizeof(int));
    //associazione inrizzo server con socket
    Bind(list_fd,(struct sockaddr *) &servaddr,sizeof(servaddr));
    //mi metto in ascolto di connessioni creando la coda di richieste
    Listen(list_fd,1024);
    //fine protocollo comunicazione server
    //Fine protocollo comunicazione
    Connect(sock,(struct sockaddr *) &serv_add,sizeof(serv_add));
    //Fine protocollo comunicazione
    //Inizio funzione protocollo applicazione
    ProtocolloApplicazione(sock,b,list_fd);
    //fine protocollo applicazione e terminazione nomrale
    return 0;
}
