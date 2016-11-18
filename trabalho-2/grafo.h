#ifndef _GRAFO_H
#define _GRAFO_H

#include <stdio.h>

//------------------------------------------------------------------------------
// o valor representando "infinito"

const long int infinito;

//-----------------------------------------------------------------------------
// (apontador para) lista encadeada

typedef struct lista *lista;

//-----------------------------------------------------------------------------
// (apontador para) nó da lista encadeada cujo conteúdo é um void *

typedef struct no *no;

//------------------------------------------------------------------------------
// devolve o número de nós da lista l

unsigned int tamanho_lista(lista l);

//------------------------------------------------------------------------------
// devolve o primeiro nó da lista l,
//      ou NULL, se l é vazia
no primeiro_no(lista l); //------------------------------------------------------------------------------
// devolve o sucessor do nó n,
//      ou NULL, se n for o último nó da lista

no proximo_no(no n);

//------------------------------------------------------------------------------
// devolve o conteúdo do nó n
//      ou NULL se n == NULL

void *conteudo(no n);

//------------------------------------------------------------------------------
// cria uma lista vazia e a devolve
//
// devolve NULL em caso de falha

lista constroi_lista(void);

//------------------------------------------------------------------------------
// desaloca a lista l e todos os seus nós
//
// se destroi != NULL invoca
//
//     destroi(conteudo(n))
//
// para cada nó n da lista.
//
// devolve 1 em caso de sucesso,
//      ou 0 em caso de falha

int destroi_lista(lista l, int destroi(void *));

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
//
// o grafo pode ser
// - direcionado ou não
// - com pesos nas arestas/arcos ou não
//
// além dos vértices e arestas, o grafo tem um nome, que é uma "string"
//
// num grafo com pesos todas as arestas/arcos tem peso, que é um long int
//
// o peso default é 0

typedef struct grafo *grafo;

//------------------------------------------------------------------------------
// devolve o nome do grafo g

char *nome_grafo(grafo g);

//------------------------------------------------------------------------------
// devolve 1, se g é direcionado,
//      ou 0, caso contrário

int direcionado(grafo g);

//------------------------------------------------------------------------------
// devolve 1, se g tem pesos nas arestas/arcos,
//      ou 0, caso contrário

int ponderado(grafo g);

//------------------------------------------------------------------------------
// devolve o número de vértices do grafo g

unsigned int numero_vertices(grafo g);

//------------------------------------------------------------------------------
// devolve o número de arestas/arcos do grafo g

unsigned int numero_arestas(grafo g);

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados que representa um vértice do grafo
//
// cada vértice tem um nome que é uma "string"

typedef struct vertice *vertice;

//------------------------------------------------------------------------------
// devolve o nome do vertice v

char *nome_vertice(vertice v);

//------------------------------------------------------------------------------
// devolve o vertice de nome s no grafo g,
//      ou NULL caso não exista em g um vertice de nome s

vertice vertice_nome(char *s, grafo g);

//------------------------------------------------------------------------------
// lê um grafo no formato dot de input, usando as rotinas de libcgraph
//
// desconsidera todos os atributos do grafo lido exceto o atributo
// "peso" quando ocorrer; neste caso o valor do atributo é o peso da
// aresta/arco que é um long int
//
// num grafo com pesos todas as arestas/arcos tem peso
//
// o peso default é 0
//
// todas as estruturas de dados alocadas pela libcgraph são
// desalocadas ao final da execução
//
// devolve o grafo lido
//      ou NULL em caso de erro

grafo le_grafo(FILE *input);

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
//
// devolve 1 em caso de sucesso,
//      ou 0 caso contrário

int destroi_grafo(grafo g);

//------------------------------------------------------------------------------
// escreve o grafo g em output usando o formato dot.
//
// o peso das arestas/arcos (quando houver) é escrito como o atributo
// de nome "peso"
//
// devolve o grafo escrito
//      ou NULL em caso de erro

grafo escreve_grafo(FILE *output, grafo g);

//------------------------------------------------------------------------------
// devolve o grau de v no grafo g, se não é direcionado ou se direcao == 0,
//      ou o grau de entrada de v no grafo g, se direcao == -1,
//      ou o grau de saída de v no grafo g, se direcao == +1

unsigned int grau(vertice v, int direcao, grafo g);

//------------------------------------------------------------------------------
// devolve uma lista de vértices de g representando o caminho mínimo
// de u a v em g
//
// a lista é vazia se u e v estão em componentes diferentes de g

lista caminho_minimo(vertice u, vertice v, grafo g);

//------------------------------------------------------------------------------
// devolve a distância de u a v em g

long int distancia(vertice u, vertice v, grafo g);

//------------------------------------------------------------------------------
// devolve um número entre 0 e numero_vertices(g)
//
// este número é único e distinto para cada vértice de g e deve servir
// para indexar vetores e matrizes a partir dos vértices de g

unsigned int indice(vertice v, grafo g);

//------------------------------------------------------------------------------
// preenche a matriz d com as distâncias entre os vértices de g de
// maneira que d[indice(u,g)][indice(v,g)] tenha o valor da distância
// entre os vértices u e v em g
//
// devolve d

long int **distancias(unsigned int **d, grafo g);

//------------------------------------------------------------------------------
// preenche a matriz c com caminhos mínimos entre os vértices de g de
// maneira que c[indice(u,g)][indice(v,g)] tenha um caminho mínimo
// entre os vértices u e v em g
//
// devolve d

lista **caminhos_minimos(lista **c, grafo g);

//------------------------------------------------------------------------------
// devolve o diâmetro de g

long int diametro(grafo g);

#endif

#ifdef TESTE

vertice * vizinhos(vertice v);
int vizinho(vertice v1, vertice v2);

#endif


