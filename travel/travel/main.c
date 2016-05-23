#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*http://www.dreamincode.net/forums/topic/90898-cross-platform-clear-screen/*/
#ifndef WIN32
#define COMMAND "cls" /*Clears a windows console screen*/
#else
#define COMMAND "clear" /*Clears a linux console screen*/
#endif
#define clrscr() system( COMMAND )

typedef struct viagem_node *Viagem;

typedef struct viagem_node {
    int data;
    int cod_destino;
    int lotacao;
    int disponiveis;
    char destino[128];
    Viagem next;
    Viagem prev;
} Viagem_node;

typedef struct clientes_node *Cliente;

typedef struct clientes_node {
    int data;
    char destino[128];
    int cod_destino;
    int id;
    Cliente next;
    Viagem prev;
} Clientes_node;

void cleanstr(char *out){
    strcpy(out,"");
}

Viagem criarlista(void){
    return NULL;
}

Viagem createGlobalListFromFile(FILE *file, Viagem lista);
Cliente createAdquiridaListFromFile(FILE *file, Cliente lista);/*Fix: por fazer*/
Cliente createEsperaListFromFile(FILE *file, Cliente lista);/*Fix: por fazer*/
Viagem insert_last_viagem(Viagem lista,int data,int cod_destino,char *destino,int lotacao,int disponiveis);
Cliente insert_last_adq(Cliente list,int data,char *destino,int cod_destino,int id);
int choose_list_viagem_destinos(Viagem lista,char *destino);
int choose_list_viagem_data(Viagem lista,int x);
int choose_list_cliente_destinos(Cliente lista);
void destroi_lista_viagem(Viagem lista);
void destroi_lista_cliente(Cliente lista);
void print_list_cliente(Cliente lista);
void print_list_viagem(Viagem lista);
int menu_principal(Viagem global_viagens,Cliente adq,Cliente esp);
int getlinecode(char *line);
int getlinename(char *line,char *aux);
void print_cliente_viagens(Cliente adq);/*Fix: por fazer*/
void remove_from_cliente(Cliente adq,int cod,int data,int cliente);/*Fix: por fazer*/
void print_cliente_adq(Cliente adq,int cliente);

int main(){
    /*Import de ficheiros para listas*/
    FILE *viagens = fopen("viagens-datas.txt","r");
    Viagem global_viagens = (Viagem)criarlista();
    global_viagens = createGlobalListFromFile(viagens, global_viagens);
    fclose(viagens);

    /*FILE *adquiridas = fopen("viagens-adquiridas.txt","r");*/
    Cliente adq;
    adq = (Cliente)criarlista();
    /*createAdquiridaListFromFile(*adquiridas, adq);
    fclose(adquiridas);*/

    /*FILE *espera = fopen("viagens-espera.txt","r");*/
    Cliente esp;
    esp = (Cliente)criarlista();
    /*createAdquiridaListFromFile(*espera, esp);
    fclose(espera);*/

    /*Menu*/
    menu_principal(global_viagens,adq,esp);

    /*destroi_lista_viagem(global_viagens);
    print_list_viagem(global_viagens);*/

    return 0;
}

void print_cliente_adq(Cliente adq, int cliente){
    int dia,mes,ano,data;
    printf("Viagens and date\n");
    while( adq!=NULL ){
        if( adq->id == cliente ){
            data=adq->data;
            ano = data % 10000;
            data = (data - ano) / 10000;
            mes = data % 100;
            data = (data - mes) / 100;
            dia = data % 100;
            printf("-> %s / data: %d-%d-%d\n",adq->destino,dia,mes,ano);
        }
        adq=adq->next;
    }
    getchar();
}

