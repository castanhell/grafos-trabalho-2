#include <graphviz/cgraph.h>
#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct grafo{
    char *nome;
    unsigned int nv;
	unsigned int na;
};

struct lista{
	int nNos;
	no primeiroNo;
};

struct no {
	no proximo;
	void *conteudo;
};

// Ref a grafo

void le_nome(Agraph_t *g, grafo graph){
    int ncpy= strlen(agnameof(g)) < 100 ? strlen(agnameof(g)) : 100;
    char *nome = malloc(100*sizeof(char));
    if(nome==NULL){ return NULL; }
	strncpy(nome,agnameof(g),ncpy);
	graph->nome=nome;
}

void le_vertices(Agraph_t *g, grafo graph){
    graph->nv=0;
	//inicia vertices
	for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v)){
		++(graph->nv);
	}
}

void le_arestas(Agraph_t *g, grafo graph){
    graph->nv=0;
	//inicia vertices
	for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v)){
		++(graph->nv);
	}
}

grafo le_grafo(FILE *input){
	grafo graph = (grafo)malloc(sizeof(grafo));
	if(!graph){
		printf("Impossivel alocar memoria para grafo");
		return NULL;
	}
	if (input == 0){
		printf("le_grafo: Entrada invalida\n");
		return 0;
	}
	Agraph_t *g = agread(input, NULL);
	if ( !g ){
		printf("Impossivel ler grafo de arquivo\n");
		return 0;
	}
    le_nome(g,graph);
    le_vertices(g,graph);
	agclose(g);
	return graph;
}

char *nome_grafo(grafo g){
	return g->nome;
}

unsigned int numero_vertices(grafo g){
	return g->nv;
}

unsigned int numero_arestas(grafo g){
	return g->na;
}

// Ref a lista

lista constroi_lista(void){
	lista l = (lista) malloc( sizeof(lista) );
	if (l == NULL){
		printf("Unable to allocate memory\n");
	}
	l->primeiroNo=NULL;
	return l;
}

int destroi_lista(lista l, int destroi(void *)){
	free(l);
	return 1;
}

no primeiro_no(lista l){
	return l->primeiroNo;
}
