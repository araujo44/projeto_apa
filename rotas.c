#include "grafo_func.h"
#include "rotas_func.h"
#include "algoritmo_guloso.h"
#include <dirent.h>
#include <time.h>

typedef struct solucao
{
    int num_rotas;
    int pesototal; // pesotal de todas as rotas
    rota * rotas; // todas as rotas
    clientes_tercerizado * clientes_tercerizado; 
    int custor_rotear_v;
    int custo_tercerizado;
}solucao;
void mostrar_resultado(solucao s1,grafo_solucao g1);
void VND(rota *rotas, int num_rotas, grafo_solucao *grafo,clientes_tercerizado *clientes_tercerizados);
solucao criar_solucao(rota * rotas,clientes_tercerizado * clientes_t,int num_rotas,int custo_uso_v);
void out_vnd();
void out_heuristica();
void escrever_resultado_em_arquivo(solucao s1, grafo_solucao g1, const char *nome_arquivo,const char * nome_instancia);
int main(){
   out_vnd();
   out_heuristica();
}

solucao criar_solucao(rota * rotas,clientes_tercerizado * clientes_t,int num_rotas,int custo_uso_v){
    solucao s1;
    s1.rotas = rotas;
    s1.clientes_tercerizado = clientes_t;
    s1.pesototal=0;
    s1.custor_rotear_v =0;
    s1.num_rotas = num_rotas;
    
    for(int i =0;i<num_rotas;i++){
        s1.pesototal+=rotas[i].pesototal;
    }


    s1.custor_rotear_v = custo_uso_v * num_rotas;

    s1.pesototal +=clientes_t->peso_toltal_cli_tercerizados;
    s1.custor_rotear_v =0;

    s1.custo_tercerizado = clientes_t->peso_toltal_cli_tercerizados;
    return s1;
}
int calcular_custo_total(solucao s1){
    int peso_total =0;
    for(int i =0;i<s1.num_rotas;i++){
        peso_total +=s1.rotas[i].pesototal;
    }

    return peso_total+s1.clientes_tercerizado->peso_toltal_cli_tercerizados;
}
void out_heuristica(){
        DIR *dp;
    struct dirent *ep;
    char pasta[] = "instancias/"; // Nome da pasta onde os arquivos estão

    dp = opendir(pasta);

    if (dp != NULL) {
        while (ep = readdir(dp)) {
            if (ep->d_type == DT_REG) { // Verifica se é um arquivo regular
                char nome_arquivo[100]; 
                snprintf(nome_arquivo, sizeof(nome_arquivo), "%s%s", pasta, ep->d_name);
                clock_t start_time_heuritico= clock(); 
                grafo_solucao grafo1 = carregar_grafo(nome_arquivo); // grafo usado para gerar as rotas
                grafo_solucao grafo2 = carregar_grafo(nome_arquivo);// grafo usado para fazer os movimento entre rotas
                rota * rotas = (rota*) malloc(grafo1.numero_veiculos*sizeof(rota)); // o numero de rotas se o numero de veiculos disponiveis    


                for(int i =0;i<grafo1.numero_veiculos;i++){
                    arestas_minimas *arestas_minimas;
                    arestas_minimas = caminho(grafo1);
                    rotas[i] = gerar_rotas(arestas_minimas,grafo1);
                    remover_clientes_visitados_grafo(grafo1,rotas[i]);
                }
                clientes_tercerizado c1 = verificar_clientes_a_tercerizar(rotas,&grafo2);
                clock_t end_time_heuritico = clock();
                solucao s1 = criar_solucao(rotas,&c1,grafo1.numero_veiculos,grafo1.custo_carro);
                escrever_resultado_em_arquivo(s1, grafo2, "resultado_heuristico.txt",nome_arquivo);
            }    
        }
    }
}

