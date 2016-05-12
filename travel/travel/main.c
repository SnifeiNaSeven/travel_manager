#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct viagem_node *Viagem;

typedef struct viagem_node {
    int data;
    int cod_destino;
    int cliente;
    Viagem prev;
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

    int option;

    printf("Menu\n");
    printf("(1)Adquirir uma viagem\n");
    printf("(2)Colocar em fila de espera para uma viagem\n");
    printf("(3)Cancelar viagem\n");
    printf("(4)Cancelar pedido em fila de espera\n");
    printf("(5)Listar viagens de um destino\n");
    printf("(6)Listar viagens de um cliente\n");
    printf("(7)Listar clientes\n\n");
    printf("Opcao: ");

    scanf("%d",&option);

    if(option>7 || option <1) {
        clrscr();
        printf("Opcao invalida!\n\n");
        menu_principal();
    }

    return option;
}

int getlinecode(char *line){
    int j;
    int n;
    char out[128];
    if( isdigit(*line) ){
        n=0;
        j=0;
        cleanstr(out);
        while( *(line+j)!=' ' ){
            out[0]=*(line+j);
            n=n*10+atoi(out);
            ++j;
        }
        return n;
    }
    return 0;
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

void printlinedates(FILE file,char *line,char *aux,int n){
    int i,code;
    fseek (viagens, 0L, SEEK_SET);
    while(fgets(line,1024,file)){
        code=getlinecode(line);
        if(code==n) break;
    }
    int j;
    fgets(line,1024,file);
    while( *line!="\n" && fgets(line,1024,file) ){
        cleanstr(aux);
        j=1;
        while( *(line+j)!='/' ) {
            aux[j-1]=*(line+j);
            ++j;
        }
        aux[j-1]='\0';
        printf(aux);
    }
}

int main()
{
    /*Menu*/
    int option = menu_principal();
    switch(option) {
    /*(1)Adquirir uma viagem*/
    case 1: ;
        FILE *viagens = fopen("viagens-datas.txt","r");
        FILE *codigos = fopen("viagens-cod.txt","r");
        clrscr();

        char line[1024];
        int out;
        char aux[128];
        int i,x1,x2;

         /*print codes
        clrscr();

        while(fgets(line,1024,viagens)){
            code=getlinecode(line);
            if(code!=0)printf("%d\n",code);
        }*/

        /*Print destinos*/
        clrscr();
        fseek (viagens, 0L, SEEK_SET);
        i=1;
        printf("Adquirir uma viagem\nSelecione o seu destino\n");
        while(fgets(line,1024,viagens)){
            cleanstr(aux);
            out=getlinename(line,aux);
            if(out!=0){
                printf("%d) %s\n",i,aux);
                ++i;
            }
        }

        printf("Opcao: ");
        scanf("%d",&x1);



        /*Print datas*/
        clrscr();
        i=1;
        printf("Adquirir uma viagem\nSelecione a data\n");
        printsectiondates(viagens,line,aux,x1);

        printf("Opcao: ");
        scanf("%d",&x2);

        fclose(viagens);
        fclose(codigos);
        break;

    /*(2)Colocar em fila de espera para uma viagem*/



    /*(3)Cancelar viagem*/



    /*(4)Cancelar pedido em fila de espera*/



    /*(5)Listar viagens de um destino*/



    /*(6)Listar viagens de um cliente*/



    /*(7)Listar Clientes*/

    }
    return 0;
}
