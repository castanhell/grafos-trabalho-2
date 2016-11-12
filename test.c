#include <assert.h>
#include <graphviz/cgraph.h>
#include "grafo.h"
#include "minunit.h"
#include "parameters.h"
#include <stdio.h>
#include <stdlib.h>

int tests_run = 0;
int plistSize = 1;
param pList [] = { { .filename="sample", .nVertex=12 } };

static char* testGraphLoad(){
	FILE *fp = fopen(pList[0].filename,"r");
	grafo grf = le_grafo(fp);	
	mu_assert("Error: Graph is null",grf!=NULL);
	fclose(fp);
}

static char* testGraphName(){
	FILE *fp = fopen(pList[0].filename,"r");
	grafo grf = le_grafo(fp);	
	rewind(fp);
    	Agraph_t *g = agread(fp, NULL);
	char * found = nome_grafo(grf);
	char * expected = agnameof(g);
	char * message = (char * ) malloc(100 * sizeof(char));
	sprintf( message, "Found: %s Expected: %s" , found, expected);
	mu_assert(message,strcmp(found,expected)==0);
	free(message);
	fclose(fp);
}

static char* testEmptyList(){
	lista l = NULL;
	mu_assert("List pointer should not be empty",l!=NULL);		
}

static char * all_tests() {
	mu_run_test(testGraphLoad);
	mu_run_test(testGraphName);
	mu_run_test(testEmptyList);
	return 0;
}

int main(int argc, char **argv) {
	char *result = all_tests();
	if (result != 0) {
		printf("%s\n", result);
	}
	else {
		printf("ALL TESTS PASSED\n");
	}
	printf("Tests run: %d\n", tests_run);

	return result != 0;
}