void out_vnd(){
    DIR *dp;
    struct dirent *ep;
    char pasta[] = "instancias/"; // Nome da pasta onde os arquivos estão

    dp = opendir(pasta);

    if (dp != NULL) {
        while (ep = readdir(dp)) {
            if (ep->d_type == DT_REG) { // Verifica se é um arquivo regular
                char nome_arquivo[100]; 
                snprintf(nome_arquivo, sizeof(nome_arquivo), "%s%s", pasta, ep->d_name);
                grafo_solucao grafo1 = carregar_grafo(nome_arquivo); // grafo usado para gerar as rotas
                grafo_solucao grafo2 = carregar_grafo(nome_arquivo);// grafo usado para fazer os movimento entre rotas
                rota * rotas = (rota*) malloc(grafo1.numero_veiculos*sizeof(rota)); // o numero de rotas se o numero de veiculos disponiveis    
        

                for(int i =0;i<grafo1.numero_veiculos;i++){
                    arestas_minimas *arestas_minimas;
                    arestas_minimas = caminho(grafo1);
                    rotas[i] = gerar_rotas(arestas_minimas,grafo1);
                    remover_clientes_visitados_grafo(grafo1,rotas[i]);
                }
                clientes_tercerizado c1 = verificar_clientes_a_tercerizar(rotas,&grafo2);
                clock_t start_time_vnd = clock(); 
                VND(rotas,grafo1.numero_veiculos,&grafo2,&c1);
                clock_t end_time = clock();
                solucao s1 = criar_solucao(rotas,&c1,grafo1.numero_veiculos,grafo1.custo_carro);
                escrever_resultado_em_arquivo(s1, grafo2, "resultado_vnd.txt",nome_arquivo);
            }    
        }
    }
}
void VND(rota *rotas, int num_rotas, grafo_solucao *grafo, clientes_tercerizado *clientes_tercerizados) {
    int peso_total_antigo;
    int peso_total_novo;
    bool houve_diferenca_peso = true; 

    while (houve_diferenca_peso) { // Correção: Consertar a condição da instrução "while".
        houve_diferenca_peso = false; // Definir como falso no início do loop.

        // Calcular o peso total antigo somando os pesos de todas as rotas
        peso_total_antigo = 0;
        for (int i = 0; i < num_rotas; i++) {
            peso_total_antigo += rotas[i].pesototal;
        }

        // Aplicar um movimento em cada rota
        for (int i = 0; i < num_rotas; i++) {
            rotas[i] = movimento_em_unica_rota(&rotas[i], grafo);
        }

        // Calcular o peso total novo somando os pesos de todas as rotas
        peso_total_novo = 0;
        for (int i = 0; i < num_rotas; i++) {
            peso_total_novo += rotas[i].pesototal;
        }

        // Verificar se houve melhoria no peso total
        if (peso_total_novo < peso_total_antigo) {
            houve_diferenca_peso = true;
        }

        // Trocar clientes entre rotas
        troca_clientes_entre_rotas(rotas, grafo->numero_veiculos, grafo);

        // Atualizar o peso total antigo com o novo
        peso_total_antigo = peso_total_novo;

        // Calcular o peso total novo novamente somando os pesos de todas as rotas
        peso_total_novo = 0;
        for (int i = 0; i < num_rotas; i++) {
            peso_total_novo += rotas[i].pesototal;
        }

        // Verificar se houve melhoria no peso total novamente
        if (peso_total_novo < peso_total_antigo) {
            houve_diferenca_peso = true;
        }

        movimento_entres_rotas_e_tercerizado(rotas,grafo->numero_veiculos,clientes_tercerizados,clientes_tercerizados->total_de_clientes_ter,grafo);
        // Atualizar o peso total antigo com o novo
        peso_total_antigo = peso_total_novo;

        // Calcular o peso total novo novamente somando os pesos de todas as rotas
        peso_total_novo = 0;
        for (int i = 0; i < num_rotas; i++) {
            peso_total_novo += rotas[i].pesototal;
        }

        // Verificar se houve melhoria no peso total novamente
        if (peso_total_novo < peso_total_antigo) {
            houve_diferenca_peso = true;
        }
    }
}

void mostrar_resultado(solucao s1,grafo_solucao g1){
    printf("%d",s1.pesototal);
    printf("\n%d",s1.custor_rotear_v - g1.numero_veiculos*g1.custo_carro);
    printf("\n%d",g1.numero_veiculos*g1.custo_carro);
    printf("\n%d",s1.custo_tercerizado);
    printf("\n\n");

    for(int i=0;i<s1.clientes_tercerizado->total_de_clientes_ter;i++){
        printf("%d ",s1.clientes_tercerizado->clientes_tercerizado[i]);
    }

    printf("\n\n%d\n",g1.numero_veiculos);

    for(int i=0;i<g1.numero_veiculos;i++){
        for(int j=0;j<s1.rotas[i].total_de_clientes_visitado;j++){
            printf("%d ",s1.rotas[i].clientes_visitados[j]);
        }
        printf("\n");
    }

}

void escrever_resultado_em_arquivo(solucao s1, grafo_solucao g1, const char *nome_arquivo, const char *nome_instancia) {
    char caminho_arquivo[100]; // Defina o tamanho apropriado
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "resultados/%s", nome_arquivo);

    FILE *arquivo = fopen(caminho_arquivo, "a"); // Abra o arquivo em modo de adição
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        return;
    }

    // Adicione o nome da instância no início do arquivo
    fprintf(arquivo, "%s\n", nome_instancia);

    fprintf(arquivo, "%d\n", s1.pesototal);
    fprintf(arquivo, "%d\n", s1.pesototal - g1.numero_veiculos * g1.custo_carro - s1.custo_tercerizado);
    fprintf(arquivo, "%d\n", g1.numero_veiculos * g1.custo_carro);
    fprintf(arquivo, "%d\n\n", s1.custo_tercerizado);

    for (int i = 0; i < s1.clientes_tercerizado->total_de_clientes_ter; i++) {
        fprintf(arquivo, "%d ", s1.clientes_tercerizado->clientes_tercerizado[i]);
    }
    fprintf(arquivo, "\n\n%d\n", g1.numero_veiculos);

    for (int i = 0; i < g1.numero_veiculos; i++) {
        for (int j = 0; j < s1.rotas[i].total_de_clientes_visitado; j++) {
            fprintf(arquivo, "%d ", s1.rotas[i].clientes_visitados[j]);
        }
        fprintf(arquivo, "\n");
    }
    fprintf(arquivo, "\n");
    fprintf(arquivo, "\n");


    fclose(arquivo);
}