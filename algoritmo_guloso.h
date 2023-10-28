#ifndef algoritmo_guloso_h
#define algoritmo_guloso_h
#include "grafo_func.h"
#define limite_peso 9999

typedef struct arestas_minimas
{
    int origen;
    int destino;
    int peso;
}arestas_minimas;

arestas_minimas * caminho(grafo_solucao grafo){ // algoritmo guloso
    int menor_peso = 0;
    int visitados[grafo.num_ver-1];
    arestas_minimas* lista_de_arestas = (arestas_minimas*)malloc(grafo.num_ver * sizeof(arestas_minimas));
    int vertice_de_menor_peso;
    for (int i = 0; i < grafo.num_ver-1; i++){
        visitados[i] = 0; // preenche todo vetor marcando todos os vertices como nao visitados
    }
    for (int i = 0; i < grafo.num_ver-1; i++) // i = origen , j detino
    {
        visitados[i]=1;
        menor_peso = limite_peso;
        for (int j = 0; j < grafo.num_ver; j++)
        {
            
            if (grafo.matriz_custo[i][j] != 0 && grafo.matriz_custo[i][j]<menor_peso ){ 

                if (visitados[j]!=1) // verifica se o vertice atual ja foi visitado
                {
                    menor_peso = grafo.matriz_custo[i][j];
                    vertice_de_menor_peso = j;
                }
            }
        }
        if(menor_peso != limite_peso){
            lista_de_arestas[i].origen = i;
            lista_de_arestas[i].destino = vertice_de_menor_peso;
            lista_de_arestas[i].peso = menor_peso;
        }
   }
   return lista_de_arestas;

}
#endif