int choose_list_cliente_destinos(Cliente lista){
    /*Ong goin*/
    printf("Viagens:\n");
    Cliente lista_orig=lista;
    int cliente_arr[128];
    int i=1;
    int in;
    char input[128];
    char prev_char[128];
    if( lista==NULL ){
        printf("NULL\n");
    }else{
        while(lista!=NULL){
            if( strcmp(prev_char,lista->destino)==0 ){
                strcpy(prev_char,lista->destino);
            }else{
                printf("(%d)%s\n",i,lista->destino);
                cliente_arr[i-1]=lista->cod_destino;
                ++i;
                strcpy(prev_char,lista->destino);
            }
            lista=lista->next;
        }
    }
    printf("\n");
    printf("Opcao: ");
    fgets (input, 128, stdin);
    in=atoi(input);
    if( isdigit(*input) && strlen(input)==2 && in<i && in>0 ){
        return cliente_arr[in-1];
    } else {
        clrscr();
        printf("Opcao Invalida!\n\n");
        choose_list_cliente_destinos(lista_orig);
    }
}

int choose_list_viagem_data(Viagem lista,int x){
    printf("Datas:\n");
    Viagem lista_orig=lista;
    int datas_arr[128];
    int i=1;
    int in;
    int dia,ano,mes,data;
    char input[128];
    int prev_int;

        while(lista!=NULL){
            if( lista->cod_destino == x ){
                break;
            }
            lista=lista->next;
        }
        prev_int=lista->cod_destino;
        while( lista != NULL && lista->cod_destino == prev_int ){
            datas_arr[i-1]=lista->data;
            data = lista->data;
            ano = data % 10000;
            data = (data - ano) / 10000;
            mes = data % 100;
            data = (data - mes) / 100;
            dia = data % 100;
            printf("(%d)%d-%d-%d\n",i,dia,mes,ano);
            ++i;
            lista=lista->next;
        }

    printf("\n");
    printf("Opcao: ");
    fgets (input, 128, stdin);
    in=atoi(input);
    if( isdigit(*input) && strlen(input)==2 && in<i && in>0 ){
        return datas_arr[in-1];
    } else {
        clrscr();
        printf("Opcao Invalida!\n\n");
        choose_list_viagem_data(lista_orig,x);
    }
}

int choose_list_viagem_destinos(Viagem lista,char *destino){
    printf("Viagens:\n");
    Viagem lista_orig=lista;
    int viagens_arr[128];
    char *dest_arr[128];
    int i=1;
    int in;
    char input[128];
    char prev_char[128];
    if( lista==NULL ){
        printf("NULL\n");
    }else{
        while(lista!=NULL){
            if( strcmp(prev_char,lista->destino)==0 ){
                strcpy(prev_char,lista->destino);
            }else{
                printf("(%d)%s\n",i,lista->destino);
                viagens_arr[i-1]=lista->cod_destino;
                dest_arr[i-1]=lista->destino;
                ++i;
                strcpy(prev_char,lista->destino);
            }
            lista=lista->next;
        }
    }
    printf("\n");
    printf("Opcao: ");
    fgets (input, 128, stdin);
    in=atoi(input);
    if( isdigit(*input) && strlen(input)==2 && in<i && in>0 ){
        strcpy(destino,dest_arr[in-1]);
        return viagens_arr[in-1];
    } else {
        clrscr();
        printf("Opcao Invalida!\n\n");
        choose_list_viagem_destinos(lista_orig,destino);
    }
}

Viagem createGlobalListFromFile(FILE *file, Viagem lista){
    char line[1024];
    int dest_code;
    char destino[128];
    int i;
    int aux_int;
    int data;
    int lotacao;
    int disponiveis;
    while(fgets(line,1024,file)){
        /*entrar na seccao - codigo e destino*/
        if(isdigit(*line)){
            cleanstr(destino);
            dest_code=getlinecode(line);
            getlinename(line,destino);
            /*Datas*/
            while(fgets(line,1024,file)){
                i=0;
                if(*line=='-'){
                    aux_int=0;
                    while( *(line+i)!= ' ' ){
                        if( *(line+i) != '-' ){
                            aux_int = aux_int * 10 + *(line+i) - '0';
                        }
                        ++i;
                    }
                    data=aux_int;
                    while( !isdigit( *(line+i) ) ){
                        ++i;
                    }

                    aux_int=0;
                    while( *(line+i)!= ' ' ){
                        aux_int = aux_int * 10 + *(line+i) - '0';
                        ++i;
                    }
                    lotacao=aux_int;
                    while( !isdigit( *(line+i) ) ){
                        ++i;
                    }

                    aux_int=0;
                    while( *(line+i)!= '\n' && *(line+i) != '\0' ){
                        aux_int = aux_int * 10 + *(line+i) - '0';
                        ++i;
                    }
                    disponiveis=aux_int;
                    lista = insert_last_viagem(lista,data,dest_code,destino,lotacao,disponiveis);

                } else break;
            }
        }
    }
    return lista;
}

