#include <graphviz/cgraph.h>
#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct grafo{
	char nome[100]; 
	unsigned int nv;
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

void inicia_vertices(grafo grf,Agraph_t *g){
	/* Determina o numero de vertices */
	for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v)){
		grf->nv++;
	}
}

grafo inicia_grafo(Agraph_t *g){
	if ( !g ) return NULL;
		grafo grf = (grafo)malloc(sizeof(grafo));
	if(!grf){
		printf("Impossivel alocar memoria para grafo");
		return NULL;
	}
	strcpy(grf->nome,agnameof(g));
	inicia_vertices(grf,g);
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

unsigned int numero_vertices(grafo g){
	return g->nv;
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
