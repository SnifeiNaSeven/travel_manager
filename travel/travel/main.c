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
    int cod_destino;
    int id;
    Cliente next;
    Viagem prev;
} Clientes_node;

void cleanstr(char *out){
    strcpy(out,"");
}

int menu_principal(void) {
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
    int c;

    printf("Menu\n");
    printf("(1)Adquirir uma viagem\n");
    printf("(2)Colocar em fila de espera para uma viagem\n");
    printf("(3)Cancelar viagem\n");
    printf("(4)Cancelar pedido em fila de espera\n");
    printf("(5)Listar viagens de um destino\n");
    printf("(6)Listar viagens de um cliente\n");
    printf("(7)Listar clientes\n\n");
    printf("Opcao: ");

    cleanstr(op);
    fgets (op, 128, stdin);
    c=atoi(op);
    if( isdigit(*op) && strlen(op)==2 && c<8 && c>0 ){
        return c;
    } else {
        clrscr();
        printf("Opcao Invalida!\n\n");
        menu_principal();
    }

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

void print_list_viagem(Viagem lista){
    if( lista==NULL ){
        printf("NULL\n");
    }else{
        while(lista!=NULL){
            printf("destino: %d %s\n",lista->cod_destino,lista->destino);
            printf("Lotacao: %d\n",lista->lotacao);
            printf("Disponiveis: %d\n",lista->disponiveis);
            printf("data: 0%d\n",lista->data);
            printf("\n");
            lista=lista->next;
        }
    }
    printf("\n");
}

Viagem criarlista(void){
    return NULL;
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

Cliente insert_last_adq(Cliente list,int data,int cod_destino,int id){
    Cliente new_node = (Cliente) malloc( sizeof(Clientes_node) );
    Cliente lista_orig = list;
    new_node->data=data;
    new_node->cod_destino=cod_destino;
    new_node->id=id;
    if(list==NULL){
        new_node->next=NULL;
        new_node->prev=NULL;
        /*diminuir lugar disponiveis + procurar na lista de viagens destino->data*/
        return new_node;
    } else {
        while(list->next!=NULL){
            list=list->next;
        }
        new_node->prev=list;
        list->next=new_node;
        new_node->next=NULL;
        /*diminuir lugar disponiveis + procurar na lista de viagens destino->data*/
        return lista_orig;
    }
}

Viagem createGlobalListFromFile(FILE *file, Viagem lista){
    char line[1024];
    int dest_code;
    char destino[128];
    int i;
    int aux_int;
	char aux_char[128];
    int data;
    int lotacao;
    int disponiveis;
    Viagem lista_orig = lista;
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

Viagem destroilista_viagem(Viagem lista){
    Viagem next;
    while(lista!=NULL){
        next=lista->next;
        free(lista);
        lista=next;
    }
    return NULL;
}

int choose_list_viagem_destinos(Viagem lista){
    printf("Viagens:\n");
    Viagem lista_orig=lista;
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
                ++i;
                strcpy(prev_char,lista->destino);
            }
            lista=lista->next;
        }
    }
    printf("\n");
    printf("Option: ");
    fgets (input, 128, stdin);
    in=atoi(input);
    if( isdigit(*input) && strlen(input)==2 && in<i-1 && in>0 ){
        return in;
    } else {
        clrscr();
        printf("Opcao Invalida!\n\n");
        choose_list_viagem_destinos(lista_orig);
    }
    return in;
}

int main()
{
    /*Menu*/
    int option = menu_principal();
    FILE *viagens = fopen("viagens-datas.txt","r");

    /*Import dos dados*/
    Viagem global_viagens = criarlista();

    global_viagens = createGlobalListFromFile(viagens, global_viagens);
    fclose(viagens);

    /*Criar lista de viagens adquiridas*/
    Cliente adq;
    adq = criarlista();
    /*Import de lista de viagens adquiridas*/

    switch(option) {
    /*(1)Adquirir uma viagem*/
        case 1:
            clrscr();
            printf("Aquirir Viagem\n");
            option=choose_list_viagem_destinos(global_viagens);
            break;
    /*(2)Colocar em fila de espera para uma viagem*/



    /*(3)Cancelar viagem*/



    /*(4)Cancelar pedido em fila de espera*/



    /*(5)Listar viagens de um destino*/



    /*(6)Listar viagens de um cliente*/



    /*(7)Listar Clientes*/

    }

    global_viagens = destroilista_viagem(global_viagens);
    print_list_viagem(global_viagens);

    return 0;
}
