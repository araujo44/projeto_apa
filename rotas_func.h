#ifndef rotas_func_h
#define rotas_func_h
#include "algoritmo_guloso.h"
#include <stdbool.h>
#include <string.h>

typedef struct rota
{
    int pesototal; // peso total gasto para visitar os clientes
    int toltal_entregue; // total de produtos entreges durante rota
    int * clientes_visitados; // array de clientes que foram visitados durante rota
    int total_de_clientes_visitado;
}rota;
typedef struct clientes_tercerizado{
    int * clientes_tercerizado;
    int total_de_clientes_ter;
    int peso_toltal_cli_tercerizados;
}clientes_tercerizado;

void alterarPesoRota(rota *rota_atual, int novoValor) {
    // Modifica o peso total de uma rota
    rota_atual->pesototal = novoValor;
}

int retornar_toltal_durante_rota(grafo_solucao *grafo, rota *rota_atual){ 
    // Calcula o total de encomendas entregues durante uma rota, somando as demandas dos clientes visitados
    int total_entregue = 0;

    for(int i = 0; i < rota_atual->total_de_clientes_visitado; i++) {
        total_entregue += grafo->demanda_de_cliente[rota_atual->clientes_visitados[i]-1];
    }
    return total_entregue;
}

bool verificar_rota_cap_carga_carro(grafo_solucao *grafo, rota *rota_atual){
    // int total_entregue = 0;

    // for(int i = 0; i < rota_atual->total_de_clientes_visitado; i++) {
    //     total_entregue += grafo->demanda_de_cliente[rota_atual->clientes_visitados[i]-1];
    // }

    // Verifica se a rota respeita a capacidade de carga do carro, 
    // comparando o total entregue a capacidade máxima
    
    if(total_entregue <= grafo->capcacidade_carga_carro){
        return true;
    }
    else{
        return false;
    }
}

int calcular_peso_rota(grafo_solucao *grafo, rota *rota_atual) {
    //  Calcula o peso total de uma rota, considerando as distâncias entre os clientes visitados e o depósito
    
    int peso_total = 0;

    // A matriz de distâncias é uma matriz 2D em grafo->matriz_distancias
    int **matriz_distancias = grafo->matriz_custo;
    
    //Calcula a distância do deposito para primeiro cliente 
    peso_total = matriz_distancias[0][rota_atual->clientes_visitados[0]];

    for (int i = 0; i < rota_atual->total_de_clientes_visitado - 1; i++) {
        int cliente_atual = rota_atual->clientes_visitados[i];
        int prox_cliente = rota_atual->clientes_visitados[i + 1];
        peso_total += matriz_distancias[cliente_atual][prox_cliente];
    }

    // Calcule a distância do último cliente de volta ao depósito (0)
    int ultimo_cliente = rota_atual->clientes_visitados[rota_atual->total_de_clientes_visitado - 1];
    peso_total += matriz_distancias[ultimo_cliente][0];
    peso_total+=grafo->custo_carro;
    return peso_total;
}

rota gerar_rotas(arestas_minimas *arestas, grafo_solucao grafo) {
    // Gera rota a partir de um conjunto de arestas mínimas; 
    // é adicionado clientes visitados até atingir a capacidade máxima do carro
    
    int capacidade_maxima = grafo.capcacidade_carga_carro;
    int prox_vert;
    int peso_total_rota = 0;
    int soma_total_entregado = 0;

    // Inicializa um vetor dinâmico para armazenar os clientes visitados
    int *clientes_visitados = (int *) malloc(grafo.n_entregra * sizeof(int));
    int total_de_clientes_visitados = 0;

    prox_vert = arestas[0].destino;
    peso_total_rota += arestas[0].peso;
    clientes_visitados[total_de_clientes_visitados] = prox_vert;
    total_de_clientes_visitados++;
    soma_total_entregado += grafo.demanda_de_cliente[prox_vert - 1];

    for (int i = 1; i < grafo.n_entregra; i++) {
        if (arestas[i].origen == prox_vert) {
            prox_vert = arestas[i].destino;
            int demanda_cliente = grafo.demanda_de_cliente[prox_vert - 1];

            if (soma_total_entregado + demanda_cliente > capacidade_maxima) {
                break;
            }

            clientes_visitados[total_de_clientes_visitados] = prox_vert;
            total_de_clientes_visitados++;
            peso_total_rota += arestas[i].peso;
            soma_total_entregado += demanda_cliente;
        }
    }

    // Aloca exatamente o espaço necessário para armazenar os clientes visitados
    int *clientes_visitados_final = (int *)malloc(total_de_clientes_visitados * sizeof(int));
    memcpy(clientes_visitados_final, clientes_visitados, total_de_clientes_visitados * sizeof(int));

    // Libera o espaço do vetor dinâmico temporário
    free(clientes_visitados);

    rota rota_atual;

    rota_atual.pesototal = peso_total_rota;
    rota_atual.toltal_entregue = soma_total_entregado;
    rota_atual.clientes_visitados = clientes_visitados_final;
    rota_atual.total_de_clientes_visitado = total_de_clientes_visitados;

    int ultimo_cliente = clientes_visitados_final[total_de_clientes_visitados - 1];
    rota_atual.pesototal += grafo.custo_carro;
    rota_atual.pesototal += grafo.matriz_custo[ultimo_cliente][0]; // calcular peso do último cliente visitado de volta para o depósito

    return rota_atual;
}

