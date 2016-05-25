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
    int disponiveis;
    char destino[128];
    Viagem next;
} Viagem_node;

typedef struct clientes_node *pCliente;

typedef struct clientes_node {
    int data;
    char destino[128];
    int cod_destino;
    int id;
    char nome[128];
    pCliente next;
} Clientes_node;

void cleanstr(char *out){
    strcpy(out,"");
}

Viagem criarlista(void){
    return NULL;
}

Viagem createGlobalListFromFile(FILE *file, Viagem lista);
Viagem insert_last_viagem(Viagem lista,int data,int cod_destino,char *destino,int disponiveis);

int main(){
    /*Import de ficheiros para listas*/

    FILE *viagens = fopen("viagens-datas.txt","r");
    Viagem global_viagens = (Viagem)criarlista();
    global_viagens = createGlobalListFromFile(viagens, global_viagens);
    fclose(viagens);

    /*Fix: criar funcao ficheiro para lista*/
    /*Fix: Adicionar nomes de clientes*/
    /*FILE *adquiridas = fopen("viagens-adquiridas.txt","r");*/
    pCliente adq=criarlista();
    /*createAdquiridaListFromFile(*adquiridas, adq);
    fclose(adquiridas);*/

    /*Fix: Adicionar nomes de clientes*/
    /*Fix: criar funcao ficheiro para lista*/
    /*FILE *espera = fopen("viagens-espera.txt","r");*/
    pCliente esp=criarlista();
    /*createAdquiridaListFromFile(*espera, esp);
    fclose(espera);*/

    /*Menu*/
    menu_principal(global_viagens,adq,esp);

    /*Fix: destruir listas*/
    /*Fix: escrever nosfiheiros*/

    return 0;
}

int choose_list_cliente_destinos(pCliente lista){
    printf("Viagens:\n");
    pCliente lista_orig=lista;
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
    char destino[128];
    get_destino_from_cod(lista,x,destino);
    printf("%s\n",destino);
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
    int disponiveis;
    while(fgets(line,1024,file)){
        if(isdigit(*line)){
            cleanstr(destino);
            dest_code=getlinecode(line);
            getlinename(line,destino);
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
                    while( *(line+i)!= '\n' && *(line+i) != '\0' ){
                        aux_int = aux_int * 10 + *(line+i) - '0';
                        ++i;
                    }
                    disponiveis=aux_int;
                    lista = insert_last_viagem(lista,data,dest_code,destino,disponiveis);

                } else break;
            }
        }
    }
    return lista;
}

Viagem insert_last_viagem(Viagem lista,int data,int cod_destino,char *destino,int disponiveis){
    Viagem new_node = (Viagem) malloc( sizeof(Viagem_node) );
    Viagem lista_orig = lista;
    new_node->disponiveis=disponiveis;
    new_node->cod_destino=cod_destino;
    new_node->data=data;
    strcpy(new_node->destino,destino);
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

void diminuir_disp(Viagem global,int data,int code){
    while( global!=NULL ){
        if( global->cod_destino==code && global->data==data ){
            global->disponiveis=global->disponiveis - 1;
        }
        global=global->next;
    }
}

pCliente aumentar_disp(Viagem global,pCliente esp,int data,int code){
    pCliente next_node;
    pCliente esp_org;
    if( esp!=NULL ){
        next_node=esp->next;
        free(esp);
        return next_node;
    }else{
        while( global!=NULL ){
            if( global->cod_destino==code && global->data==data ){
                global->disponiveis=global->disponiveis + 1;
            }
            global=global->next;
        }
    }
    return esp_org;
}

int get_viagens_disp_destino(Viagem global,int data,int code){
    while( global!=NULL ){
        if( global->cod_destino==code && global->data==data ){
            return global->disponiveis;
        }
        global=global->next;
    }
}

pCliente insert_last_adq(int option,Viagem global,pCliente adq,pCliente esp,int data,char *destino,int cod_destino,int id,char *nome,int *out){
    pCliente new_node = (pCliente) malloc( sizeof(Clientes_node) );
    pCliente adq_ch = adq;
    pCliente esp_ch = esp;
    printf("new_node: %d\n",new_node);
    clean_input();
    new_node->data=data;
    new_node->cod_destino=cod_destino;
    new_node->id=id;
    strcpy(new_node->destino,destino);
    strcpy(new_node->nome,nome);
    switch( option ){
        case 0:
            if(adq==NULL){
                new_node->next=NULL;
                if(option==0){
                    diminuir_disp(global,data,cod_destino);
                }
                *out=0;
                return new_node;
            } else if( get_viagens_disp_destino(global,data,cod_destino) > 0 ){
                while(adq->next!=NULL){
                    adq=adq->next;
                }
                adq->next=new_node;
                new_node->next=NULL;
                diminuir_disp(global,data,cod_destino);
                *out=0;
                return adq_ch;
            } else if( get_viagens_disp_destino(global,data,cod_destino)==0 ){
                if(esp==NULL){
                    new_node->next=NULL;
                    *out=1;
                    return new_node;
                }else{
                    while(esp->next!=NULL){
                        esp=esp->next;
                    }
                    esp->next=new_node;
                    new_node->next=NULL;
                    *out =0;
                    return adq_ch;
                }
            }
            break;
        case 1:
            if(esp==NULL){
                    new_node->next=NULL;
                    *out=1;
                    return new_node;
                }else{
                    while(esp->next!=NULL){
                        esp=esp->next;
                    }
                    esp->next=new_node;
                    new_node->next=NULL;
                    *out=1;
                    return esp_ch;
                }
            break;
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
            printf("Disponiveis: %d\n",lista->disponiveis);
            printf("data: %d\n",lista->data);
            printf("\n");
            lista=lista->next;
        }
    }
    printf("\n");
    getchar();
}

