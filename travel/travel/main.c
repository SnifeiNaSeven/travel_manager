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
    FILE *cod_viagens = fopen("viagens-cod.txt","r")
    char line[1024];
    int i=0;
    int j;
    int option;

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
    fclose(viagens);
    fclose(cod_viagens);
}

int main()
{
    /*Menu*/
    int option = menu_principal();

    /*(1)Adquirir uma viagem*/
    clrscr();
    int out = menu_1();


    /*(2)Colocar em fila de espera para uma viagem*/



    /*(3)Cancelar viagem*/



    /*(4)Cancelar pedido em fila de espera*/



    /*(5)Listar viagens de um destino*/



    /*(6)Listar viagens de um cliente*/



    /*(7)Listar Clientes*/




    return 0;
}
