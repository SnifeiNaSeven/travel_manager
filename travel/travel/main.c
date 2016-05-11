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

void clean_str(char *str){
    memset (str,' ',strlen(str));
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

int menu_1(void) {
    FILE *viagens = fopen("viagens-datas.txt","r");
    FILE *cod_viagens = fopen("viagens-cod.txt","r");
    char line[1024];
    char aux[128];
    int i=0;
    int j;
    int option;
    Viagem new_node= malloc(sizeof(Viagem_node));

    printf("Adquirir uma viagem\n");

    while(fgets(line,1024,viagens)){
        if(*line!='-'&&*line!='\n') {
            ++i;
            printf("(%d)",i);
            j=0;
            while(!isalpha(*(line+j))){
                ++j;
            }
            while(*(line+j)) {
                printf("%c",*(line+j));
                ++j;
            }
        }
    }

    printf("\nInsira uma viagem: ");
    scanf("%d",&option);

    if(option>i || option< 1) {
        clrscr();
        printf("Viagem invalida!\n\n");
        menu_1();
    }

    j=1;
    clrscr();
    while(fgets(line,1024,cod_viagens)){
        if(j==option) {
            break;
        }else ++j;
    }

    j=0;
    while( *(line+j) ){
        if( *(line+j) == ' ') break;
        aux[j]=*(line+j);
        ++j;
    }

    new_node->cod_destino=atoi(aux);
    clrscr();

    data:
    i=0;
    fseek (viagens, 0L, SEEK_SET);
    printf("Data:\n");
    while(fgets(line,1024,viagens)){/*iterar linhas*/
        if(*line!='-'&&*line!='\n'){
            j=0;
            clean_str(aux);
            while(*(line+j)!=' '){
                aux[j]=*(line+j);
                ++j;
            }
            option = atoi(aux);/*codigo capturado*/
            if(new_node->cod_destino == option) {
                j=1;
                fgets(line,1024,viagens);
                while(*line != '\n'){/*Entrar seccao*/
                    ++i;
                    printf("%d)%c%c/%c%c/%c%c%c%c\n",i,line[1],line[2],line[3],line[4],line[5],line[6],line[7],line[8]);
                    fgets(line,1024,viagens);
                }
            }
        }
    }
    printf("\nOpcao: ");
    scanf("%d",&option);
    if(option>i || option< 1) {
        clrscr();
        printf("Data invalida!\n\n");
        goto data;
    }





    fclose(viagens);
    fclose(cod_viagens);
}

int main()
{
    /*Menu*/
    int option = menu_principal();
    switch(option) {
    /*(1)Adquirir uma viagem*/
    case 1:
        clrscr();
        int out = menu_1();

        break;
    /*(2)Colocar em fila de espera para uma viagem*/
    case 2: ;
        break;
    /*(3)Cancelar viagem*/



    /*(4)Cancelar pedido em fila de espera*/



    /*(5)Listar viagens de um destino*/



    /*(6)Listar viagens de um cliente*/



    /*(7)Listar Clientes*/

    }
    return 0;
}
