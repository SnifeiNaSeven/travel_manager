#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "global.h"

int main(){
    /*Import de ficheiros para listas*/
    pCliente esp=NULL;
    pCliente adq=NULL;
    Viagem global_viagens = NULL;

    FILE *viagens;
    FILE *adquiridas;
    FILE *espera;
    FILE *adquiridas_wrt;
    FILE *espera_wrt;

    viagens = fopen("viagens-datas.txt","r");
    global_viagens = createGlobalListFromFile(viagens, global_viagens);
    fclose(viagens);
    if( global_viagens==NULL ) return 0;


    adquiridas = fopen("viagens-adquiridas.txt","r");
    adq = createAdquiridaListFromFile(adquiridas,adq,esp,global_viagens);
    fclose(adquiridas);

    espera = fopen("viagens-espera.txt","r");
    esp = createEsperaListFromFile(espera,esp,adq,global_viagens);
    fclose(espera);

    /*Menu*/
    menu_principal(global_viagens,&adq,&esp);

    /*Atualizar ficheiros*/
    adquiridas_wrt=fopen("viagens-adquiridas.txt","w");
    Export_cliente_to_file(adquiridas_wrt,&adq);
    fclose(adquiridas_wrt);

    espera_wrt=fopen("viagens-espera.txt","w");
    Export_cliente_to_file(espera_wrt,&esp);
    fclose(espera);

    /*Destruir listas*/
    destroi_lista_viagem(global_viagens);
    destroi_lista_cliente(adq);
    destroi_lista_cliente(esp);

    return 0;
}
