#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct viagem_node *Viagem;

typedef struct viagem_node {
    int data;
    int cod_destino;
    int cliente;
    Viagem next;
} Viagem_node;

void clrscr()
{
    system("@cls||clear");
}

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
    gets(op);
    c=atoi(op);
    if(c==0 || c>7 || c<1 ){
        clrscr();
        printf("Opcao Invalida!\n\n");
        menu_principal();
    }
    return c;
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

void printsectiondates(FILE *file,char *line,char *aux,int n){
    int i,code;
    fseek (file, 0L, SEEK_SET);
    while(fgets(line,1024,file)){
        code=getlinecode(line);
        if(code==n) break;
    }
    int j;
    i=0;
    while( fgets(line,1024,file) && *line=='-' ){
        cleanstr(aux);
        j=1;
        while( *(line+j)!='/' ) {
            aux[j-1]=*(line+j);
            ++j;
        }
        aux[j-1]='\0';
        ++i;
        printf("(%d)%s\n",i,aux);
    }
}

void print_list(Viagem lista){
    if( lista==NULL ){
        printf("NULL\n");
    }else{
        while(lista->next!=NULL){
            printf("cliente: %d\n",lista->cliente);
            printf("destino: %d\n",lista->cod_destino);
            printf("data: 0%d\n",lista->data);
            lista=lista->next;
        }
    }
    printf("\n");
}

Viagem criarlista(void){
    return NULL;
}

Viagem insert_last(Viagem lista,int data,int cod_destino,int cliente){
    Viagem new_node = (Viagem) malloc( sizeof(Viagem_node) );
    Viagem lista_orig = lista;
    new_node->cliente=cliente;
    new_node->cod_destino=cod_destino;
    new_node->data=data;
    if(lista==NULL){
        new_node->next=NULL;
        return new_node;
    } else {
        while(lista->next!=NULL){
            lista=lista->next;
        }
        lista->next=new_node;
        new_node->next=NULL;
        return lista_orig;
    }
}

int main()
{
    /*Menu*/
    int option = menu_principal();
    Viagem global_viagens = criarlista();
    FILE *viagens = fopen("viagens-datas.txt","r");
    char line[1024];
    clrscr();
    int i;
    int aux_int;
    int dest_code;
    int data;
    int cliente;

    while(fgets(line,1024,viagens)){
        /*entrar na secçao - codigo e destino*/
        if(isdigit(*line)){
            dest_code=getlinecode(line);
            /*Datas*/
            while(fgets(line,1024,viagens)){
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
                    while( *(line+i)!= '\n' && *(line+i) != '\0' ){
                        aux_int = aux_int * 10 + *(line+i) - '0';
                        ++i;
                    }
                    cliente = aux_int;
                    global_viagens = insert_last(global_viagens,data,dest_code,cliente);
                } else break;
            }
        }
    }

    print_list(global_viagens);

    fclose(viagens);

    switch(option) {
    /*(1)Adquirir uma viagem*/



    /*(2)Colocar em fila de espera para uma viagem*/



    /*(3)Cancelar viagem*/



    /*(4)Cancelar pedido em fila de espera*/



    /*(5)Listar viagens de um destino*/



    /*(6)Listar viagens de um cliente*/



    /*(7)Listar Clientes*/

    }
    return 0;
}
