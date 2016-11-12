#include <graphviz/cgraph.h>
#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct grafo{
    char nome[100]; 
};

struct lista{
};

// Ref a grafo

grafo inicia_grafo(Agraph_t *g){
    if ( !g ) return NULL;
    grafo grf = (grafo)malloc(sizeof(grafo));
    if(!grf){
	printf("Impossivel alocar memoria para grafo");
	return NULL;
    }
    strcpy(grf->nome,agnameof(g));
    return grf;
}

grafo le_grafo(FILE *input){
    if (input == 0){
	printf("le_grafo: Entrada invalida\n");
        return 0;
    }
    Agraph_t *g = agread(input, NULL);
    if ( !g ){
	printf("Impossivel ler grafo de arquivo\n");
        return 0;
    }
    grafo graph = inicia_grafo(g);
    if(graph == NULL){
	printf("Impossivel inicializar o grafo\n");
    }
    agclose(g);
    return graph;
}  

char *nome_grafo(grafo g){
    return g->nome;   
}

// Ref a lista

lista constroi_lista(void){
	lista l = (lista) malloc( sizeof(lista));
	return l;
}
