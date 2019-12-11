#include <stdio.h>
#include <stdlib.h>

/* alus�o das fun��es que implementam os algoritmos pretendidos */
/* allusion to the functions that implement the required algorithms */
int T (int);

/* vari�vel global para contar as invoca��es recursivas executadas pelo algoritmo */
/* global variable for counting the recursive calls executed by the algorithm */
int Inv;

int main (void)
{
	int Result, Line, N, Test;

	/* leitura do valor limite da tabela */
	/* reading the limit value for the table */
	do
	{
		printf ("N? "); Test = scanf ("%d", &N);
		scanf ("%*[^\n]"); scanf ("%*c");
	} while (Test == 0);

    /* impress�o da tabela de execu��o do algoritmo */
    /* impression of the table values for the algorithm's execution */
	for (Line = 1; Line <= N; Line++)
	{
        /* inicializa��o da vari�vel global contadora das invoca��es recursivas */
        /* initialization of the global variable for counting the recursive calls */
		Inv = 0;

		/* invoca��o do algoritmo pretendido */
		/* invocation of the pretended algorithm */
		Result = T (Line);

		/* apresenta��o do resultado e do n�mero de invoca��es recursivas executadas pelo algoritmo */
		/* presenting the result and the number of recursive calls executed by the algorithm */
		fprintf (stdout, "T(%2d) = %4d e fez %2d invocacoes\n", Line, Result, Inv);
	}

	scanf ("%*c");
	exit (EXIT_SUCCESS);
}

/* implementa��o dos algoritmos pretendidos */
/* acrescentar a contagem das invoca��es recursivas executadas pelos algoritmos usando a vari�vel global */

/* implementation of the pretended algorithms */
/* do not forget to count the recursive calls using the global variable */

int T (int n)
{
	if(n == 1) return 1;
	Inv+=2;
	return T(n/2) + T(((n+1)/2)) + n;
}