void remover_clientes_visitados_grafo(grafo_solucao grafo, rota rota1){
    //  remove os clientes visitados na rota1 pela matriz de custos do grafo
    
    for(int i =0;i<rota1.total_de_clientes_visitado;i++){
        for(int j=0;j<grafo.num_ver;j++){
            int indice = rota1.clientes_visitados[i];
            grafo.matriz_custo[indice][j] = 0;
            grafo.matriz_custo[j][indice] = 0;
        }
    }

}

rota movimento_em_unica_rota(rota *rota_atual, grafo_solucao *grafo) {
    // 1a. estrutura de vizinhança: movimento em uma única rota
    // Troca as posições dos clientes dentro da rota para ver se reduz o custo total
    
    // Calcula o peso da rota atual
    int melhor_peso = calcular_peso_rota(grafo, rota_atual);
    
    // Inicializa a melhor rota como cópia da rota atual
    rota melhor_rota;
    melhor_rota.clientes_visitados = (int*)malloc(sizeof(int) * rota_atual->total_de_clientes_visitado);
    memcpy(melhor_rota.clientes_visitados, rota_atual->clientes_visitados, sizeof(int) * rota_atual->total_de_clientes_visitado);
    melhor_rota.pesototal = melhor_peso;
    melhor_rota.toltal_entregue = rota_atual->toltal_entregue;
    melhor_rota.total_de_clientes_visitado = rota_atual->total_de_clientes_visitado;


    // Percorre todas as combinações possíveis de trocas na vizinhança
    for (int i = 0; i < rota_atual->total_de_clientes_visitado-1; i++) {
        for (int j = i + 1; j < rota_atual->total_de_clientes_visitado; j++) {
            // Realiza a troca dos clientes na rota
            int temp = rota_atual->clientes_visitados[i];
            rota_atual->clientes_visitados[i] = rota_atual->clientes_visitados[j];
            rota_atual->clientes_visitados[j] = temp;
    
            // Avalia peso da nova rota após a troca
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
        }
    }
    
    free(rota_atual->clientes_visitados);
    
    return melhor_rota;
}

void modificarClientesVisitados(rota *rota_atual, int novoValor, int indice) {
    //  Modifica, se possível, o cliente visitado na posição dada
    
    if (indice >= 0 && indice < rota_atual->total_de_clientes_visitado) {
        rota_atual->clientes_visitados[indice] = novoValor;
    }
}

void troca_clientes_entre_rotas(rota *rotas, int num_rotas, grafo_solucao *grafo) {
    // 2a. estrutura de vizinhança: movimentos em múltiplas rotas
    // Percorre todas as combinações possíveis de troca de clientes entre as rotas
    
    for (int i = 0; i < num_rotas; i++) {
        for (int j = 0; j < num_rotas; j++) {
            if (i != j) { // Evita a troca com a mesma rota
                for (int cliente_i = 0; cliente_i < rotas[i].total_de_clientes_visitado; cliente_i++) {
                    for (int cliente_j = 0; cliente_j < rotas[j].total_de_clientes_visitado; cliente_j++) {
                        // Realiza a troca dos clientes entre as rotas
                        int temp = rotas[i].clientes_visitados[cliente_i];
                        modificarClientesVisitados(&rotas[i], rotas[j].clientes_visitados[cliente_j], cliente_i);
                        modificarClientesVisitados(&rotas[j], temp, cliente_j);

                        // Calcula os pesos das novas rotas
                        int peso_rota_i = calcular_peso_rota(grafo, &rotas[i]);
                        int peso_rota_j = calcular_peso_rota(grafo, &rotas[j]);

                        // Verifica se a troca resulta em rotas com menor peso total e respeita a capacidade de entrega do carro
                        if (peso_rota_i + peso_rota_j < rotas[i].pesototal + rotas[j].pesototal /*&&
                            verificar_rota_cap_carga_carro(grafo, &rotas[i]) &&
                            verificar_rota_cap_carga_carro(grafo, &rotas[j])*/) {
                            // Atualiza os pesos das rotas com os novos valores
                            alterarPesoRota(&rotas[i], peso_rota_i);
                            alterarPesoRota(&rotas[j], peso_rota_j);
                            rotas[i].toltal_entregue = retornar_toltal_durante_rota(grafo, &rotas[i]);
                            rotas[j].toltal_entregue = retornar_toltal_durante_rota(grafo, &rotas[j]);
                        } else {
                            // Desfaz a troca
                            temp = rotas[i].clientes_visitados[cliente_i];
                            modificarClientesVisitados(&rotas[i], rotas[j].clientes_visitados[cliente_j], cliente_i);
                            modificarClientesVisitados(&rotas[j], temp, cliente_j);
                        }
                    }
                }
            }
        }
    }
}