Viagem insert_last_viagem(Viagem lista,int data,int cod_destino,char *destino,int lotacao,int disponiveis){
    Viagem new_node = (Viagem) malloc( sizeof(Viagem_node) );
    Viagem lista_orig = lista;
    new_node->lotacao=lotacao;
    new_node->disponiveis=disponiveis;
    new_node->cod_destino=cod_destino;
    new_node->data=data;
    strcpy(new_node->destino,destino);
    if(lista==NULL){
        new_node->next=NULL;
        new_node->prev=NULL;
        return new_node;
    } else {
        while(lista->next!=NULL){
            lista=lista->next;
        }
        lista->next=new_node;
        new_node->prev=lista;
        new_node->next=NULL;
        return lista_orig;
    }
}

Cliente insert_last_adq(Cliente list,int data,char *destino,int cod_destino,int id){
    /*Fix: Se nao houver vagas por en lista de espera*/
    Cliente new_node = (Cliente) malloc( sizeof(Clientes_node) );
    Cliente lista_orig = list;
    new_node->data=data;
    new_node->cod_destino=cod_destino;
    new_node->id=id;
    strcpy(new_node->destino,destino);
    if(list==NULL){
        new_node->next=NULL;
        new_node->prev=NULL;
        /*Fix: diminuir lugar disponiveis + procurar na lista de viagens destino->data*/
        return new_node;
    } else {
        while(list->next!=NULL){
            list=list->next;
        }
        new_node->prev=list;
        list->next=new_node;
        new_node->next=NULL;
        /*Fix: diminuir lugar disponiveis + procurar na lista de viagens destino->data*/
        return lista_orig;
    }
}

int getlinename(char *line,char *aux){
    int j,i;
    if( isdigit(*line) ){
        j=0;
        i=0;
        cleanstr(aux);
        while( *(line+j)!='\n' ){
            if( isalpha( *(line+j) ) ){
                aux[i]= *(line+j);
                ++i;
            }
            ++j;
        }
        aux[i]='\0';
        return 1;
    }
    return 0;
}

int getlinecode(char *line){
    int j;
    int n;
    n=0;
    j=0;
    while( *(line+j)!=' ' ){
        n=n*10 + *(line+j) - '0';
        ++j;
    }
    return n;
}

void print_list_viagem(Viagem lista){
    if( lista==NULL ){
        printf("NULL\n");
    }else{
        while(lista!=NULL){
            printf("destino: %d %s\n",lista->cod_destino,lista->destino);
            printf("Lotacao: %d\n",lista->lotacao);
            printf("Disponiveis: %d\n",lista->disponiveis);
            printf("data: %d\n",lista->data);
            printf("\n");
            lista=lista->next;
        }
    }
    printf("\n");
    getchar();
}

void print_list_cliente(Cliente lista){
    if( lista==NULL ){
        printf("NULL\n");
    }else{
        while(lista!=NULL){
            printf("cliente: %d\n",lista->id);
            printf("destino: %s\n",lista->destino);
            printf("code: %d\n",lista->cod_destino);
            printf("data: %d\n",lista->data);
            printf("\n");
            lista=lista->next;
        }
    }
    printf("\n");
    getchar();
}

void destroi_lista_viagem(Viagem lista){
    Viagem next;
    while(lista!=NULL){
        next=lista->next;
        free(lista);
        lista=next;
    }
}

int in_arr_int(int *arr,int i,int size){
    int x=0;
    while(x<size){
        if( arr[x]==i ) return 1;
        ++x;
    }
    return 0;
}

void print_clientes(Cliente adq,Cliente esp){
    int int_arr[128];
    int j=0;
    int i=0;
    int id;
    while( adq!=NULL ){
        id=adq->id;
        if( in_arr_int(int_arr,id,i)==0 ){
            int_arr[i]=id;
            ++i;
        }
        adq=adq->next;
    }
    while( j<i ){
        printf("-> %d\n",int_arr[j]);
        ++j;
    }
}

