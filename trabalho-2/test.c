#include <assert.h>
#include <graphviz/cgraph.h>
#include "grafo.h"
#include "minunit.h"
#include "parameters.h"
#include <stdio.h>
#include <stdlib.h>

char msg[100];
int tests_run = 0;
int pListSize = 3;
param pList [] =
{
    { .filename="sample", .nVertex=12, .nEdge=12 },
    { .filename="samples/petersen.dot", .nVertex=10, .nEdge=15 },
    { .filename="samples/heawood.dot", .nVertex=14, .nEdge=21 }
};

static char* testGraphLoad()
{
    FILE *fp = fopen(pList[0].filename,"r");
    grafo grf = le_grafo(fp);
    mu_assert("Test 1 - Error: Graph is null",grf!=NULL);
    fclose(fp);
    return 0;
}

static char* testGraphName()
{
    FILE *fp = fopen(pList[0].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    sprintf( msg, "Test 2 - Found: %s Expected: %s", nome_grafo(grf), agnameof(g));
    mu_assert(msg,strcmp(nome_grafo(grf),agnameof(g))==0);
    agclose(g);
    fclose(fp);
    return 0;
}

static char* testCreateEmptyList()
{
    lista l= constroi_lista();
    mu_assert("Test 3 - Unable to start list", l!=NULL);
    return 0;
}

static char* testDestroyEmptyList()
{
    lista l= constroi_lista();
    int ok = destroi_lista(l,NULL);
    mu_assert("Test 4 - Unable to start list", ok);
    return 0;
}

static char* primeiroNoNulo()
{
    lista l= constroi_lista();
    no n = primeiro_no(l);
    mu_assert("Test 5 - First node should be null ", n==NULL);
    destroi_lista(l,NULL);
    return 0;
}

static char* primeiroNoNaoNulo()
{
    lista l= constroi_lista();
    no n = primeiro_no(l);
    no n1 = malloc(sizeof(no));
    if (n1 == NULL)
    {
        return "unable to allocate node";
    }
    mu_assert("Test 6 - First node should not be null ", n1!=NULL);
    destroi_lista(l,NULL);
    return 0;
}

static char* testNumeroVertices()
{
    FILE *fp = fopen(pList[0].filename,"r");
    grafo grf = le_grafo(fp);
    mu_assert("Test 7 - Error: Graph vertex number is not the same as input",numero_vertices(grf)==pList[0].nVertex);
    fclose(fp);
    return 0;
}

static char* testNumeroVertices10vezes()
{
    FILE *fp = fopen(pList[0].filename,"r");
    for(int i = 1; i < 11; i++)
    {
        grafo grf = le_grafo(fp);
        sprintf(msg,"Test 8 - Error: Graph vertex number is not the same as input after %d attempts", i);
        mu_assert(msg,numero_vertices(grf)==pList[0].nVertex);
        rewind(fp);
    }
    fclose(fp);
    return 0;
}

static char* testNumeroVerticesAllGraphs()
{
    for( int i = 0; i < pListSize; i++)
    {
        FILE *fp = fopen(pList[i].filename,"r");
        grafo grf = le_grafo(fp);
        char * message = (char * ) malloc(100 * sizeof(char));
        sprintf(message, "Test 9 - Filename: %s Expected: %d, Found: %d ", pList[i].filename, pList[i].nVertex,  numero_vertices(grf));
        mu_assert(message,numero_vertices(grf)==pList[i].nVertex);
        free(grf);
        fclose(fp);
    }
    return 0;
}

static char* testNumeroArestasAllGraphs()
{
    for( int i = 0; i < pListSize; i++)
    {
        FILE *fp = fopen(pList[i].filename,"r");
        grafo grf = le_grafo(fp);
        char * message = (char * ) malloc(100 * sizeof(char));
        sprintf(message, "Test 10 - Filename: %s Expected: %d, Found: %d ", pList[i].filename, pList[i].nEdge, numero_arestas(grf));
        mu_assert(message,numero_arestas(grf)==pList[i].nEdge);
        free(grf);
        fclose(fp);
    }
    return 0;
}

static char* testVerticesNome()
{
    for( int i = 0; i < pListSize; i++)
    {
        FILE *fp = fopen(pList[i].filename,"r");
        grafo grf = le_grafo(fp);
        rewind(fp);
        Agraph_t *g = agread(fp, NULL);
        Agnode_t *agfst = agfstnode(g);
        sprintf(
            msg, "Test 11 Vertice nome comparison failed - Filename: %s Expected: %s, Found: %s ",
                    pList[i].filename,
                    agnameof(agfst),
                    nome_vertice(vertice_nome(agnameof(agfst),grf))
                );
        mu_assert(msg,strcmp(agnameof(agfst),vertice_nome(agnameof(agfst),grf))!=0);
        agclose(g);
        free(grf);
        fclose(fp);
    }
    return 0;
}

static char* testTodosVerticesNome()
{
    for( int i = 0; i < pListSize; i++)
    {
        FILE *fp = fopen(pList[i].filename,"r");
        grafo grf = le_grafo(fp);
        rewind(fp);
        Agraph_t *g = agread(fp, NULL);
        for (Agnode_t *agfst=agfstnode(g); agfst; agfst=agnxtnode(g,agfst)){
            sprintf(
                msg, "Test 12 - Vertice nome comparison failed - Filename: %s Expected: %s, Found: %s ",
                        pList[i].filename,
                        agnameof(agfst),
                        nome_vertice(vertice_nome(agnameof(agfst),grf))
                    );
            mu_assert(msg,strcmp(agnameof(agfst),vertice_nome(agnameof(agfst),grf))!=0);
        }
        agclose(g);
        free(grf);
        fclose(fp);
    }
    return 0;
}

static char* testDestroiGrafo(){
    FILE *fp = fopen(pList[0].filename,"r");
    grafo grf = le_grafo(fp);
    int status = destroi_grafo(grf);
    mu_assert("Test 8 - Error: Unable to deallocate graph",status==1);
    fclose(fp);
    return 0;
}

static char * all_tests()
{
    mu_run_test(testGraphLoad);
    mu_run_test(testGraphName);
    mu_run_test(testCreateEmptyList);
    mu_run_test(testDestroyEmptyList);
    mu_run_test(primeiroNoNulo);
    mu_run_test(primeiroNoNaoNulo);
    mu_run_test(testNumeroVertices);
    mu_run_test(testNumeroVertices10vezes);
    mu_run_test(testNumeroVerticesAllGraphs);
    mu_run_test(testVerticesNome);
    mu_run_test(testTodosVerticesNome);
    mu_run_test(testDestroiGrafo);
    return 0;
}

int main(int argc, char **argv)
{
    char *result = all_tests();
    if (result != 0)
    {
        printf("%s\n", result);
    }
    else
    {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
