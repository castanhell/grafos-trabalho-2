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
    unsigned int direcionado;
    unsigned int ponderado;
};

struct vertice
{
    vertice* vizinhos;
    char *nome;
    int id;
    unsigned int grauEntrada;
    unsigned int grauSaida;
};

struct aresta
{
    vertice origem;
    vertice destino;
    int peso;
};

struct lista
{
    int nNos;
    no primeiroNo;
    no ultimoNo;
};

struct no
{
    no proximo;
    void *conteudo;
};

// Ref a lista

lista constroi_lista(void)
{
    lista l = (lista) malloc( sizeof(lista) );
    if (l == NULL)
    {
        printf("Unable to allocate memory\n");
    }
    l->primeiroNo=NULL;
    l->ultimoNo=NULL;
    l->nNos=0;
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

no proximo_no(no n)
{
    return n->proximo;
}

int adiciona_lista(lista l, void* conteudo)
{
    no n = (no) malloc(sizeof(struct no) );
    n->proximo=NULL;
    n->conteudo=conteudo;
    if(!l)
    {
        return 0;
    }

    if(l->ultimoNo)
    {
        l->ultimoNo->proximo=n;
        l->ultimoNo=n;
    }
    else
    {
        l->primeiroNo=n;
        l->ultimoNo=n;
    }
    (l->nNos)++;
    return 1;
}

unsigned int tamanho_lista(lista l)
{
    return l->nNos;
}

void * conteudo(no n){
    return n->conteudo;
}

// Ref a grafo

char *cpy_char(char * org)
{
    char *cpy = malloc(strlen(org)*sizeof(char));
    if(cpy==NULL)
    {
        return NULL;
    }
    strcpy(cpy,org);
    return cpy;
}

void le_direcionado(Agraph_t *g, grafo graph)
{
    if(agisdirected(g))
    {
        graph->direcionado=1;
    }
    else
    {
        graph->direcionado=0;
    }
}

void le_nome(Agraph_t *g, grafo graph)
{
    char * nome = cpy_char(agnameof(g));
    graph->nome=nome;
}

int estado_ponderado_valido(int ponderado)
{
    return ponderado == 0 || ponderado == 1;
}

void insere_ponderado(grafo graph, int ponderado)
{
    if(estado_ponderado_valido(graph->ponderado) && ponderado!=graph->ponderado)
    {
        graph->ponderado=0;
    }
    else
    {
        graph->ponderado=ponderado;
    }
}

void inicia_vertice(vertice vertice)
{
    vertice->grauEntrada=0;
    vertice->grauSaida=0;
}

void insere_pesos(Agraph_t *g, grafo graph, Agedge_t *a, Agnode_t *v)
{
    char *peso = agget(a, (char *)"peso");
    if(peso==NULL || strlen(peso)==0)
    {
        insere_ponderado(graph,0);
    }
    else
    {
        insere_ponderado(graph,1);
    }
}

void insere_graus_vertices(vertice vertice, grafo graph, Agedge_t *a, Agnode_t *v)
{
    if (v == agtail(a))
    {
        ++(graph->na);
        ++(vertice->grauSaida);
    }
    if(v == aghead(a))
    {
        ++(vertice->grauEntrada);
    }
}

void insere_nome_vertice(vertice vertice, char*nome)
{
    vertice->nome=cpy_char(nome);
}

void preencheVertices(Agraph_t *g, grafo graph)
{
    vertice vertices = malloc( (graph->nv)*sizeof(struct vertice) );
    int i = 0;
    for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v))
    {
        inicia_vertice(vertices + i );
        for (Agedge_t *a=agfstedge(g,v); a; a=agnxtedge(g,a,v))
        {
            insere_pesos(g,graph,a,v);
            insere_graus_vertices(vertices+i,graph,a,v);
        }
        insere_nome_vertice(vertices+i,agnameof(v));
        i++;
    }
    graph->vertices=vertices;
}

void preenche_vizinhos_vertice(Agraph_t *g, grafo graph)
{
    for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v))
    {
        vertice vv = vertice_nome(agnameof(v),graph);
        int gvv = grau(vv,0,g);
        vertice * vzv = malloc( sizeof(vertice) * gvv );
        int i = 0;
        for (Agedge_t *a=agfstedge(g,v); a; a=agnxtedge(g,a,v))
        {
            if(v == aghead(a))
            {
                vzv[i] = vertice_nome(agnameof(agtail(a)),graph);
            }
            if (v == agtail(a))
            {
                vzv[i] = vertice_nome(agnameof(aghead(a)),graph);
            }
            ++i;
        }
        vv->vizinhos=vzv;
    }
}

void le_vertices_arestas(Agraph_t *g, grafo graph)
{
    graph->nv=0;
    graph->na=0;
    for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v))
    {
        ++(graph->nv);
    }
    preencheVertices(g,graph);
    preenche_vizinhos_vertice(g,graph);
}

/*
Inicia variáveis que iniciam de 0 a 1 como 2, ou seja, um valor não válido
*/
void inicia_grafo(grafo graph)
{
    graph->direcionado=2;
    graph->ponderado= 2;
}

grafo le_grafo(FILE *input)
{
    grafo graph = (grafo)malloc(sizeof(struct grafo));
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
    inicia_grafo(graph);
    le_nome(g,graph);
    le_direcionado(g,graph);
    le_vertices_arestas(g,graph);
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

int destroi_vertices(grafo g)
{
    for(int i = 0; i < g->nv; i++)
    {
        free(g->vertices[i].nome);
    }
    free(g->vertices);
}

int destroi_grafo(grafo g)
{
    destroi_vertices(g);
    free(g->nome);
    free(g);
    g=NULL;
    return 1;
}

int direcionado(grafo g)
{
    return g->direcionado;
}

int ponderado(grafo g)
{
    return g->ponderado;
}

// Ref a vértice

char * nome_vertice(vertice v)
{
    return v->nome;
}

vertice vertice_nome(char* s, grafo g)
{
    for(int i = 0; i < g->nv; i++)
    {
        if( strcmp( g->vertices[i].nome, s) == 0 )
        {
            return (g->vertices) + i;
        }
    }
}

unsigned int grauDirecionado(vertice v, int direcao)
{
    if(direcao==-1)
    {
        return v->grauEntrada;
    }
    if(direcao==0)
    {
        return v->grauEntrada+v->grauSaida;
    }
    if(direcao==1)
    {
        return v->grauSaida;
    }
}

unsigned int grauNaoDirecionado(vertice v)
{
    return (v->grauEntrada)+(v->grauSaida);
}

unsigned int grau(vertice v, int direcao, grafo g)
{
    if(direcionado(g))
    {
        return grauDirecionado(v,direcao);
    }
    else
    {
        return grauNaoDirecionado(v);
    }
}

/*
Retorna os vizinhos de um dado vertice
*/
vertice * vizinhos(vertice v)
{
    return NULL;
}

/*
Retorna 1 se v1 é vizinho de v2 ou NULL, caso contrário
*/
int vizinho(vertice v1, vertice v2)
{
    for(int i = 0; i < v1->grauEntrada+v1->grauSaida; i++)
    {
        if( v1->vizinhos[i] == v2 )
        {
            return 1;
        }
    }
    for(int i = 0; i < v2->grauEntrada+v2->grauSaida; i++)
    {
        if( v2->vizinhos[i] == v1 )
        {
            return 1;
        }
    }
    return 0;
}