void print_list_cliente(pCliente lista){
    if( lista==NULL ){
        printf("NULL\n");
    }else{
        while(lista!=NULL){
            printf("numero cc: %d\n",lista->id);
            printf("nome: %s",lista->nome);
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

void print_clientes(pCliente adq,pCliente esp){
    int int_arr[128];
    char *cl_arr[128];
    int j=0;
    int i=0;
    int id;
    while( adq!=NULL ){
        id=adq->id;
        if( in_arr_int(int_arr,id,i)==0 ){
            int_arr[i]=id;
            cl_arr[i]=adq->nome;
            ++i;
        }
        adq=adq->next;
    }
    while( j<i ){
        printf("-> %d %s\n",int_arr[j],cl_arr[j]);
        ++j;
    }

    while( esp!=NULL ){
        id=esp->id;
        if( in_arr_int(int_arr,id,i)==0 ){
            int_arr[i]=id;
            cl_arr[i]=esp->nome;
            ++i;
        }
        esp=esp->next;
    }
    while( j<i ){
        printf("-> %d %s\n",int_arr[j],cl_arr[j]);
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

void get_destino_from_cod(Viagem lista,int cod,char *destino){
    while( lista->cod_destino!=cod ){
        lista=lista->next;
    }
    strcpy(destino,lista->destino);
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
    char destino[128];
    int data,dia,mes,ano;
    int i=0;
    int j=0;
    get_destino_from_cod(lista,cod,destino);
    printf("%s\n",destino);
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

int get_nome_from_id(pCliente adq,pCliente esp,int cliente,char *nome){
    while( adq!=NULL ){
        if( adq->id==cliente ){
            strcpy(nome,adq->nome);
            return 0;
        }
        adq=adq->next;
    }
    while( esp!=NULL ){
        if( esp->id==cliente ){
            strcpy(nome,esp->nome);
            return 0;
        }
        esp=esp->next;
    }
    return 1;
}

int print_cliente_adq_adq(pCliente adq,pCliente esp, int cliente, Viagem lista){
    int adq_arr[128];
    int i=0;
    int j=0;
    int codes[128];
    int data,dia,mes,ano;
    int size=0;
    char destino[128];

    if(adq!=NULL) printf("\nAdquiridas:\n");
    while( adq!=NULL ){
        if( adq->id == cliente ){
            adq_arr[i]=data_invert(adq->data);
            codes[i]=adq->cod_destino;
            ++i;
        }
        adq=adq->next;
    }
    bubblesort_viagens(adq_arr,codes,i);
    while( j<i ){
        data=adq_arr[j];
        data=data_fix(data);
        data_to_dia_mes_ano(data,&dia,&mes,&ano);
        printf("(%d) ",j+1);
        get_destino_from_cod(lista,codes[j],destino);
        printf("%s\n",destino);
        printf(" / %d-%d-%d\n",dia,mes,ano);
        ++size;
        ++j;
    }
    return size;
}

int print_cliente_adq_esp(pCliente adq,pCliente esp, int cliente, Viagem lista){
    int esp_arr[128];
    int i=0;
    int j=0;
    int size=0;
    int data,dia,mes,ano;
    int codes[128];
    char destino[128];
    if(esp!=NULL) printf("\nViagens em lista de espera:\n");
    while( esp!=NULL ){
        if( esp->id == cliente ){
            esp_arr[i]=data_invert(esp->data);
            codes[i]=esp->cod_destino;
            ++i;
        }
        esp=esp->next;
    }
    bubblesort_viagens(esp_arr,codes,i);
    while( j<i ){
        data=esp_arr[j];
        data=data_fix(data);
        data_to_dia_mes_ano(data,&dia,&mes,&ano);
        printf("(%d) ",j+1);
        get_destino_from_cod(lista,codes[j],destino);
        printf("%s\n",destino);
        printf(" / %d-%d-%d\n",dia,mes,ano);
        ++size;
        ++j;
    }
    return size;
}

void print_cliente_adq(pCliente adq,pCliente esp, int cliente, Viagem lista){
    printf("Viagens e datas respetivas\n");
    print_cliente_adq_adq(adq,esp,cliente,lista);
    print_cliente_adq_esp(adq,esp,cliente,lista);
}

void clean_input(void){
    int ch;
    while ((ch=getchar()) != EOF && ch != '\n') ;
}

void remove_return(char *in,char *out){
    int i=0;
    while( in[i]!='\n' ){
        out[i]=in[i];
        ++i;
    }
}

pCliente cancelar_viagem(Viagem global,pCliente adq,pCliente esp,int id,int *out){
    char op[128];
    char nome[128];
    char destino[128];
    int option,size,data,code,cc;
    int i=1;
    pCliente prev_node=NULL;
    pCliente next_node=adq->next;
    pCliente esp_org=esp;
    printf("Cancelar Viagem\n");
    size = print_cliente_adq_adq(adq,esp,id,global);
    printf("\nOpcao: ");
    fgets (op, 128, stdin);
    option=atoi(op);
    if( isdigit(*op) && strlen(op)==2 && option<size+1 && option>0 ){
    } else {
        clrscr();
        printf("Opcao Invalida!\n\n");
        return cancelar_viagem(global,adq,esp,id,&out);
    }
    /*COMPLETAR*/
}

int menu_principal(Viagem global_viagens,pCliente adq,pCliente esp) {
    char op[128];
    int option,cod,data,id;
    char nome[128];
    char destino[128];
    char aux[128];
    int aux_int;
    pCliente aux_node;

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
            fgets (nome, 128, stdin);
            id=atoi(nome);
            if( get_nome_from_id(adq,esp,id,nome)==1 ){
                printf("Nome: ");
                cleanstr(nome);
                fgets (nome, 128, stdin);
            }
            remove_return(nome,aux);
            strcpy(nome,aux);
            aux_node = insert_last_adq(0,global_viagens,adq,esp,data,destino,cod,id,nome,&aux_int);
            if( aux_int==0 ){
                adq=aux_node;
            } else if( aux_int==1 ){
                esp=aux_node;
            }
            clean_input();
            break;
    /*(2)Colocar em fila de espera para uma viagem*/
        case 2:
            /*FIX: Proteger input*/
            clrscr();
            printf("Aquirir Viagem\n");
            cod=choose_list_viagem_destinos(global_viagens,destino);
            clrscr();
            printf("Aquirir Viagem\n");
            data=choose_list_viagem_data(global_viagens,cod);
            clrscr();
            printf("Aquirir Viagem\n");
            printf("Numero cartao do cidadao: ");
            fgets (nome, 128, stdin);
            id=atoi(nome);
            if( get_nome_from_id(adq,esp,id,nome)==1 ){
                printf("Nome: ");
                cleanstr(nome);
                fgets (nome, 128, stdin);
            }
            remove_return(nome,aux);
            strcpy(nome,aux);
            /*esp = insert_last_adq(1,global_viagens,&adq,&esp,data,destino,cod,id,nome);*/
            break;
        /*(3)Cancelar viagem*/
        case 3:
            /*FIX: Proteger input*/
            clrscr();
            if( adq==NULL && esp==NULL ){
                printf("Nao existem clientes no sistema\n");
                clean_input();
                break;
            }
            printf("Cancelar viagem\n");
            printf("Numero cartao do cidadao: ");
            fgets (nome, 128, stdin);
            id=atoi(nome);
            if( get_nome_from_id(adq,esp,id,nome)==1 ){
                printf("Cliente nao existe nao existente\n");
                clean_input();
                break;
            }
            clrscr();
            esp=cancelar_viagem(global_viagens,adq,esp,id,&aux_int);
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
            clean_input();
            break;
        /*(6)Listar viagens de um cliente*/
        case 6:
            /*Fix: proteger input*/
            clrscr();
            printf("Listar viagens de cliente\n");
            printf("Numero cartao do cidadao: ");
            fgets (nome, 128, stdin);
            id=atoi(nome);
            if( get_nome_from_id(adq,esp,id,nome)==1 ){
                printf("Cliente nao encontrado!\n");
                clean_input();
                break;
            }
            clrscr();
            print_cliente_adq(adq,esp,id,global_viagens);
            clean_input();
            break;
        /*(7)Listar clientes*/
        case 7:
            clrscr();
            printf("Lista de clientes\n");
            print_clientes(adq,esp);
            clean_input();
            break;
        case 8:
            return 0;
            break;
    }
    clrscr();
    cleanstr(op);
    printf("Deseja voltar?\n(S ou Enter)para voltar ou qualquer outra tecla para sair\n");
    printf("Opcao: ");
    fgets (op, 128, stdin);
    if( (strlen(op)==2 && (*op=='S' || *op=='s') || *op=='\n') ){
        clrscr();
        menu_principal(global_viagens,adq,esp);
    }else return 0;
}
