#include <stdlib.h>
#include <stdio.h>
#include "carregar_grafo.h"
#define limite_peso 9999
#include <stdbool.h>
#include <string.h>
typedef struct arestas_minimas
{
    int origen;
    int destino;
    int peso;
}arestas_minimas;

typedef struct rota
{
    int pesototal; // pesotal gasto para visitar os clientes
    int toltal_entregue; // toltal de produstos entrege durante essa rota
    int * clientes_visitados; // array de clientes que foram visitados durante essa rota
    int total_de_clientes_visitado;
}rota;

arestas_minimas * caminho(grafo_solucao grafo);
rota gerar_rotas(arestas_minimas * arestas,grafo_solucao grafo); // gera as rotas respetiando a capicadade dos carros
void remover_clientes_visitados_grafo(grafo_solucao grafo,rota rota1); // 
rota *troca_clientes_entre_rotas(rota *rotas, int num_rotas, grafo_solucao *grafo);
rota movimento_em_unica_rota(rota *rota_atual, grafo_solucao *grafo); // troca as ordem do clientes na rota buscando otimizar o peso
bool verificar_rota_cap_carga_carro(grafo_solucao *grafo, rota *rota_atual);
int calcular_peso_rota(grafo_solucao *grafo, rota *rota_atual) ;

int main(){

    char nome_arquivo[] = "n9k5_A.txt";

    grafo_solucao grafo1 = carregar_grafo(nome_arquivo); // grafo usado para gerar as rotas
    grafo_solucao grafo2 = carregar_grafo(nome_arquivo);// grafo usado para fazer os movimento entre rotas

    rota * rotas = (rota*) malloc(grafo1.numero_veiculos*sizeof(rota)); // o numero de rotas se o numero de veiculos disponiveis    

    for(int i =0;i<grafo1.numero_veiculos;i++){
        arestas_minimas *arestas_minimas;
        arestas_minimas = caminho(grafo1);
        rotas[i] = gerar_rotas(arestas_minimas,grafo1);
        remover_clientes_visitados_grafo(grafo1,rotas[i]);
        
    }
    //printf("%d",rotas[1].pesototal);
    /*for(int i =0;i<rotas[1].total_de_clientes_visitado;i++){
        printf("%d-",rotas[1].clientes_visitados[i]);
    }*/
    ///rotas[0]  = movimento_em_unica_rota(&rotas[0],&grafo2);
    rota * rotas1 = (rota*) malloc(grafo1.numero_veiculos*sizeof(rota)); 
    /*printf("\n%d",calcular_peso_rota(&grafo2,&rotas[0]));
    printf("\n%d",melhora_rota.pesototal);*/
    //printf("\n%d",melhora_rota.pesototal);
    //printf("\n%d",rotas[0].pesototal);
    bool cond = false;
    cond = verificar_rota_cap_carga_carro(&grafo2,&rotas[0]);
    rotas1 = troca_clientes_entre_rotas(rotas,grafo2.numero_veiculos,&grafo2);
    for(int i =0;i<rotas[0].total_de_clientes_visitado;i++){
        printf("%d-",rotas1[0].clientes_visitados[i]);
    }
}

