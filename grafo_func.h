#include <stdlib.h>
#include <stdio.h>
#ifndef grafo_func_h
#define grafo_func_h
typedef struct grafo_solucao {
    int n_entregra; // n (numero total de entregas) [pode ser visto tbm como o tamanho da matriz ou seja a matriz sempre vai ser n+1xn+1)(portanto pode ser usado para iterar sobre os dados)]
    int numero_veiculos; // k (numero total de veiculos que pode ser usados)
    int capcacidade_carga_carro; // q (quantidade de carga que cada carro pode carregar)
    int entrega_nao_tercerizada; // l [numero obrigatorio de entregas nao tercerizadas]
    int custo_carro; // r [custo por carro a cada rota]
    int *demanda_de_cliente; // d (demanda de cada cliente )
    int *custo_tercerizar; // p (custo para tercerizar entrega, cada entrega tem seu custo independete)
    int **matriz_custo; 
    int num_aresta;
    int num_ver;
} grafo_solucao;

grafo_solucao carregar_grafo(char n_arquivo[]){ // carrega todos dados do arquivo em grafo 
    FILE *arquivo;
    grafo_solucao grafo;
    grafo.num_aresta = 0;
    arquivo = fopen(n_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
    }

    fscanf(arquivo, "%d", &grafo.n_entregra);
    fscanf(arquivo, "%d", &grafo.numero_veiculos);
    fscanf(arquivo, "%d", &grafo.capcacidade_carga_carro);
    fscanf(arquivo, "%d", &grafo.entrega_nao_tercerizada);
    fscanf(arquivo, "%d", &grafo.custo_carro);


    grafo.num_ver = grafo.n_entregra+1; // pra incluir o vertice de origem(ou seja o vertice do deposito)


    grafo.matriz_custo = (int **)malloc(grafo.num_ver * sizeof(int *)); // aloca espaço para matriz_de_adjacencia(peso)
    for (int i = 0; i < grafo.num_ver; i++) {
        grafo.matriz_custo[i] = (int *)malloc(grafo.num_ver * sizeof(int));
    }



    grafo.demanda_de_cliente = (int *)malloc(grafo.n_entregra * sizeof(int));
    for (int i = 0; i < grafo.n_entregra; i++) {
        fscanf(arquivo, "%d", &grafo.demanda_de_cliente[i]);
    }

    grafo.custo_tercerizar = (int *)malloc(grafo.n_entregra * sizeof(int));
    for (int i = 0; i < grafo.n_entregra; i++) {
        fscanf(arquivo, "%d", &grafo.custo_tercerizar[i]);
    }
    for (int i = 0; i < grafo.num_ver; i++) {
        for (int j = 0; j < grafo.num_ver; j++) {
            fscanf(arquivo, "%d", &grafo.matriz_custo[i][j]);
            if(grafo.matriz_custo[i][j]!=0){
                grafo.num_aresta++;
            }
        }
    }
    fclose(arquivo);
    return grafo;
}
#endif