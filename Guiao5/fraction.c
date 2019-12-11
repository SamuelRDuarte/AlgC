/*******************************************************************************

 Ficheiro de implementa��o do Tipo de Dados Abstracto FRACTION (fraction.c).
 A estrutura de dados de suporte da fra��o � uma estrutura, constitu�da pelos
 campos de tipo inteiro Num e Den para armazenar, respectivamente, o numerador 
 e o denominador da fra��o.

 Autor : Ant�nio Manuel Adrego da Rocha    Data : Abril de 2019
 
 Implementation file of the abstract data type FRACTION (fraction.c). The data
 structure to store the fraction is composed with the integer fields Num and Den
 for storing the fraction's numerator and denominator.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "fraction.h"	/* Ficheiro de interface do TDA - ADT Interface file */

/************** Defini��o da Estrutura de Dados Interna da Fra��o *************/
/**************** Fraction's Internal Data Structure Definition ***************/

struct fraction
{
	int Num;	/* numerador da fra��o - fraction's numerator */
	int Den;	/* denominador da fra��o - fraction's denominator */
};

/*********************** Controlo Centralizado de Erro ************************/
/************************* Centralized Error Control **************************/

static unsigned int Error = OK;	/* inicializa��o do erro - error initialization */

static char *ErrorMessages[] = {
									"sem erro - without error",
									"fracao(fracoes) inexistente(s) - fraction(s) do not exist",
									"memoria esgotada - out of memory",
									"divisor nulo - division by zero",
									"denominador nulo - null denominator"
								};

static char *AbnormalErrorMessage = "erro desconhecido - unknown error";

/************** N�mero de mensagens de erro previstas no m�dulo ***************/
/************** Number of predicted error messages in the module **************/

#define N (sizeof (ErrorMessages) / sizeof (char *))

/************************ Alus�o �s Fun��es Auxiliares ************************/
/*********************** Allusion to Auxiliary Functions **********************/

static PtFraction CreateFraction (void);	/* construtor prim�rio - primary constructor */
static void ReduceFraction (PtFraction);   /* simplificador da fra��o - fraction simplifier */

/*************************** Defini��o das Fun��es ****************************/
/*************************** Function Definitions *****************************/

void FractionClearError (void)
{ Error = OK; }

int FractionErrorCode (void)
{ return Error; }

char *FractionErrorMessage (void)
{
	if (Error < N) return ErrorMessages [Error];
	else return AbnormalErrorMessage;	/* n�o h� mensagem de erro - - no error message */
}

PtFraction FractionCreate (int pnum, int pden)	/* construtor inicializador - initializer constructor */
{
	PtFraction Frac;

    if (pden == 0) { Error = NULL_DEN; return NULL; }
    if (pden < 0) { pnum = -pnum; pden = -pden; }
	if ((Frac = CreateFraction ()) == NULL) { Error = NO_MEM; return NULL; }
	Frac->Num = pnum; Frac->Den = pden;
	if (Frac->Num != 0) ReduceFraction (Frac);
	Error = OK;
	return Frac;
}

int FractionGetNumerator (PtFraction pfrac)
{
	if (pfrac == NULL) { Error = NO_FRACTION; return 0; }
	else { Error = OK; return pfrac->Num; }
}

int FractionGetDenominator (PtFraction pfrac)
{
	if (pfrac == NULL) { Error = NO_FRACTION; return 0; }
	else { Error = OK; return pfrac->Den; }
}

void FractionDestroy (PtFraction *pfrac)	/* destrutor - destructor */
{
	PtFraction Frac = *pfrac;

	if (Frac == NULL) { Error = NO_FRACTION; return; }
	free (Frac);
	*pfrac = NULL;	/* programa��o defensiva - defensive programming */
	Error = OK;
}

PtFraction FractionCopy (PtFraction pfrac)	/* construtor c�pia - copy constructor */
{
	if (pfrac == NULL) { Error = NO_FRACTION; return NULL; }
	else return FractionCreate (pfrac->Num, pfrac->Den);
}

