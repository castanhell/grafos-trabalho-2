#define TESTE
#include <assert.h>
#include <graphviz/cgraph.h>
#include "grafo.h"
#include "minunit.h"
#include "parameters.h"
#include <stdio.h>
#include <stdlib.h>

char msg[100];
int tests_run = 0;
int pListSize = 4;
param pList [] =
{
    { .filename="sample", .nVertex=12, .nEdge=12, .directed=0, .weighted=1 },
    { .filename="samples/petersen.dot", .nVertex=10, .nEdge=15, .directed=0, .weighted=0 },
    { .filename="samples/heawood.dot", .nVertex=14, .nEdge=21, .directed=0, .weighted=0 },
    { .filename="samples/sampledirected.dot", .nVertex=12, .nEdge=12, .directed=1, .weighted=1 },
    { .filename="samples/samplepartialweighted.dot", .nVertex=12, .nEdge=12, .directed=1, .weighted=0 }
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
        for (Agnode_t *agfst=agfstnode(g); agfst; agfst=agnxtnode(g,agfst))
        {
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

static char* testDestroiGrafo()
{
    FILE *fp = fopen(pList[0].filename,"r");
    grafo grf = le_grafo(fp);
    int status = destroi_grafo(grf);
    mu_assert("Test 13 - Error: Unable to deallocate graph",status==1);
    fclose(fp);
    return 0;
}

static char* testGrauPrimeiroVerticeNaoDirecionado()
{
    for( int i = 0; i < pListSize; i++)
    {
        FILE *fp = fopen(pList[i].filename,"r");
        grafo grf = le_grafo(fp);
        rewind(fp);
        Agraph_t *g = agread(fp, NULL);
        Agnode_t *agfst = agfstnode(g);
        int expectedDeg = 0;
        for (Agedge_t *a=agfstedge(g,agfst); a; a=agnxtedge(g,a,agfst))
        {
            if (agfst == agtail(a))
            {
                expectedDeg++;
            }
            else if(agfst == aghead(a))
            {
                expectedDeg++;
            }
        }
        int foundDeg = grau(vertice_nome(agnameof(agfst),grf),0,grf);
        sprintf(
            msg, "Test 14  Grau failed - Filename: %s Expected: %d, Found: %d ",
            pList[i].filename,
            expectedDeg,
            foundDeg
        );
        mu_assert(msg,expectedDeg==foundDeg);
        agclose(g);
        destroi_grafo(grf);
        fclose(fp);
    }
    return 0;
}

static char* testGrauTodosVerticesNaoDirecionado()
{
    for( int i = 0; i < pListSize; i++)
    {
        FILE *fp = fopen(pList[i].filename,"r");
        grafo grf = le_grafo(fp);
        rewind(fp);
        Agraph_t *g = agread(fp, NULL);
        for (Agnode_t *agfst=agfstnode(g); agfst; agfst=agnxtnode(g,agfst))
        {
            int expectedDeg = 0;
            for (Agedge_t *a=agfstedge(g,agfst); a; a=agnxtedge(g,a,agfst))
            {
                if (agfst == agtail(a))
                {
                    expectedDeg++;
                }
                else if(agfst == aghead(a))
                {
                    expectedDeg++;
                }
            }
            int foundDeg = grau(vertice_nome(agnameof(agfst),grf),0,grf);
            sprintf(
                msg, "Test 15  Grau failed - Filename: %s Expected: %d, Found: %d ",
                pList[i].filename,
                expectedDeg,
                foundDeg
            );
            mu_assert(msg,expectedDeg==foundDeg);
        }
        agclose(g);
        destroi_grafo(grf);
        fclose(fp);
    }
    return 0;
}

static char* testGrauPrimeiroVerticeEntradaSaidaDirecionado()
{
    FILE *fp = fopen(pList[3].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    Agnode_t *agfst = agfstnode(g);
    int expectedOutputDeg = 0;
    int expectedInputDeg = 0;
    for (Agedge_t *a=agfstedge(g,agfst); a; a=agnxtedge(g,a,agfst))
    {
        if (agfst == agtail(a))
        {
            expectedOutputDeg++;
        }
        if(agfst==aghead(a))
        {
            expectedInputDeg++;
        }
    }
    int foundOutputDeg = grau(vertice_nome(agnameof(agfst),grf),1,grf);
    sprintf(
        msg, "Test 16  Grau saida failed - Filename: %s Expected: %d, Found: %d ",
        pList[3].filename,
        expectedOutputDeg,
        foundOutputDeg
    );
    mu_assert(msg,expectedOutputDeg==foundOutputDeg);
    int foundInputDeg = grau(vertice_nome(agnameof(agfst),grf),-1,grf);
    sprintf(
        msg, "Test 16  Grau entrada failed - Filename: %s Expected: %d, Found: %d ",
        pList[3].filename,
        expectedInputDeg,
        foundInputDeg
    );
    mu_assert(msg,expectedInputDeg==foundInputDeg);
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);

    return 0;
}

static char* testDirecionadoNaoDirecionado()
{
    for( int i = 0; i < pListSize; i++)
    {
        FILE *fp = fopen(pList[i].filename,"r");
        grafo grf = le_grafo(fp);
        sprintf(msg,"Test 17 - Directed expected: %d Directed found: %d for file: %s", pList[i].directed, direcionado(grf), pList[i].filename);
        mu_assert(msg, direcionado(grf) == pList[i].directed );
        fclose(fp);
    }
    return 0;
}

static char* testPonderadoNaoPonderado()
{
    for( int i = 0; i < pListSize; i++)
    {
        FILE *fp = fopen(pList[i].filename,"r");
        grafo grf = le_grafo(fp);
        sprintf(msg,"Test 17 - Weighted expected: %d Weighted found: %d for file: %s",
                pList[i].weighted, ponderado(grf),
                pList[i].filename
               );
        mu_assert(msg, ponderado(grf) == pList[i].weighted );
        fclose(fp);
    }
    return 0;
}

static char* testPonderadoIncompleto()
{
    FILE *fp = fopen(pList[4].filename,"r");
    grafo grf = le_grafo(fp);
    sprintf(msg,"Test 18 - Weighted expected: %d Weighted found: %d for file: %s",
            pList[4].weighted, ponderado(grf),
            pList[4].filename
           );
    mu_assert(msg, ponderado(grf) == pList[4].weighted );
    fclose(fp);
    return 0;
}

static char* testTodasArestasPrimeiroVertice()
{
    for( int i = 0; i < pListSize; i++)
    {
        FILE *fp = fopen(pList[i].filename,"r");
        grafo grf = le_grafo(fp);
        rewind(fp);
        Agraph_t *g = agread(fp, NULL);
        Agnode_t *agfst = agfstnode(g);
        for (Agedge_t *a=agfstedge(g,agfst); a; a=agnxtedge(g,a,agfst))
        {
            vertice v1=vertice_nome(agnameof(aghead(a)),grf),
                    v2=vertice_nome(agnameof(agtail(a)),grf);
            sprintf(msg,"Test 19 - vertexes %s and %s expected to be neighboors. Filename: %s",
                    nome_vertice(v1),
                    nome_vertice(v2),
                    pList[i].filename
                   );
            mu_assert(msg,vizinho(v1,v2)==1);
        }
        agclose(g);

        destroi_grafo(grf);
        fclose(fp);
    }
    return 0;
}

static char* test_adiciona_lista()
{
    lista l = constroi_lista();
    adiciona_lista(l,1);
    adiciona_lista(l,2);
    adiciona_lista(l,3);
    sprintf(msg,"Teste 20 :Tamanho da list esperado : 3, encontrado: %d", tamanho_lista(l));
    mu_assert(msg,3==tamanho_lista(l));
    return 0;
}

static char* test_primeiro_no()
{
    lista l = constroi_lista();
    adiciona_lista(l,1);
    adiciona_lista(l,2);
    adiciona_lista(l,3);
    adiciona_lista(l,4);
    adiciona_lista(l,5);
    sprintf(msg,"Teste 21 :Primeiro vertice esperado : 1, encontrado: %d", (int)conteudo(primeiro_no(l)));
    mu_assert(msg,1==(int)conteudo(primeiro_no(l)));
    no n = primeiro_no(l);
    for(int i = 1; i < 5; i++)
    {
        int encontrado = 0;
        if(proximo_no(n))
        {
            encontrado = conteudo(proximo_no(n));
        }
        sprintf(msg,"Teste 21: Proximo no esperado : 1, encontrado: %d", i+1, encontrado );
        mu_assert(msg,i+1==encontrado);
        n = proximo_no(n);
    }
    return 0;
}

static char* testPesoTodasArestasPrimeiroVertice()
{
    for( int i = 0; i < pListSize; i++)
    {
        FILE *fp = fopen(pList[i].filename,"r");
        grafo grf = le_grafo(fp);
        rewind(fp);
        Agraph_t *g = agread(fp, NULL);
        Agnode_t *agfst = agfstnode(g);
        for (Agedge_t *a=agfstedge(g,agfst); a; a=agnxtedge(g,a,agfst))
        {
            vertice v1=vertice_nome(agnameof(aghead(a)),grf),
                    v2=vertice_nome(agnameof(agtail(a)),grf);
            char *ps = agget(a, (char *)"peso");
            int peso;
            if(ps==NULL || strlen(ps)==0)
            {
                peso = 1;
            }
            else{
                peso = atoi(ps);
            }
            sprintf(msg,"Test 21 - vertexes %s and %s expected to have weight. Filename: %s",
                    nome_vertice(v1),
                    nome_vertice(v2),
                    pList[i].filename
                   );
            mu_assert(msg,vizinho_peso(v1,v2)==peso);
        }
        agclose(g);

        destroi_grafo(grf);
        fclose(fp);
    }
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
    mu_run_test(testGrauPrimeiroVerticeNaoDirecionado);
    mu_run_test(testGrauTodosVerticesNaoDirecionado);
    mu_run_test(testGrauPrimeiroVerticeEntradaSaidaDirecionado);
    mu_run_test(testDirecionadoNaoDirecionado);
    mu_run_test(testPonderadoNaoPonderado);
    mu_run_test(testPonderadoIncompleto);
    mu_run_test(testTodasArestasPrimeiroVertice);
    mu_run_test(test_adiciona_lista);
    mu_run_test(test_primeiro_no);
    mu_run_test(testPesoTodasArestasPrimeiroVertice);
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
