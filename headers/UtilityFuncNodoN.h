#include <stdint.h>

void blockchainGeneration(Blockchain **b);
void push(Blockchain ** head_ref, blocco newBlockData);
void printList(Blockchain* b);
void deleteNode(Blockchain** head_ref,Blockchain* del);
void ProtocolloApplicazione(int socket,Blockchain* b);
blocco BlockOfBlockchain(Blockchain * b,int i);
void blockchainGeneration(Blockchain **b)
{
    int num=0;
    int nread;
    int size=-1;
    //variabili che accolgono un indirizzo ip generato casualmente
    uint32_t ul_dst;
    uint32_t random_num;
    
    blocco blockTemp;
    FILE *file_des;
    srand(time(NULL));
    file_des = fopen("blockchain.dat","r+");
    if(file_des == NULL){
        perror("open file error");
        exit(-1);
    }else{
        if (NULL != file_des) {
            fseek (file_des, 0, SEEK_END);
            size = ftell(file_des);
            fseek(file_des,0,SEEK_SET);
            if (size >0) {
                int tot = size;
                while(tot > 0){
                   fread(&blockTemp,sizeof(blocco),1,file_des);
                    tot = tot - sizeof(blocco);
                    num++;
                    push(b,blockTemp);
                    if(feof(file_des)){
                        break;
                    }
                }
                fclose(file_des);
    }
   
    fclose(file_des);
}
    }
    while(1){
            //generazione di un blocco ed inserimento nella lista
            blockTemp.attesaR = (rand()%6+10);
            blockTemp.nBlocco=++num;
            blockTemp.transaction.mittente.sin_family=AF_INET;
            blockTemp.transaction.mittente.sin_port=htons(rand()%1025+64506);
        
            //generatore di indirizzo ip casuale
            random_num = rand()%256+1024;
            ul_dst = (random_num >> 24 & 0xFF) << 24 |
            (random_num >> 16 & 0xFF) << 16 |
            (random_num >> 8 & 0xFF) << 8 |
            (random_num & 0xFF);
        
            blockTemp.transaction.mittente.sin_addr.s_addr = ul_dst;
            blockTemp.transaction.ammontare=rand()%110000;
            blockTemp.transaction.destinatario.sin_family=AF_INET;
            blockTemp.transaction.destinatario.sin_port=htons((rand()%65530)+1024);
        
            //generatore di indirizzo ip casuale
            random_num = rand()%256+1024;
            ul_dst = (random_num >> 24 & 0xFF) << 24 |
            (random_num >> 16 & 0xFF) << 16 |
            (random_num >> 8 & 0xFF) << 8 |
            (random_num & 0xFF);
            blockTemp.transaction.destinatario.sin_addr.s_addr= ul_dst;
        
            printf("\nattesaR: %d\n",blockTemp.attesaR);
            printf("\nnBlocco: %d\n",blockTemp.nBlocco);
            printf("\nporta mit: %d\n",blockTemp.transaction.mittente.sin_port);
            printf("\nip mit: %d\n",blockTemp.transaction.mittente.sin_addr.s_addr);
            printf("\nammontare: %d\n",blockTemp.transaction.ammontare);
            printf("\nporta dest: %d\n",blockTemp.transaction.destinatario.sin_port);
            printf("\nip dest: %d\n",blockTemp.transaction.destinatario.sin_addr.s_addr);
        
            sleep(blockTemp.attesaR);
            push(b,blockTemp);
            file_des=fopen("blockchain.dat","a");
            fwrite(&blockTemp,sizeof(blocco),1,file_des);
            fclose(file_des);
    }
}
void push(Blockchain ** head_ref, blocco newBlockData)
{
    //alloca il nodo*
    Blockchain* newBlock = (Blockchain *)malloc(sizeof(Blockchain));
    //inserisci il blocco dati
    newBlock->bloccoData = newBlockData;
    // rendi il nodo successivo la testa e il precedente a null
    newBlock->next = (*head_ref);
    newBlock->prev = NULL;
    
    // 4. change prev of head node to new node
    if ((*head_ref) != NULL)
        (*head_ref)->prev = newBlock;
    
    // 5. move the head to point to the new node
    (*head_ref) = newBlock;
}
void printList(Blockchain * b)
{
    Blockchain* last;
    printf("\nstampa blockchain\n");
    int time=0;
    while (b != NULL) {
//        printf("blocco: %d\t", b->bloccoData.nBlocco);
//        printf("ammontare: %d\n",b->bloccoData.transaction.ammontare);
        last = b;
        b = b->next;
    }
    printf("\nstampa inversa blockchain\n");
    while (last != NULL) {
        printf("blocco: %d\t", last->bloccoData.nBlocco);
        printf("ammontare: %d\n",last->bloccoData.transaction.ammontare);
        last = last->prev;
    }
    printf("\n");
}
void deleteAllOccurOfX(Blockchain** head_ref, int x)
{
    /* if list is empty */
    if ((*head_ref) == NULL)
        return;
    
    Blockchain* current = *head_ref;
    Blockchain* next;
    
    /* traverse the list up to the end */
    while (current != NULL) {
        
        /* if node found with the value 'x' */
        if (current->bloccoData.nBlocco == x) {
            
            /* save current's next node in the
             pointer 'next' */
            next = current->next;
            
            /* delete the node pointed to by
             'current' */
            deleteNode(head_ref, current);
            
            /* update current */
            current = next;
        }
        
        /* else simply move to the next node */
        else
            current = current->next;
    }
}
void deleteNode(Blockchain** head_ref,Blockchain* del)
{
    /* base case */
    if (*head_ref == NULL || del == NULL)
        return;
    
    /* If node to be deleted is head node */
    if (*head_ref == del)
        *head_ref = del->next;
    
    /* Change next only if node to be deleted
     is NOT the last node */
    if (del->next != NULL)
        del->next->prev = del->prev;
    
    /* Change prev only if node to be deleted
     is NOT the first node */
    if (del->prev != NULL)
        del->prev->next = del->next;
    
    /* Finally, free the memory occupied by del*/
    free(del);
}
void ProtocolloApplicazione(int socket,Blockchain* b){
    int maxd;
    fd_set set;
    int numeroBlocco;
    blocco tempBloc;
    while(1){
        FD_ZERO(&set);
        FD_SET(socket,&set);
        maxd = socket+1;
        if(select(maxd,&set,&set,NULL,NULL)<0){
            perror("errore sulla select");
            exit(-1);
        }
        //inizio sezione operazioni di lettura e scrittura sulla socket
        if(FD_ISSET(socket,&set)){
            //legge il numero di blocco dal blockServer
            FullRead(socket,&numeroBlocco,1);
            //cerca nella blockchain il blocco relativo
            tempBloc = BlockOfBlockchain(b,numeroBlocco);
            //invia il blocco, relativo al numero di blocco ricevuto, al blockServer
            FullWrite(socket,&tempBloc,sizeof(blocco));
        }
    }
    }
blocco BlockOfBlockchain(Blockchain * b,int i)
{
    Blockchain* last;
    int time=0;
    while (b != NULL) {
        last = b;
        b = b->next;
    }
    
    while (last != NULL) {
        if(last->bloccoData.nBlocco == i){
            return last->bloccoData;
        }
        last = last->prev;
    }
}
