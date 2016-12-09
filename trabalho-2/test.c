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
int pListSize = 10;
param pList [] =
{
    { .filename="sample", .nVertex=12, .nEdge=12, .directed=0, .weighted=1 },
    { .filename="samples/petersen.dot", .nVertex=10, .nEdge=15, .directed=0, .weighted=0 },
    { .filename="samples/heawood.dot", .nVertex=14, .nEdge=21, .directed=0, .weighted=0 },
    { .filename="samples/sampledirected.dot", .nVertex=12, .nEdge=12, .directed=1, .weighted=1 },
    { .filename="samples/samplepartialweighted.dot", .nVertex=12, .nEdge=12, .directed=1, .weighted=0 },
    { .filename="samples/dijkstrapequenonaodirecionado.dot", .nVertex=6, .nEdge=5, .directed=0, .weighted=0 },
    { .filename="samples/dijkstrapequenonaodirecionado.dot", .nVertex=6, .nEdge=5, .directed=0, .weighted=0 },
    { .filename="samples/dijkstrapequenodirecionado.dot", .nVertex=7, .nEdge=5, .directed=1, .weighted=0 },
    { .filename="samples/dijkstrapequenodirecionado2componentes.dot", .nVertex=12, .nEdge=10, .directed=0, .weighted=0 },
    { .filename="samples/dijkstrapequenonaodirecionadociclo.dot", .nVertex=6, .nEdge=6, .directed=0, .weighted=0 }
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
        sprintf(msg, "Test 9 - Number of vertices Filename: %s Expected: %d, Found: %d ", pList[i].filename, pList[i].nVertex,  numero_vertices(grf));
        mu_assert(msg,numero_vertices(grf)==pList[i].nVertex);
        destroi_grafo(grf);
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
            vertice v1=vertice_nome(agnameof(agtail(a)),grf),
                    v2=vertice_nome(agnameof(aghead(a)),grf);
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
            vertice v1=vertice_nome(agnameof(agtail(a)),grf),
                    v2=vertice_nome(agnameof(aghead(a)),grf);
            char *ps = agget(a, (char *)"peso");
            int peso;
            if(ps==NULL || strlen(ps)==0)
            {
                peso = 1;
            }
            else
            {
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

static char* testTodosVerticesIndice()
{
    for( int i = 0; i < pListSize; i++)
    {
        FILE *fp = fopen(pList[i].filename,"r");
        grafo grf = le_grafo(fp);
        rewind(fp);
        Agraph_t *g = agread(fp, NULL);
        unsigned int ii = 0;
        for (Agnode_t *agfst=agfstnode(g); agfst; agfst=agnxtnode(g,agfst))
        {
            sprintf(
                msg, "Test 22 - Vertice index comparison failed - Filename: %s Expected: %d, Found: %d\n",
                pList[i].filename,
                i,
                indice(vertice_nome(agnameof(agfst),grf),grf));
            mu_assert(msg,ii==indice(vertice_nome(agnameof(agfst),grf),grf));
            ii++;
        }
        agclose(g);
        destroi_grafo(grf);
        fclose(fp);
    }
    return 0;
}

static char* testListaDistanciaPequenoNaoDirecionadoDistanciaUm()
{
    FILE *fp = fopen(pList[5].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    lista caminho = caminho_minimo(vertice_nome("c",grf),vertice_nome("d",grf),grf);
    mu_assert("Test 23 - o caminho nao pode ser nulo",caminho!=NULL);
    vertice v = (vertice) conteudo(primeiro_no(caminho));
    mu_assert("Test 23 - o vertice nao pode ser nulo",v!=NULL);
    sprintf(
        msg, "Test 23 - Distancia simples failed - Filename: %s Expected: %d, Found: %d ",
        pList[5].filename,
        1,
        indice(v,grf)
    );
    mu_assert(msg,indice(vertice_nome("c",grf),grf)==indice(v,grf));
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    return 0;
}

static char* testListaDistanciaPequenoNaoDirecionadoDistanciaDois()
{
    FILE *fp = fopen(pList[5].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    lista caminho = caminho_minimo(vertice_nome("a",grf),vertice_nome("c",grf),grf);
    mu_assert("Test 24 - o caminho nao pode ser nulo",caminho!=NULL);
    vertice v = (vertice) conteudo(primeiro_no(caminho));
    vertice proximo = conteudo(proximo_no(primeiro_no(caminho)));
    mu_assert("Test 24 - o vertice nao pode ser nulo",v!=NULL);
    mu_assert("Test 24 - o proximo vertice nao pode ser nulo",proximo!=NULL);
    sprintf(
        msg, "Test 24 - Distancia simples failed - Filename: %s Expected: %d, Found: %d ",
        pList[5].filename,
        indice(vertice_nome("b",grf),grf),
        indice(v,grf)
    );
    mu_assert(msg,indice(vertice_nome("b",grf),grf)==indice(v,grf));
    sprintf(
        msg, "Test 24 - Distancia simples failed - Filename: %s Expected: %d, Found: %d ",
        pList[5].filename,
        indice(vertice_nome("a",grf),grf),
        indice(proximo,grf)
    );
    mu_assert(msg,indice(vertice_nome("a",grf),grf)==indice(proximo,grf));
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    return 0;
}

static char* testValorDistanciaPequenoNaoDirecionadoDistanciaUm()
{
    FILE *fp = fopen(pList[5].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    long int dist = distancia(vertice_nome("c",grf),vertice_nome("d",grf),grf);
    sprintf(
        msg, "Test 25 - Distancia simples failed - Filename: %s Expected: %d, Found: %d ",
        pList[5].filename,
        1,
        dist
    );
    mu_assert(msg,1==dist);
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    return 0;
}

static char* testValorDistanciaPequenoNaoDirecionadoDistanciaDois()
{
    FILE *fp = fopen(pList[5].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    long int dist = distancia(vertice_nome("c",grf),vertice_nome("a",grf),grf);
    sprintf(
        msg, "Test 25 - Distancia simples failed - Filename: %s Expected: %d, Found: %d ",
        pList[5].filename,
        2,
        dist
    );
    mu_assert(msg,2==dist);
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    return 0;
}

static char* testListaDistanciaPequenoDirecionadoDistanciaUm()
{
    FILE *fp = fopen(pList[7].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    lista caminho = caminho_minimo(vertice_nome("c",grf),vertice_nome("d",grf),grf);
    mu_assert("Test 26 - o caminho nao pode ser nulo",caminho!=NULL);
    vertice v = (vertice) conteudo(primeiro_no(caminho));
    mu_assert("Test 26 - o vertice nao pode ser nulo",v!=NULL);
    sprintf(
        msg, "Test 26 - Distancia simples failed - Filename: %s Expected: %d, Found: %d ",
        pList[5].filename,
        1,
        indice(v,grf)
    );
    mu_assert(msg,indice(vertice_nome("c",grf),grf)==indice(v,grf));
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    return 0;
}

static char* testValorDistanciaPequenoDirecionadoDistanciaDois()
{
    FILE *fp = fopen(pList[7].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    long int dist = distancia(vertice_nome("a",grf),vertice_nome("c",grf),grf);
    sprintf(
        msg, "Test 27 - Distancia simples failed - Filename: %s Expected: %d, Found: %d ",
        pList[5].filename,
        2,
        dist
    );
    mu_assert(msg,2==dist);
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    return 0;
}

static char* testValorDistanciaPequenoDirecionadoDistanciaInfinita()
{
    FILE *fp = fopen(pList[7].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    long int dist = distancia(vertice_nome("c",grf),vertice_nome("a",grf),grf);
    sprintf(
        msg, "Test 28 - Distancia simples failed - Filename: %s Expected: %d, Found: %d ",
        pList[5].filename,
        2,
        dist
    );
    mu_assert(msg,infinito==dist);
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    return 0;
}

static char* testValorDistanciaDirecionado2ComponentesDistanciaInfinita()
{
    FILE *fp = fopen(pList[8].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    long int dist = distancia(vertice_nome("a1",grf),vertice_nome("a",grf),grf);
    sprintf(
        msg, "Test 29 - Distancia simples failed - Filename: %s Expected: %d, Found: %d ",
        pList[5].filename,
        infinito,
        dist
    );
    mu_assert(msg,infinito==dist);
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    return 0;
}

static char* testListaDistancia2ComponentesNaoDirecionadoDistanciaInfinita()
{
    FILE *fp = fopen(pList[8].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    lista caminho = caminho_minimo(vertice_nome("a",grf),vertice_nome("c1",grf),grf);
    mu_assert("Test 30 - o caminho deve ser nulo",tamanho_lista(caminho)==0);
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    return 0;
}

static char* testValorDistanciaNaoDirecionadoComCiclo()
{
    FILE *fp = fopen(pList[9].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    long int dist = distancia(vertice_nome("a",grf),vertice_nome("f",grf),grf);
    sprintf(
        msg, "Test 31 - Distancia simples failed - Filename: %s Expected: %d, Found: %d ",
        pList[5].filename,
        1,
        dist
    );
    mu_assert(msg,1==dist);
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    return 0;
}

static char* testValorDistanciaNaoDirecionadoComCicloCaminhosRedundantes()
{
    FILE *fp = fopen(pList[9].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    long int dist = distancia(vertice_nome("a",grf),vertice_nome("d",grf),grf);
    sprintf(
        msg, "Test 32 - Distancia simples failed - Filename: %s Expected: %d, Found: %d ",
        pList[5].filename,
        3,
        dist
    );
    mu_assert(msg,3==dist);
    lista caminhos = caminho_minimo(vertice_nome("a",grf),vertice_nome("c",grf),grf);
    sprintf(
        msg, "Test 32 - Tamanho lista failed - Filename: %s Expected: %d, Found: %d ",
        pList[5].filename,
        3,
        tamanho_lista(caminhos)
    );
    mu_assert(msg,2==tamanho_lista(caminhos));
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    return 0;
}

static char* testValorCNaoDirecionadoComCicloCaminhosRedundantes()
{
    FILE *fp = fopen(pList[9].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    long int dist = distancia(vertice_nome("a",grf),vertice_nome("d",grf),grf);
    sprintf(
        msg, "Test 32 - Distancia simples failed - Filename: %s Expected: %d, Found: %d ",
        pList[5].filename,
        3,
        dist
    );
    mu_assert(msg,3==dist);
    lista caminhos = caminho_minimo(vertice_nome("a",grf),vertice_nome("c",grf),grf);
    sprintf(
        msg, "Test 32 - Tamanho lista failed - Filename: %s Expected: %d, Found: %d ",
        pList[5].filename,
        3,
        tamanho_lista(caminhos)
    );
    mu_assert(msg,3==tamanho_lista(caminhos));
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    return 0;
}

static char* testMatrizDistanciasNaoDirecionadoPequeno()
{
    FILE *fp = fopen(pList[5].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    long int diam = diametro(grf);
    long int ** M;
    M = malloc( numero_vertices(grf) * sizeof(long int*));
    for(int j = 0; j < numero_vertices(grf); j++)
    {
        M[j] = malloc( numero_vertices(grf) * sizeof(long int));
    }
    distancias(M,grf,'d');
    mu_assert("M[0][5] deveria ser 5",M[0][5]==5);
    distancias(M,grf,'a');
    mu_assert("M[0][5] deveria ser 5",M[0][5]==5);
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    free(M);
    return 0;
}

static char* testListaCaminhosGrafoNaoDirecionado()
{
    FILE *fp = fopen(pList[5].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    lista ** caminhos = malloc(numero_vertices(grf) * sizeof(lista*));
    for(int j = 0; j < numero_vertices(grf) ; j++)
    {
        caminhos[j] = malloc(numero_vertices(grf) * sizeof(lista));
    }
    caminhos_minimos(caminhos,grf,'d');
    mu_assert("Dijkstra: M[0][5] deveria ser 5",tamanho_lista(caminhos[0][5])==5);
    caminhos_minimos(caminhos,grf,'a');
    no n = primeiro_no(caminhos[0][5]);
    while(n != NULL)
    {
        vertice v = (vertice) conteudo(n);
        printf("%s ", nome_vertice(v));
        n = proximo_no(n);
    }
    printf("\n");
    mu_assert("FW : M[0][5] deveria ser 5",tamanho_lista(caminhos[0][5])==5);
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    free(caminhos);
    return 0;
}

static char* testListaCaminhosGrafoNaoDirecionado2Componentes()
{
    FILE *fp = fopen(pList[8].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    long int diam = diametro(grf);
    long int ** M;
    M = malloc( numero_vertices(grf) * sizeof(long int*));
    for(int j = 0; j < numero_vertices(grf); j++)
    {
        M[j] = malloc( numero_vertices(grf) * sizeof(long int));
    }
    distancias(M,grf,'d');
    mu_assert("Dijkstra: M[0][6] deveria ser infinito",M[0][6]==infinito);
    distancias(M,grf,'a');
    mu_assert("FW : M[0][10] deveria ser infinito",M[0][10]==infinito);
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    free(M);
    return 0;
}

static char* testDistanciaCaminhosGrafoDirecionado()
{
    FILE *fp = fopen(pList[7].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    long int diam = diametro(grf);
    long int ** M;
    M = malloc( numero_vertices(grf) * sizeof(long int*));
    for(int j = 0; j < numero_vertices(grf); j++)
    {
        M[j] = malloc( numero_vertices(grf) * sizeof(long int));
    }
    distancias(M,grf,'d');
    mu_assert("FW : M[0][5] deveria ser 5",M[0][5]==5);
    for(int i = 0; i < numero_vertices(grf); i++)
    {
        for(int j = 0; j < numero_vertices(grf); j++)
        {
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    free(M);
    return 0;
}

static char* testDistanciaFWCaminhosGrafoDirecionado()
{
    FILE *fp = fopen(pList[7].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    long int diam = diametro(grf);
    long int ** M;
    M = malloc( numero_vertices(grf) * sizeof(long int*));
    for(int j = 0; j < numero_vertices(grf); j++)
    {
        M[j] = malloc( numero_vertices(grf) * sizeof(long int));
    }
    distancias(M,grf,'a');
    mu_assert("FW : M[0][5] deveria ser 5",M[0][5]==5);
    for(int i = 0; i < numero_vertices(grf); i++)
    {
        for(int j = 0; j < numero_vertices(grf); j++)
        {
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    free(M);
    return 0;
}

static char* testMatrizDistanciasGrafoNaoDirecionado2Componentes()
{
    FILE *fp = fopen(pList[8].filename,"r");
    grafo grf = le_grafo(fp);
    rewind(fp);
    Agraph_t *g = agread(fp, NULL);
    lista ** caminhos = malloc(numero_vertices(grf) * sizeof(lista*));
    for(int j = 0; j < numero_vertices(grf) ; j++)
    {
        caminhos[j] = malloc(numero_vertices(grf) * sizeof(lista));
    }
    caminhos_minimos(caminhos,grf,'a');
    mu_assert("Dijkstra: M[6][10] deveria ser 5",tamanho_lista(caminhos[0][10])==0);
    caminhos_minimos(caminhos,grf,'d');
    mu_assert("FW : M[0][4] deveria ser 5",tamanho_lista(caminhos[0][4])==4);
    no n = primeiro_no(caminhos[0][5]);
    while(n != NULL)
    {
        vertice v = (vertice) conteudo(n);
        printf("%s ", nome_vertice(v));
        n = proximo_no(n);
    }
    printf("\n");
    agclose(g);
    destroi_grafo(grf);
    fclose(fp);
    free(caminhos);
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
    mu_run_test(testNumeroArestasAllGraphs);
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
    mu_run_test(testTodosVerticesIndice);
    mu_run_test(testListaDistanciaPequenoNaoDirecionadoDistanciaUm);
    mu_run_test(testListaDistanciaPequenoNaoDirecionadoDistanciaDois);
    mu_run_test(testValorDistanciaPequenoNaoDirecionadoDistanciaUm);
    mu_run_test(testValorDistanciaPequenoNaoDirecionadoDistanciaDois);
    mu_run_test(testListaDistanciaPequenoDirecionadoDistanciaUm);
    mu_run_test(testValorDistanciaPequenoDirecionadoDistanciaDois);
    mu_run_test(testValorDistanciaPequenoDirecionadoDistanciaInfinita);
    mu_run_test(testValorDistanciaDirecionado2ComponentesDistanciaInfinita);
    mu_run_test(testListaDistancia2ComponentesNaoDirecionadoDistanciaInfinita);
    mu_run_test(testListaDistancia2ComponentesNaoDirecionadoDistanciaInfinita);
    mu_run_test(testValorDistanciaNaoDirecionadoComCiclo);
    mu_run_test(testValorDistanciaNaoDirecionadoComCicloCaminhosRedundantes);
    mu_run_test(testMatrizDistanciasNaoDirecionadoPequeno);
    mu_run_test(testListaCaminhosGrafoNaoDirecionado);
    mu_run_test(testListaCaminhosGrafoNaoDirecionado2Componentes);
    mu_run_test(testListaCaminhosGrafoNaoDirecionado2Componentes);
    mu_run_test(testDistanciaCaminhosGrafoDirecionado);
    mu_run_test(testDistanciaFWCaminhosGrafoDirecionado);
    mu_run_test(testMatrizDistanciasGrafoNaoDirecionado2Componentes);
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