PtFraction FractionSymmetrical (PtFraction pfrac)	/* construtor sim�trico - symmetrical constructor */
{
	if (pfrac == NULL) { Error = NO_FRACTION; return NULL; }
	else return FractionCreate (-pfrac->Num, pfrac->Den);
}

int FractionIsZero (PtFraction pfrac)
{	
	if(pfrac == NULL){Error = NO_FRACTION;return NULL;}
	Error = OK;

	if(pfrac->Num == 0) return 1;
	return 0;
}

int FractionEquals (PtFraction pfrac1, PtFraction pfrac2)
{
	if(pfrac1 == NULL || pfrac2 == NULL){Error = NO_FRACTION;return NULL;}
	Error = OK;

	if (pfrac1->Num == pfrac2->Num && pfrac1->Den == pfrac2->Den) return 1;
	return 0;
}

int FractionCompareTo (PtFraction pfrac1, PtFraction pfrac2)
{
	if(pfrac1 == NULL || pfrac2 == NULL){Error = NO_FRACTION;return NULL;}
	Error = OK;

	double frac1 = pfrac1->Num*pfrac2->Den;
	double frac2 = pfrac2->Num*pfrac1->Den;

	if(frac1>frac2) return 1;
	if(frac1<frac2) return -1;
	return 0;
}

int FractionIsProper (PtFraction pfrac)
{
	if(pfrac == NULL){Error = NO_FRACTION;return NULL;}
	Error = OK;

	if(pfrac->Num<pfrac->Den)
		return 1;
	return 0;
}

void FractionSet (PtFraction pfrac, int pnum, int pden)
{
	if (pfrac == NULL) { Error = NO_FRACTION; return; }
    if (pden == 0) { Error =  NULL_DEN; return; }
    if (pden < 0) { pnum = -pnum; pden = -pden; }
	pfrac->Num = pnum; pfrac->Den = pden;
	if (pfrac->Num != 0) ReduceFraction (pfrac);
	Error = OK;
}

char * FractionToString (PtFraction pfrac)
{
	if (pfrac == NULL) { Error = NO_FRACTION; return 0; }
	char * Str;  
	/* cria a sequ�ncia de caracteres - allocating the string */
	if ((Str = calloc (64, sizeof (char))) == NULL)
	{ Error = NO_MEM; return NULL; }

	sprintf (Str, "%d/%d", pfrac->Num, pfrac->Den);
	Error = OK;
	return Str;
}

/*********************** Defini��o das Fun��es Internas ***********************/
/*********************** Definition of Internal Functions *********************/

/*******************************************************************************
 Fun��o que aloca mem�ria din�mica para a fra��o sem qualquer inicializa��o.
 Este construtor prim�rio serve para ser usado pelas restantes opera��es que criam
 uma nova fra��o.
 
 Function that allocates dynamic memory for the fraction without any initialization.
 This primary constructor is to be used by the remaining operations that create a 
 new fraction.
*******************************************************************************/
static PtFraction CreateFraction (void)
{
	return (PtFraction) malloc (sizeof (struct fraction));
}

/*******************************************************************************
 Fun��o que calcula o m�ximo divisor comum atrav�s do m�todo de Euclides.
 
 Function that calculates the maximum common divisor through the Euclid method.
*******************************************************************************/
static int mdc (int px, int py)
{
	int r, m, n;
	
	if (px >= py) { m = px; n = py;}
	else { m = py; n = px;}
    
    while (n != 0)
    {
        r = m % n; m = n; n = r;
    }
    return m;
} 

/*******************************************************************************
 Fun��o que reduz uma fra��o. Esta fun��o deve ser usada pelas opera��es 
 aritm�ticas e pelo construtor inicializador.
 
 Function that reduces a fraction. This function must be used by the arithmetic 
 operations and by the initializer constructor.
*******************************************************************************/
static void ReduceFraction (PtFraction pfrac)
{
	int MDC;
	
	if (pfrac->Num < 0) MDC = mdc (-pfrac->Num, pfrac->Den);
	else MDC = mdc (pfrac->Num, pfrac->Den);
	
	if (MDC == 1) return;
	else { pfrac->Num /= MDC; pfrac->Den /= MDC; }
}
