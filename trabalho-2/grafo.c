#include <graphviz/cgraph.h>
#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct grafo
{
    char *nome;
    unsigned int nv;
    unsigned int na;
    vertice vertices;
};

struct vertice{
    char *nome;
    int id;
};

struct lista
{
    int nNos;
    no primeiroNo;
};

struct no
{
    no proximo;
    void *conteudo;
};

// Ref a grafo

char *cpyChar(char * org){
    char *cpy = malloc(strlen(org)*sizeof(char));
    if(cpy==NULL)
    {
        return NULL;
    }
    strcpy(cpy,org);
    return cpy;
}

void le_nome(Agraph_t *g, grafo graph)
{
    char * nome = cpyChar(agnameof(g));
    graph->nome=nome;
}

void preencheVertices(Agraph_t *g, grafo graph){
    vertice vertices = malloc( (graph->nv)*sizeof(struct vertice) );
    int i = 0;
    for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v))
    {
        vertices[i++].nome=cpyChar(agnameof(v));
    }
    graph->vertices=vertices;
}

void le_vertices(Agraph_t *g, grafo graph)
{
    graph->nv=0;
    for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v))
    {
        ++(graph->nv);
    }
    preencheVertices(g,graph);
}

void le_arestas(Agraph_t *g, grafo graph)
{
    graph->na=0;
    for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v))
    {
        for (Agedge_t *a=agfstedge(g,v); a; a=agnxtedge(g,a,v))
        {
            if (v == agtail(a))
            {
                graph->na++;
            }
        }
    }

}

grafo le_grafo(FILE *input)
{
    grafo graph = (grafo)malloc(sizeof(grafo));
    if(!graph)
    {
        printf("Impossivel alocar memoria para grafo");
        return NULL;
    }
    if (input == 0)
    {
        printf("le_grafo: Entrada invalida\n");
        return 0;
    }
    Agraph_t *g = agread(input, NULL);
    if ( !g )
    {
        printf("Impossivel ler grafo de arquivo\n");
        return 0;
    }
    le_nome(g,graph);
    le_vertices(g,graph);
    le_arestas(g,graph);
    agclose(g);
    return graph;
}

char *nome_grafo(grafo g)
{
    return g->nome;
}

unsigned int numero_vertices(grafo g)
{
    return g->nv;
}

unsigned int numero_arestas(grafo g)
{
    return g->na;
}

// Ref a vértice

char * nome_vertice(vertice v){
    return v->nome;
}

vertice vertice_nome(char* s, grafo g){
    for(int i = 0; i < g->nv; i++){
        if( strcmp( g->vertices[i].nome, s) == 0 ){
            return (g->vertices) + i;
        }
    }
}

int destroi_vertices(grafo g){
    for(int i = 0; i < g->nv; i++){
        free(g->vertices[i].nome);
    }
    free(g->vertices);
}

int destroi_grafo(grafo g){
    destroi_vertices(g);
    free(g->nome);
    free(g);
    g=NULL;
    return 1;
}

// Ref a lista

lista constroi_lista(void)
{
    lista l = (lista) malloc( sizeof(lista) );
    if (l == NULL)
    {
        printf("Unable to allocate memory\n");
    }
    l->primeiroNo=NULL;
    return l;
}

int destroi_lista(lista l, int destroi(void *))
{
    free(l);
    return 1;
}

no primeiro_no(lista l)
{
    return l->primeiroNo;
}
