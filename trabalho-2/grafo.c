#include <graphviz/cgraph.h>
#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const long int infinito= 1 << 30;

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
    lista arestas;
    char *nome;
    int id;
    unsigned int grauEntrada;
    unsigned int grauSaida;
};

typedef struct aresta
{
    vertice origem;
    vertice destino;
    int peso;
} aresta;

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

typedef struct veio_de
{
    vertice v;
    int id_vertice_veio_de;
    long int distancia;
    int processado;
} veio_de;

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
    if(l == NULL)
    {
        return 0;
    }
    else
    {
        no p = primeiro_no(l);
        while(p != NULL)
        {
            no cpy = p;
            p = proximo_no(p);
            void *vp = conteudo(cpy);
            destroi(&vp);
        }
        free(l);
        return 1;
    }
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

void * conteudo(no n)
{
    if(n)
    {
        return n->conteudo;
    }
    else
    {
        return NULL;
    }
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
    vertice->arestas=NULL;
}

void insere_pesos(Agraph_t *g, grafo graph, Agedge_t *a, Agnode_t *v, aresta* ar)
{
    char *peso = agget(a, (char *)"peso");
    if(peso==NULL || strlen(peso)==0)
    {
        insere_ponderado(graph,0);
        ar->peso=1;
    }
    else
    {
        insere_ponderado(graph,1);
        ar->peso=atoi(peso);
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

void insere_id_vertice(vertice vertice, int id)
{
    vertice->id=id;
}

void preencheVertices(Agraph_t *g, grafo graph)
{
    vertice vertices = malloc( (graph->nv)*sizeof(struct vertice) );
    int i = 0;
    for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v))
    {
        inicia_vertice(vertices + i);
        for (Agedge_t *a=agfstedge(g,v); a; a=agnxtedge(g,a,v))
        {
            insere_graus_vertices(vertices+i,graph,a,v);
        }
        insere_nome_vertice(vertices+i,agnameof(v));
        insere_id_vertice(vertices+i,i);
        i++;
    }
    graph->vertices=vertices;
}

void insere_aresta_vertice(vertice v, aresta* ar)
{
    if(v->arestas==NULL)
    {
        v->arestas=constroi_lista();
    }
    adiciona_lista(v->arestas,ar);
}