int menu_principal(Viagem global_viagens,Cliente adq,Cliente esp) {
    /*Fix: Adicionar nomes de clientes*/
    /*Fix: mudar texto de saida - qualquer tecla para voltar*/
    /*
    Menu
    (1)Adquirir uma viagem
    (2)Colocar em fila de espera para uma viagem
    (3)Cancelar viagem
    (4)Cancelar pedido em fila de espera
    (5)Listar viagens de um destino
    (6)Listar viagens de um cliente
    (7)Listar Clientes
    */

    char op[128];
    int option,cod,data,id;
    char destino[128];

    printf("Menu\n");
    printf("(1)Adquirir uma viagem\n");
    printf("(2)Colocar em fila de espera para uma viagem\n");
    printf("(3)Cancelar viagem\n");
    printf("(4)Cancelar pedido em fila de espera\n");
    printf("(5)Listar viagens de um destino\n");
    printf("(6)Listar viagens de um cliente\n");
    printf("(7)Listar clientes\n");
    printf("(8)Sair\n\n");
    printf("Opcao: ");

    cleanstr(op);
    fgets (op, 128, stdin);
    option=atoi(op);
    if( isdigit(*op) && strlen(op)==2 && option<9 && option>0 ){
    } else {
        clrscr();
        printf("Opcao Invalida!\n\n");
        return menu_principal(global_viagens,adq,esp);
    }

    switch(option) {
    /*(1)Adquirir uma viagem*/
        case 1:
            clrscr();
            printf("Aquirir Viagem\n");
            cleanstr(destino);
            cod=choose_list_viagem_destinos(global_viagens,destino);
            clrscr();
            printf("Aquirir Viagem\n");
            data=choose_list_viagem_data(global_viagens,cod);
            clrscr();
            printf("Aquirir Viagem\n");
            printf("Numero de cliente: ");
            scanf("%d",&id);
            adq = insert_last_adq(adq,data,destino,cod,id);
            getchar();
            break;
    /*(2)Colocar em fila de espera para uma viagem*/
        case 2:
            clrscr();
            printf("Aquirir Viagem\n");
            cod=choose_list_viagem_destinos(global_viagens,destino);
            clrscr();
            printf("Aquirir Viagem\n");
            data=choose_list_viagem_data(global_viagens,cod);
            clrscr();
            printf("Aquirir Viagem\n");
            printf("Numero de cliente: ");
            scanf("%d",&id);
            esp = insert_last_adq(esp,data,destino,cod,id);
            getchar();
            break;
        /*(3)Cancelar viagem*/
        case 3:
            break;
        /*(4)Cancelar pedido em fila de espera*/
        case 4:;
        /*(5)Listar viagens de um destino*/
        case 5:
            clrscr();
            printf("Listar viagens de um destino\n");
            cleanstr(destino);
            cod=choose_list_viagem_destinos(global_viagens,destino);
            clrscr();
            printf("Listar viagens de um destino\n");
            choose_list_viagem_data(global_viagens,cod);/*Fix: Modificar para nao ter opçao*/
        /*(6)Listar viagens de um cliente*/
        case 6:
            clrscr();
            printf("Listar viagens de cliente\n");
            printf("cliente: ");
            scanf("%d",&id);/*Fix: proteger input*/
            clrscr();
            print_cliente_adq(adq,id);
            getchar();
            break;
        /*(7)Listar clientes*/
        case 7:
            clrscr();
            printf("Lista de clientes\n");
            print_clientes(adq,esp);/*Por fazer*/
            getchar();
        case 8:
            break;
    }
    clrscr();
    cleanstr(op);
    printf("Deseja voltar?\n(S)para voltar ou qualquer outra tecla para sair\n");
    printf("Opcao: ");
    fgets (op, 128, stdin);
    if( strlen(op)==2 && (*op=='S' || *op=='s') ){
        clrscr();
        menu_principal(global_viagens,adq,esp);
    }else return 0;
}