rota gerar_rotas(arestas_minimas *arestas, grafo_solucao grafo) {
    int capacidade_maxima = grafo.capcacidade_carga_carro;
    int prox_vert;
    int peso_total_rota = 0;

    prox_vert = arestas[0].destino;
    peso_total_rota += arestas[0].peso;

    int soma_total_entregado = grafo.demanda_de_cliente[prox_vert - 1];

    int *clientes_visitados = (int *)malloc(sizeof(int));
    clientes_visitados[0] = prox_vert;
    int total_de_clientes_visitados = 1;

    for (int i = 1; i < grafo.n_entregra; i++) {
        if (arestas[i].origen == prox_vert) {
            prox_vert = arestas[i].destino;
            int demanda_cliente = grafo.demanda_de_cliente[prox_vert - 1];

            if (soma_total_entregado + demanda_cliente > capacidade_maxima) {
                break;
            }

            clientes_visitados = (int *)realloc(clientes_visitados, (total_de_clientes_visitados + 1) * sizeof(int));
            clientes_visitados[total_de_clientes_visitados] = prox_vert;
            total_de_clientes_visitados++;

            peso_total_rota += arestas[i].peso;
            soma_total_entregado += demanda_cliente;
        }
    }

    rota rota_atual;

    rota_atual.pesototal = peso_total_rota;
    rota_atual.toltal_entregue = soma_total_entregado;
    rota_atual.clientes_visitados = clientes_visitados;
    rota_atual.total_de_clientes_visitado = total_de_clientes_visitados;
    
    int ultimo_cliente = clientes_visitados[total_de_clientes_visitados-1];
    rota_atual.pesototal += grafo.custo_carro;
    rota_atual.pesototal += grafo.matriz_custo[ultimo_cliente][0]; // calcular peso do ultimo cliente visitado de volta para o deposito
    return rota_atual;
}
arestas_minimas * caminho(grafo_solucao grafo){ // algoritmo guloso
    int menor_peso = 0;
    int visitados[grafo.num_ver-1];
    arestas_minimas* lista_de_arestas = (arestas_minimas*)malloc(grafo.num_ver * sizeof(arestas_minimas));
    int vertice_de_menor_peso;
    for (int i = 0; i < grafo.num_ver-1; i++){
        visitados[i] = 0; // preenche todo vetor marcando todos os vertices como nao visitados
        int count =0;
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
            /*printf("%d->",lista_de_arestas[i].origen);
            printf("%d",lista_de_arestas[i].destino);
            printf(" peso = %d \n",lista_de_arestas[i].peso);*/
        }
   }
   return lista_de_arestas;

}
void remover_clientes_visitados_grafo(grafo_solucao grafo,rota rota1){
    for(int i =0;i<rota1.total_de_clientes_visitado;i++){
        for(int j=0;j<grafo.num_ver;j++){
            int indice = rota1.clientes_visitados[i];
            grafo.matriz_custo[indice][j] = 0;
            grafo.matriz_custo[j][indice] = 0;
        }
    }

}

