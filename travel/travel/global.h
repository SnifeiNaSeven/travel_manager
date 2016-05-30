typedef struct clientes_node *pCliente;

typedef struct clientes_node {
    int data;
    char destino[128];
    int cod_destino;
    int id;
    char nome[128];
    pCliente next;
} Clientes_node;

typedef struct viagem_node *Viagem;

typedef struct viagem_node {
    int data;
    int cod_destino;
    int disponiveis;
    char destino[128];
    Viagem next;
} Viagem_node;

/*http://www.dreamincode.net/forums/topic/90898-cross-platform-clear-screen/*/
#ifndef WIN32
#define COMMAND "cls" /*Clears a windows console screen*/
#else
#define COMMAND "clear" /*Clears a linux console screen*/
#endif
#define clrscr() system( COMMAND )

/*Clientes*/
pCliente createEsperaListFromFile(FILE *file, pCliente lista,pCliente adq,Viagem global);
pCliente createAdquiridaListFromFile(FILE *file, pCliente lista,pCliente esp,Viagem global);
void Export_cliente_to_file(FILE *file,pCliente *cliente);
void enqueue(pCliente *lista,int code,int data,char *destino,int id,char *nome);
void insert_last_adq(int option,Viagem *global,pCliente *adq,pCliente *esp,int data,char *destino,int cod_destino,int id,char *nome);
void destroi_lista_cliente(pCliente lista);
void print_clientes(pCliente *adq,pCliente *esp);
void print_viagens_mais_recentes(Viagem *lista, int cod);
int get_nome_from_id(pCliente *adq,pCliente *esp,int cliente,char *nome);
int print_cliente_adq_adq(pCliente *adq, int cliente, Viagem *lista,int *data_arr,int *codes);
int print_cliente_adq_esp(pCliente *esp, int cliente, Viagem *lista,int *data_arr, int *codes);
void print_cliente_adq(pCliente adq,pCliente esp, int cliente, Viagem lista);
int in_cliente(pCliente *cliente,int id);
void dequeue(pCliente *lista,int id,int code,int data);
void dequeue_and_save(pCliente *lista,int *code,int *data,char *destino, int *id, char *nome);
void remover_espera(Viagem *global,pCliente *adq,pCliente *esp,int id);
void remover_viagem(Viagem *global,pCliente *adq,pCliente *esp,int id);

/*Viagens*/
Viagem createGlobalListFromFile(FILE *file, Viagem lista);
int choose_list_viagem_data(Viagem *lista,int x);
int choose_list_viagem_destinos(Viagem *lista,char *destino);
Viagem insert_last_viagem(Viagem lista,int data,int cod_destino,char *destino,int disponiveis);
void diminuir_disp(Viagem *global,int data,int code);
void aumentar_disp(Viagem *global,int data,int code);
int get_viagens_disp_destino(Viagem *global,int data,int code);
void destroi_lista_viagem(Viagem lista);
void get_destino_from_cod(Viagem *lista,int cod,char *destino);


/*Menue extras*/
void cleanstr(char *out);
int getlinename(char *line,char *aux);
int getlinecode(char *line);
int in_arr_int(int *arr,int i,int size);
void data_to_dia_mes_ano(int data, int *dia, int *mes, int *ano);
int data_invert(int data);
int data_fix(int data);
void bubblesort_inv(int vect[], int n);
void bubblesort_viagens(int vect[],int *codes, int n);
void clean_input(void);
void remove_return(char *in);
int check_name(char *str);
int check_number(char *str);
int menu_principal(Viagem global_viagens,pCliente *adq,pCliente *esp);
