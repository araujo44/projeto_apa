#include "grafo_func.h"
#include "rotas_func.h"
#include "algoritmo_guloso.h"

typedef struct solucao
{
    int pesototal; // pesotal de todas as rotas
    rota * rotas; // todas as rotas
    clientes_tercerizado * clientes_tercerizado; 
    int custor_rotear_v;
    int custo_tercerizado;
}solucao;
void mostrar_resultado(solucao s1,grafo_solucao g1);
solucao criar_solucao(rota * rotas,clientes_tercerizado * clientes_t,int num_rotas);
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
        rotas[i] = movimento_em_unica_rota(&rotas[i],&grafo2);
        
    }
    //troca_clientes_entre_rotas(rotas,grafo1.numero_veiculos,&grafo2);

    clientes_tercerizado c1 = verificar_clientes_a_tercerizar(rotas,&grafo1);
    //printf("%d",c1.peso_toltal_cli_tercerizados);
    
    movimento_entres_rotas_e_tercerizado(rotas,grafo1.numero_veiculos,&c1,c1.total_de_clientes_ter,&grafo2);
    //printf("\n%d",c1.peso_toltal_cli_tercerizados);
    solucao s1 =  criar_solucao(rotas,&c1,grafo1.numero_veiculos);
    //printf("%d\n",s1.pesototal);
    mostrar_resultado(s1,grafo2);
}

solucao criar_solucao(rota * rotas,clientes_tercerizado * clientes_t,int num_rotas){
    solucao s1;
    s1.rotas = rotas;
    s1.clientes_tercerizado = clientes_t;
    s1.pesototal=0;
    s1.custor_rotear_v =0;

    for(int i =0;i<num_rotas;i++){
        s1.pesototal+=rotas[i].pesototal;
    }
    s1.custor_rotear_v = s1.pesototal;
    s1.pesototal +=clientes_t->peso_toltal_cli_tercerizados;
    s1.custo_tercerizado = clientes_t->peso_toltal_cli_tercerizados;
    //printf("\n%d\n",s1.clientes_tercerizado->total_de_clientes_ter);
    return s1;
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