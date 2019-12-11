#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "elapsed_time.h"


/* definição do tipo de dados inteiro sem sinal de 64 bits */
/* definition of the 64-bit unsigned integer type */
typedef unsigned long long u64;

/* alusão das funções que implementam os algoritmos pretendidos */
/* allusion to the functions that implement the required algorithms */
u64 fibrecursivo (int);
u64 fibrepetitiva (int);
u64 fibfechada1 (int);
u64	fibfechada2 (int);
u64 fibfechada3 (int);

/* variável global para contar as operações aritméticas executadas pelo algoritmo */
/* global variable for counting the arithmetic operations executed by the algorithm */
u64 Count;
double Phi = (1+sqrt(5))/2;
double C1 = 0.44721357549995793928;
double C2 = 0.48121182505960344750;

int main (void)
{
	u64 Result; int NLines, N, Test;

	/* leitura do valor limite da tabela */
	/* reading the limit value for the table */
	do
	{
		printf ("N de elementos da tabela? "); Test = scanf ("%d", &NLines);
		scanf ("%*[^\n]"); scanf ("%*c");
	} while (Test == 0);

    /* impressão da tabela de execução do algoritmo */
    /* impression of the table values for the algorithm's execution */
	for (N = 0; N <= NLines; N++)
	{
        /* inicialização da variável global contadora das operações aritméticas */
        /* initialization of the global variable for counting the arithmetic operations */
		Count = 0;

		/* invocação do algoritmo pretendido */
		/* invocation of the pretended algorithm */
		elapsed_time();
		Result = fibfechada3(N);
		double time = elapsed_time();
		/* apresentação do resultado e do número de operações aritméticas executadas pelo algoritmo */
		/* presenting the result and the number of arithmetic operations executed by the algorithm */
		fprintf (stdout, "Fibonacci de %2d = %22llu, executou %12llu somas e demorou %f\n", N, Result, Count,time);
	}

	exit (EXIT_SUCCESS);
}

/* implementação dos algoritmos pretendidos */
/* acrescentar a contagem das operações aritméticas executadas pelos algoritmos usando a variável global */

/* implementation of the pretended algorithms */
/* do not forget to count the arithmetic operations using the global variable */

u64 fibrecursivo (int n){
	if (n<=1) return n;
	Count++;
	return fibrecursivo(n-1)+fibrecursivo(n-2);
}

u64 fibrepetitiva (int n){
	u64 anteriorAnterior = 0;
	u64 anterior = 1;
	u64 atual;
	if (n<2) {
		return n;
	}
	for (int i = 1; i < n; i++) {
		atual = anterior+anteriorAnterior;
		Count++;
		anteriorAnterior = anterior;
		anterior = atual;
	}
	return atual;
}

u64 fibfechada1 (int n){
	return (pow(Phi,n)-pow(1-Phi,n))/sqrt(5);
}

u64 fibfechada2(int n){
	return round(pow(Phi,n)/sqrt(5));
}

u64 fibfechada3(int n){
	return round(C1*exp(n*C2));
}