void preenche_vizinhos_vertice(Agraph_t *g, grafo graph)
{
    for (Agnode_t *v=agfstnode(g); v; v=agnxtnode(g,v))
    {
        vertice v1 = vertice_nome(agnameof(v),graph);
        int gvv = grau(v1,0,g);
        int i = 0;
        for (Agedge_t *a=agfstedge(g,v); a; a=agnxtedge(g,a,v))
        {
            aresta * ar = malloc(sizeof(struct aresta));

            if(v == aghead(a))
            {
                vertice v2 = vertice_nome(agnameof(agtail(a)),graph);
                ar->origem=v2;
                ar->destino=v1;
                if(direcionado(graph))
                {
                    insere_aresta_vertice(v2,ar);
                }
                if(!direcionado(graph))
                {
                    aresta * arcpy = malloc(sizeof(struct aresta));
                    arcpy->origem = ar->origem;
                    arcpy->destino=ar->destino;
                    arcpy->peso=ar->peso;
                    insere_aresta_vertice(v1,ar);
                    insere_aresta_vertice(v2,arcpy);
                    insere_pesos(g,graph,a,v,arcpy);
                }
            }
            if (v == agtail(a))
            {
                vertice v2 = vertice_nome(agnameof(aghead(a)),graph);
                ar->origem=v1;
                ar->destino=v2;
                if(direcionado(graph))
                {
                    insere_aresta_vertice(v1,ar);
                }
                if(!direcionado(graph))
                {
                    aresta * arcpy = malloc(sizeof(struct aresta));
                    arcpy->origem = ar->origem;
                    arcpy->destino=ar->destino;
                    arcpy->peso=ar->peso;
                    insere_aresta_vertice(v1,ar);
                    insere_aresta_vertice(v2,arcpy);
                    insere_pesos(g,graph,a,v,arcpy);
                }
            }
            insere_pesos(g,graph,a,v,ar);
            ++i;
        }
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

int destroi_aresta(aresta ** ar)
{
    if(ar == NULL)
    {
        return 0;
    }
    else
    {
        free(*ar);
        return 1;
    }
}

int destroi_vertices(grafo g)
{
    for(int i = 0; i < g->nv; i++)
    {
        free(g->vertices[i].nome);
        destroi_lista(g->vertices[i].arestas, destroi_aresta);
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

vertice vertice_id(int id, grafo grf)
{
    return grf->vertices+id;
}

vertice cpy_vertice(vertice org, vertice dst)
{
    dst->arestas=NULL;
    dst->grauEntrada=org->id;
    dst->grauSaida=org->id;
    dst->id=org->id;
    dst->nome=cpy_char(org->nome);
}

int destroi_vertice_dummy(vertice v)
{
    free(v->nome);
    free(v);
    return 1;
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

unsigned int indice(vertice v, grafo g)
{
    return v->id;
}

/*
Retorna 1 se v1 é vizinho de v2 ou NULL, caso contrário
*/
int vizinho(vertice v1, vertice v2)
{
    no n = v1->arestas->primeiroNo;
    while(n != NULL)
    {
        aresta * ar = (aresta*) n->conteudo;
        if(ar->destino == v2)
        {
            return 1;
        }
        if(ar->origem==v2)
        {
            return 1;
        }
        n = n->proximo;
    }
    return 0;
}

int vizinho_peso(vertice v1, vertice v2)
{
    no n = v1->arestas->primeiroNo;
    while(n != NULL)
    {
        aresta * ar = (aresta*) n->conteudo;
        if(ar->destino == v2)
        {
            return ar->peso;
        }
        if(ar->origem==v2)
        {
            return ar->peso;
        }
        n = n->proximo;
    }
    return 0;
}

int encontra_menor_id_distancia(veio_de* veio_de,grafo g)
{
    int id_de = -1;
    long int menor_dist = infinito;
    for(int i = 0; i < numero_vertices(g); i++)
    {
        if(!veio_de[i].processado && veio_de[i].distancia <= menor_dist)
        {
            id_de = i;
            menor_dist=veio_de[i].distancia;
        }
    }
    return id_de;
}

vertice vertice_vizinho(vertice v, aresta* ar)
{
    if(ar->origem==v)
    {
        return ar->destino;
    }
    if(ar->destino==v)
    {
        return ar->origem;
    }
}

/* Algoritmos de distancia */
void constroi_veio_de_dijkstra(veio_de * veio_de, vertice origem, grafo g)
{
    //Inicia vertices
    for(int i = 0; i < numero_vertices(g); i++)
    {
        veio_de[i].v=g->vertices+i;
        veio_de[i].distancia=infinito;
        veio_de[i].id_vertice_veio_de=-1;
        veio_de[i].processado=0;
    }
    veio_de[origem->id].distancia = 0;
    //Executa o algoritmo
    int processados = 0;
    while(processados < numero_vertices(g))
    {
        int id_menor = encontra_menor_id_distancia(veio_de,g);
        vertice menor = g->vertices + id_menor;
        //para cada vertice adjacente ao menor
        lista arestas = menor->arestas;
        if(arestas)
        {
            no no_aresta = primeiro_no(arestas);
            while(no_aresta != NULL)
            {
                aresta *ar = conteudo(no_aresta);
                vertice vizinho = vertice_vizinho(menor,ar);
                if(veio_de[vizinho->id].distancia > veio_de[menor->id].distancia + ar->peso  )
                {
                    veio_de[vizinho->id].distancia = veio_de[menor->id].distancia + ar->peso;
                    veio_de[vizinho->id].id_vertice_veio_de = menor->id;
                }
                no_aresta = no_aresta->proximo;
            }
        }
        //processamos o menor
        veio_de[id_menor].processado=1;
        processados++;
    }

    return veio_de;
}

int destroi_veio_de(veio_de **vd)
{
    free(*vd);
    return 1;
}

veio_de* veio_de_id(veio_de* referencia, int id)
{
    return referencia + id;
}

lista constroi_lista_minimo(vertice u, vertice v, veio_de* vd,grafo g)
{
    veio_de *vertice_destino = veio_de_id(vd,indice(v,g));
    lista l = constroi_lista();
    while(vertice_destino->v != u)
    {
        if(vertice_destino->id_vertice_veio_de == -1)
        {
            destroi_lista(l,destroi_vertice_dummy);
            l=NULL;
            break;
        }

        vertice dummycpy = malloc(sizeof(struct vertice));
        cpy_vertice(vertice_id(vertice_destino->id_vertice_veio_de,g),dummycpy);
        adiciona_lista(l,dummycpy);
        //pula para o qual veio
        vertice_destino = veio_de_id(vd,vertice_destino->id_vertice_veio_de);
    }
    adiciona_lista(l,u);
    destroi_veio_de(&vd);
    return l;
}

lista caminho_minimo(vertice u, vertice v, grafo g)
{
    veio_de *vd = malloc(sizeof(veio_de) * g->nv);
    constroi_veio_de_dijkstra(vd,u,g);
    return constroi_lista_minimo(u, v,vd, g);
}

long int diametro(grafo g)
{
    long int diametro = 0;
    veio_de *vd = malloc(sizeof(veio_de) * g->nv);
    long int dist;

    for(int i = 0; i < numero_vertices(g); i++)
    {
        constroi_veio_de_dijkstra(vd,vertice_id(i,g),g);
        for(int j = 0; j < numero_vertices(g); j++)
        {
            veio_de *vdptr = (vd + j);
            dist = vdptr->distancia;
            if(dist != infinito && diametro < dist)
            {
                diametro = dist;
            }
        }
    }
    destroi_veio_de(&vd);
    return diametro;
}

long int distancia(vertice u, vertice v, grafo g)
{
    veio_de *vd = malloc(sizeof(veio_de) * g->nv);
    constroi_veio_de_dijkstra(vd,u,g);
    int valor = (vd + indice(v,g))->distancia;
    destroi_veio_de(&vd);
    return valor;
}

long int **distancias_dijkstra(long int **d, grafo g)
{

    veio_de *vd = malloc(sizeof(veio_de) * numero_vertices(g));

    for(int i = 0; i < numero_vertices(g); i++)
    {
        constroi_veio_de_dijkstra(vd,vertice_id(i,g),g);
        for(int j = 0; j < numero_vertices(g); j++)
        {
            veio_de *vdptr = (vd+j);
            d[i][j] = vdptr->distancia;
        }
    }
    destroi_veio_de(&vd);
    return d;
}

long int **distancias_fw(long int **d, grafo g){
    for(int i = 0; i < numero_vertices(g); i++){
        for(int j = 0; j < numero_vertices(g); j++){
            if(i==j)
                d[i][j] = 0;
            else
                d[i][j] = infinito;
        }
        vertice v = vertice_id(i,g);
        lista ars = v->arestas;
        no ar = primeiro_no(ars);
        while(ar != NULL){
            aresta * a = (aresta*) ar->conteudo;
            d[i][a->destino->id] = a->peso;
            ar = ar->proximo;
        }
    }

    for(int k = 0; k < numero_vertices(g); k++){
            for(int i = 0; i < numero_vertices(g); i++){
                    for(int j = 0; j < numero_vertices(g); j++){
                        if(d[i][j] > d[i][k] + d[k][j]){
                            d[i][j] = d[i][k] + d[k][j];
                        }
                    }
            }
    }

    return d;
}

long int **distancias(long int **d, grafo g, char algoritmo){
    if(algoritmo=='d'){
        return distancias_dijkstra(d,g);
    }
    else{
        return distancias_fw(d,g);
    }
}

lista **caminhos_minimos(lista **c, grafo g, char algoritmo)
{
    veio_de *vd = malloc(sizeof(veio_de) * g->nv);
    for(int i = 0; i < numero_vertices(g); i++)
    {
        constroi_veio_de_dijkstra(vd,vertice_id(i,g),g);
        for(int j = 0; j < numero_vertices(g); j++)
        {
            c[i][j]=constroi_lista_minimo( vertice_id(i,g), vertice_id(j,g),vd, g);
        }
    }
    destroi_veio_de(&vd);
    return c;
}
