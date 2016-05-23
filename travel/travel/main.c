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

/*FIX: remover prev*/
/*FIX: remover lotacao*/
typedef struct clientes_node {
    int data;
    char destino[128];
    int cod_destino;
    int id;
    char nome;
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
Cliente insert_last_adq(int option,Cliente list,int data,char *destino,int cod_destino,int id);
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
void print_cliente_viagens(Cliente adq);
void remove_from_cliente(Cliente adq,int cod,int data,int cliente);
void print_cliente_adq(Cliente adq,Cliente esp, int cliente, Viagem lista);
void print_viagens_mais_recentes(Viagem global_viagens,int cod);
Viagem print_destino_from_cod(Viagem lista,int cod);

int main(){
    /*Import de ficheiros para listas*/
    /*Fix: Adicionar nomes de clientes*/
    FILE *viagens = fopen("viagens-datas.txt","r");
    Viagem global_viagens = (Viagem)criarlista();
    global_viagens = createGlobalListFromFile(viagens, global_viagens);
    fclose(viagens);

    /*Fix: criar ficheiro*/
    /*Fix: criar funcao ficheiro para lista*/
    /*Fix: Adicionar nomes de clientes*/
    /*FILE *adquiridas = fopen("viagens-adquiridas.txt","r");*/
    Cliente adq;
    adq = (Cliente)criarlista();
    /*createAdquiridaListFromFile(*adquiridas, adq);
    fclose(adquiridas);*/

    /*Fix: criar ficheiro*/
    /*Fix: criar funcao ficheiro para lista*/
    /*FILE *espera = fopen("viagens-espera.txt","r");*/
    Cliente esp;
    esp = (Cliente)criarlista();
    /*createAdquiridaListFromFile(*espera, esp);
    fclose(espera);*/

    /*Menu*/
    menu_principal(global_viagens,adq,esp);

    /*Fix: destruir listas*/
    /*Fix: escrever nosfiheiros*/

    return 0;
}

int choose_list_cliente_destinos(Cliente lista){
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
    lista=print_destino_from_cod(lista,x);
    printf("\n");
    printf("Datas:\n");
    Viagem lista_orig=lista;
    int datas_arr[128];
    int i=1;
    int in;
    int dia,ano,mes;
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
            data_to_dia_mes_ano(lista->data,&dia,&mes,&ano);
            printf("(%d)%d/%d/%d\n",i,dia,mes,ano);
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

Cliente insert_last_adq(int option,Cliente list,int data,char *destino,int cod_destino,int id){
    /*Fix: Se nao houver vagas por em adq lista de espera*/
    /*FIX: option: 0->adq 1->esp*/
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

void data_to_dia_mes_ano(int data, int *dia, int *mes, int *ano){
    *ano = data % 10000;
    data /= 10000;
    *mes = data % 100;
    data /= 100;
    *dia = data % 100;
}

int data_invert(int data){
    int dia,mes,ano;
    data_to_dia_mes_ano(data,&dia,&mes,&ano);
    data=ano;
    data=data*100+mes;
    return data*100+dia;
}

int data_fix(int data){
    int dia,mes,ano;
    dia=data%100;
    data/=100;
    mes=data%100;
    data/=100;
    ano=data;
    data=dia*100;
    data=(data+mes)*10000;
    return data+=ano;
}

Viagem print_destino_from_cod(Viagem lista,int cod){
    Viagem org=lista;
    while( lista->cod_destino!=cod ){
        lista=lista->next;
    }
    printf("%s",lista->destino);
    return org;
}

void bubblesort_inv(int vect[], int n){
    int temp, j, pass; int trocou = 1;
    for (pass=0; pass<n-1 && trocou; pass++) {
        trocou = 0; for (j = 0; j < n-pass-1; j++)
        if (vect[j] < vect[j+1]){
            trocou = 1;
        temp = vect[j];
        vect[j] = vect[j+1]; vect[j+1] = temp;
        }
    }
}

void bubblesort_viagens(int vect[],int *codes, int n){
    int temp, temp2, j, pass; int trocou = 1;
    for (pass=0; pass<n-1 && trocou; pass++) {
        trocou = 0;
        for (j = 0; j < n-pass-1; j++){
            if (vect[j] > vect[j+1]){
                trocou = 1;
                temp = vect[j];
                vect[j] = vect[j+1];
                vect[j+1] = temp;
                temp2 = codes[j];
                codes[j] = codes[j+1];
                codes[j+1] = temp2;
            }
        }
    }
}

void print_viagens_mais_recentes(Viagem lista, int cod){
    int datas_arr[128];
    int data,dia,mes,ano;
    int i=0;
    int j=0;
    lista=print_destino_from_cod(lista,cod);
    while( lista!=NULL ){
        if( lista->cod_destino==cod ){
            data=data_invert(lista->data);
            datas_arr[i]=data;
            ++i;
        }
        lista=lista->next;
    }
    bubblesort_inv(datas_arr,i);
    while( j<i ){
        data=data_fix(datas_arr[j]);
        data_to_dia_mes_ano(data,&dia,&mes,&ano);
        printf("- %d/%d/%d\n",dia,mes,ano);
        ++j;
    }
}

void print_viagens_cliente_mais_recentes(Viagem lista, int cliente){
    int datas_arr[128];
    int data,dia,mes,ano;
    int i=0;
    int j=0;
    lista=print_destino_from_cod(lista,cliente);
    while( lista!=NULL ){
        if( lista->cod_destino==cliente ){
            data=data_invert(lista->data);
            datas_arr[i]=data;
            ++i;
        }
        lista=lista->next;
    }
    bubblesort_inv(datas_arr,i);
    while( j<i ){
        data=data_fix(datas_arr[j]);
        data_to_dia_mes_ano(data,&dia,&mes,&ano);
        printf("- %d/%d/%d\n",dia,mes,ano);
        ++j;
    }
}

void print_cliente_adq(Cliente adq,Cliente esp, int cliente, Viagem lista){
    int dia,mes,ano,data;
    int codes_1[128];
    int adq_arr[128];
    int i1=0;
    int codes_2[128];
    int esp_arr[128];
    int i2=0;
    int j=0;
    printf("Viagens e datas respetivas\n");
    if(adq!=NULL) printf("\nAdquiridas:\n");
    while( adq!=NULL ){
        if( adq->id == cliente ){
            adq_arr[i1]=data_invert(adq->data);
            codes_1[i1]=adq->cod_destino;
            ++i1;
        }
        adq=adq->next;
    }
    bubblesort_viagens(adq_arr,codes_1,i1);
    while( j<i1 ){
        data=adq_arr[j];
        data=data_fix(data);
        data_to_dia_mes_ano(data,&dia,&mes,&ano);
        printf("-> ");
        print_destino_from_cod(lista,codes_1[j]);
        printf(" / %d-%d-%d\n",dia,mes,ano);
        ++j;
    }

    if(esp!=NULL) printf("\nViagens em lista de espera:\n");
    j=0;
    while( esp!=NULL ){
        if( esp->id == cliente ){
            esp_arr[i2]=data_invert(esp->data);
            codes_2[i2]=esp->cod_destino;
            ++i2;
        }
        esp=esp->next;
    }
    bubblesort_viagens(esp_arr,codes_2,i2);
    while( j<i2 ){
        data=esp_arr[j];
        data=data_fix(data);
        data_to_dia_mes_ano(data,&dia,&mes,&ano);
        printf("-> ");
        print_destino_from_cod(lista,codes_2[j]);
        printf(" / %d-%d-%d\n",dia,mes,ano);
        ++j;
    }
    getchar();
}

int menu_principal(Viagem global_viagens,Cliente adq,Cliente esp) {
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
            /*FIX: Proteger input*/
            clrscr();
            printf("Aquirir Viagem\n");
            cleanstr(destino);
            cod=choose_list_viagem_destinos(global_viagens,destino);
            clrscr();
            printf("Aquirir Viagem\n");
            data=choose_list_viagem_data(global_viagens,cod);
            clrscr();
            printf("Aquirir Viagem\n");
            printf("Numero cartao do cidadao: ");
            scanf("%d",&id);
            adq = insert_last_adq(0,adq,data,destino,cod,id);
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
            printf("Numero cartao do cidadao: ");
            scanf("%d",&id);
            esp = insert_last_adq(1,esp,data,destino,cod,id);
            getchar();
            break;
        /*(3)Cancelar viagem*/
        case 3:
            break;
        /*(4)Cancelar pedido em fila de espera*/
        case 4:
            break;
        /*(5)Listar viagens de um destino*/
        case 5:
            clrscr();
            printf("Listar viagens de um destino\n");
            cleanstr(destino);
            cod=choose_list_viagem_destinos(global_viagens,destino);
            clrscr();
            printf("Listar viagens de um destino\n");
            print_viagens_mais_recentes(global_viagens,cod);
            getchar();
            break;
        /*(6)Listar viagens de um cliente*/
        case 6:
            /*Fix: proteger input*/
            /*Fix: Imprimir nome de cliente*/
            clrscr();
            printf("Listar viagens de cliente\n");
            printf("Numero cartao do cidadao: ");
            scanf("%d",&id);
            clrscr();
            print_cliente_adq(adq,esp,id,global_viagens);
            getchar();
            break;
        /*(7)Listar clientes*/
        case 7:
            /*Fix: Adicionar nomes de clientes*/
            clrscr();
            printf("Lista de clientes\n");
            print_clientes(adq,esp);
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