clientes_tercerizado verificar_clientes_a_tercerizar(rota *rota_atual, grafo_solucao *grafo) { 
    // verifica quais clientes nao foram atingidos pelos carros e terceriza eles
    
    clientes_tercerizado c1;
    int count = 0;
    int peso_total_tercerizado =0;
    int *clientes_visitados = (int *)calloc(grafo->n_entregra, sizeof(int));
    for (int i = 0; i < grafo->numero_veiculos; i++) {
        for (int j = 0; j < rota_atual[i].total_de_clientes_visitado; j++) {
            clientes_visitados[rota_atual[i].clientes_visitados[j] - 1] = 1;
        }
    }

    // Conta o número de clientes não visitados
    for (int i = 0; i < grafo->n_entregra; i++) {
        if (clientes_visitados[i] == 0) {
            count++;
        }
    }

    // Aloca memória
    int *cli_nao_vi = (int *)calloc(count, sizeof(int));

    int index = 0;
    for (int i = 0; i < grafo->n_entregra; i++) {
        if (clientes_visitados[i] == 0) {
            cli_nao_vi[index] = i + 1 ; // para representar o cliente não visitado
            peso_total_tercerizado+=grafo->custo_tercerizar[i];
            index++;
        }
    }

    c1.clientes_tercerizado = cli_nao_vi;
    c1.total_de_clientes_ter = count;
    c1.peso_toltal_cli_tercerizados = peso_total_tercerizado;
    free(clientes_visitados); // Libera a memória alocada para clientes_visitados

    return c1;
}

int calcular_peso_tercerizado(clientes_tercerizado clientes_tercerizados1, int num_clientes_tercerizados, grafo_solucao *grafo) {
    // Retorna os custos de terceirização
    
    int peso_total =0;
    
    for (int i = 0; i < num_clientes_tercerizados; i++) {

        peso_total += grafo->custo_tercerizar[clientes_tercerizados1.clientes_tercerizado[i] - 1];
    }
    return peso_total;
}

void movimento_entres_rotas_e_tercerizado(rota *rotas, int num_rotas, clientes_tercerizado *clientes_tercerizados, int num_clientes_tercerizados, grafo_solucao *grafo) {
    // 3a. estrutura de vizinhança: movimentos com terceirização de entregas
    // Troca o cliente da rota pelo cliente terceirizado
    
        for (int i = 0; i < num_clientes_tercerizados; i++) {
            for (int j = 0; j < num_rotas; j++) {
                for (int k = 0; k < rotas[j].total_de_clientes_visitado; k++) {
                    int temp = rotas[j].clientes_visitados[k];
                    modificarClientesVisitados(&rotas[j], clientes_tercerizados->clientes_tercerizado[i], k);
                    clientes_tercerizados->clientes_tercerizado[i] = temp;

                    int peso_antigo_tercerizado = clientes_tercerizados->peso_toltal_cli_tercerizados;
                    int peso_novo_tercerizado = calcular_peso_tercerizado(*clientes_tercerizados, num_clientes_tercerizados, grafo);
                
                    int peso_total_nova_rota = calcular_peso_rota(grafo, &rotas[j]);

                    if (peso_total_nova_rota + peso_novo_tercerizado < rotas[j].pesototal + peso_antigo_tercerizado && verificar_rota_cap_carga_carro(grafo, &rotas[j])) {
                        alterarPesoRota(&rotas[j], peso_total_nova_rota); // Chama a função para alterar o peso da rota
                        rotas[j].toltal_entregue = retornar_toltal_durante_rota(grafo, &rotas[j]);
                        clientes_tercerizados->peso_toltal_cli_tercerizados = peso_novo_tercerizado;
                    } else {
                        // Desfazer a troca
                        temp = rotas[j].clientes_visitados[k];
                        modificarClientesVisitados(&rotas[j], clientes_tercerizados->clientes_tercerizado[i], k);
                        clientes_tercerizados->clientes_tercerizado[i] = temp;
                    }
                }
            }
        }
    }

#endif