rota movimento_em_unica_rota(rota *rota_atual, grafo_solucao *grafo) {
    // Calcula o peso da rota atual
    int melhor_peso = calcular_peso_rota(grafo, rota_atual);
    
    // Inicializa a melhor rota 
    rota melhor_rota;

    melhor_rota.clientes_visitados = (int*)malloc(sizeof(int) * rota_atual->total_de_clientes_visitado);
    memcpy(melhor_rota.clientes_visitados, rota_atual->clientes_visitados, sizeof(int) * rota_atual->total_de_clientes_visitado);
    melhor_rota.pesototal = melhor_peso;
    melhor_rota.toltal_entregue = rota_atual->toltal_entregue;
    melhor_rota.total_de_clientes_visitado = rota_atual->total_de_clientes_visitado;


    // Percorre todas as combinações possíveis na vizinhança
    for (int i = 0; i < rota_atual->total_de_clientes_visitado-1; i++) {
        for (int j = i + 1; j < rota_atual->total_de_clientes_visitado; j++) {
            // Realiza a troca dos clientes na rota
            int temp = rota_atual->clientes_visitados[i];
            rota_atual->clientes_visitados[i] = rota_atual->clientes_visitados[j];
            rota_atual->clientes_visitados[j] = temp;
    
            // Avalia o peso da nova rota após a troca
            int peso_vizinho = calcular_peso_rota(grafo, rota_atual);

            // Se o peso da nova rota for menor que o melhor peso encontrado até agora
            if (peso_vizinho < melhor_peso) {
                // Atualiza o melhor peso
                melhor_peso = peso_vizinho;
                // Atualiza a melhor rota com uma cópia da rota atual
                melhor_rota.clientes_visitados = (int*)malloc(sizeof(int) * rota_atual->total_de_clientes_visitado);
                memcpy(melhor_rota.clientes_visitados, rota_atual->clientes_visitados, sizeof(int) * rota_atual->total_de_clientes_visitado);
                melhor_rota.pesototal = melhor_peso;
                melhor_rota.toltal_entregue = rota_atual->toltal_entregue;
                melhor_rota.total_de_clientes_visitado = rota_atual->total_de_clientes_visitado;
            }
            
            /*// Desfaz a troca para explorar outros vizinhos
            temp = rota_atual->clientes_visitados[i];
            rota_atual->clientes_visitados[i] = rota_atual->clientes_visitados[j];
            rota_atual->clientes_visitados[j] = temp;*/
        }
    }
    // Retorna a melhor rota encontrada na vizinhança
    return melhor_rota;
}
rota *troca_clientes_entre_rotas(rota *rotas, int num_rotas, grafo_solucao *grafo) {
    // Aloca memória para uma matriz que armazenará as novas rotas após a troca de clientes
    rota *novas_rotas = (rota*)malloc(num_rotas * sizeof(rota));
    // Faz uma cópia das rotas originais nas novas rotas
    for (int i = 0; i < num_rotas; i++) {
        novas_rotas[i].clientes_visitados = (int*)malloc(sizeof(int) * rotas[i].total_de_clientes_visitado);
        memcpy(novas_rotas[i].clientes_visitados, rotas[i].clientes_visitados, sizeof(int) * rotas[i].total_de_clientes_visitado);
        novas_rotas[i].pesototal = calcular_peso_rota(grafo, &rotas[i]);
        novas_rotas[i].toltal_entregue = rotas[i].toltal_entregue;
        novas_rotas[i].total_de_clientes_visitado = rotas[i].total_de_clientes_visitado;
    }

    // Percorre todas as combinações possíveis de troca de clientes entre as rotas
    for (int i = 0; i < num_rotas; i++) {
        for (int j = 0; j < num_rotas; j++) {
            if (i != j) { // Evita a troca com a mesma rota
                for (int cliente_i = 0; cliente_i < rotas[i].total_de_clientes_visitado; cliente_i++) {
                    for (int cliente_j = 0; cliente_j < rotas[j].total_de_clientes_visitado; cliente_j++) {
                        // Realiza a troca dos clientes entre as rotas
                        int temp = novas_rotas[i].clientes_visitados[cliente_i];
                        novas_rotas[i].clientes_visitados[cliente_i] = novas_rotas[j].clientes_visitados[cliente_j];
                        novas_rotas[j].clientes_visitados[cliente_j] = temp;

                        // Calcula os pesos das novas rotas
                        int peso_rota_i = calcular_peso_rota(grafo, &novas_rotas[i]);
                        int peso_rota_j = calcular_peso_rota(grafo, &novas_rotas[j]);

                        // Verifica se a troca resulta em rotas com menor peso total e tbm verifica se respeita a capacidade de entrega do carro
                        if (peso_rota_i + peso_rota_j < novas_rotas[i].pesototal + novas_rotas[j].pesototal && verificar_rota_cap_carga_carro(grafo, &novas_rotas[i])==true && verificar_rota_cap_carga_carro(grafo, &novas_rotas[j])==true) {
                            // Atualiza os pesos das rotas com os novos valores
                            novas_rotas[i].pesototal = peso_rota_i;
                            novas_rotas[j].pesototal = peso_rota_j;
                        } else {
                            // Desfaz a troca
                            temp = novas_rotas[i].clientes_visitados[cliente_i];
                            novas_rotas[i].clientes_visitados[cliente_i] = novas_rotas[j].clientes_visitados[cliente_j];
                            novas_rotas[j].clientes_visitados[cliente_j] = temp;
                        }
                    }
              }
            }
        }
    }

    return novas_rotas;
}
int calcular_peso_rota(grafo_solucao *grafo, rota *rota_atual) {
    int peso_total = 0;

    // A matriz de distâncias é uma matriz 2D em grafo->matriz_distancias
    int **matriz_distancias = grafo->matriz_custo;
    //Calcule a distância do deposito para primeiro cliente 
    peso_total = matriz_distancias[0][rota_atual->clientes_visitados[0]];

    for (int i = 0; i < rota_atual->total_de_clientes_visitado - 1; i++) {
        int cliente_atual = rota_atual->clientes_visitados[i];
        int prox_cliente = rota_atual->clientes_visitados[i + 1];
        // Calcule a distância entre o cliente atual e o próximo cliente
        peso_total += matriz_distancias[cliente_atual][prox_cliente];
    }

    // Calcule a distância do último cliente de volta ao depósito (0)
    int ultimo_cliente = rota_atual->clientes_visitados[rota_atual->total_de_clientes_visitado - 1];
    peso_total += matriz_distancias[ultimo_cliente][0];
    peso_total+=grafo->custo_carro;
    //printf("\n%d",peso_total);
    //printf("\n%d\n",peso_total);
    return peso_total;
}

bool verificar_rota_cap_carga_carro(grafo_solucao *grafo, rota *rota_atual){
    int total_entregue = 0;

    for(int i = 0; i < rota_atual->total_de_clientes_visitado - 1; i++) {
        total_entregue += grafo->demanda_de_cliente[rota_atual->clientes_visitados[i]-1];
    }
    if(total_entregue<=grafo->capcacidade_carga_carro){
        return true;

    }
    else{return false;}
}
