/*******************************************************************************
 Ficheiro de implementa��o do Tipo de Dados Abstracto EGYPTIAN FRACTION.
 A estrutura de dados de suporte da fra��o eg�pcia � uma estrutura, constitu�da
 pelo campo inteiro Size para indicar o n�mero de fra��es existentes, pelo campo
 inteiro Complete para indicar se a fra��o eg�pcia est� completa/incompleta, e
 pelo array que armazena as fra��es unit�rias contituintes da fra��o eg�pcia.

 Autor : Ant�nio Manuel Adrego da Rocha    Data : Mar�o de 2019
 
 Implementation file of the abstract data type EGYPTIAN FRACTION.
 The supporting data structure of the egyptian fraction is a structure, constituted
 by the integer field Size to indicate the number of fractions, by the integer field
 Complete to indicate if the egyptian fraction is complete/incomplete, and by the
 array that stores the unit fractions constituent of the egyptian fraction.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "egyptianfraction.h"	/* Ficheiro de interface do TDA - ADT Interface file */

/********** Defini��o da Estrutura de Dados Interna da Fra��o Eg�pcia *********/
/************ Egiptian Fraction's Internal Data Structure Definition **********/

struct egyptianfraction
{
	int Size;	/* n�mero de fra��es unit�rias - number of unit fractions */
	int Complete;	/* fra��o eg�pcia completa/incompleta - complete/incomplete egyptian fraction */
	PtFraction Array[MAX_SIZE];  /* array de fra��es unit�rias - array of unit fractions */
};

/*********************** Controlo Centralizado de Erro ************************/
/************************* Centralized Error Control **************************/

static unsigned int Error = OK;	/* inicializa��o do erro - error initialization */

static char *ErrorMessages[] = {
									"sem erro - without error",
									"fracao(fracoes) inexistente(s) - fraction(s) do not exist",
									"memoria esgotada - out of memory",
									"fracao nao propria - fraction not proper",
									"denominador nulo - null denominator",
									"indice errado - bad index",
									"ponteiro nulo - null pointer"
								};

static char *AbnormalErrorMessage = "erro desconhecido - unknown error";

/************** N�mero de mensagens de erro previstas no m�dulo ***************/
/************** Number of predicted error messages in the module **************/

#define N (sizeof (ErrorMessages) / sizeof (char *))

/************************ Alus�o �s Fun��es Auxiliares ************************/
/*********************** Allusion to Auxiliary Functions **********************/

static PtFraction CreateUnitFraction (PtFraction *);

/*************************** Defini��o das Fun��es ****************************/
/*************************** Function Definitions *****************************/

void EgyptianFractionClearError (void)
{ Error = OK; }

int EgyptianFractionError (void)
{ return Error; }

char *EgyptianFractionErrorMessage (void)
{
	if (Error < N) return ErrorMessages [Error];
	else return AbnormalErrorMessage;	/* n�o h� mensagem de erro - - no error message */
}

PtEgyptianFraction EgyptianFractionCreate (PtFraction pfraction)	/* construtor inicializador - initializer constructor */
{
	PtEgyptianFraction Frac;
	if (pfraction==NULL) { Error = NO_FRACTION; return NULL;}
	
	if (FractionIsProper(pfraction)==0){Error=NOT_PROPER;return NULL;}
	if(FractionGetNumerator(pfraction)<=0){Error =NOT_PROPER; return NULL;}

	PtFraction copy; 
	if((copy = FractionCopy(pfraction))==NULL){Error=NO_MEM;return NULL;}
	if ((Frac = malloc(sizeof(struct egyptianfraction)))==NULL){Error=NO_MEM; FractionDestroy(&copy); return NULL;}

	int complete = 0;
	int i = 0;
	while(i<MAX_SIZE)
	{
		PtFraction ptFrac = CreateUnitFraction(&copy);
		//printf("%s\n",FractionToString(ptFrac));
		//scanf("%*c");
		if(ptFrac == NULL){
			Error = NO_MEM;
			FractionDestroy(&copy);
			EgyptianFractionDestroy(&Frac);
			return NULL;
		}
			

		Frac->Array[i++] = ptFrac;
		if(copy == NULL) break;
		if(FractionIsNull(copy)) {complete=1;break;}	
			
	}	

	Frac->Size = i;
	Frac->Complete = complete;
	Error=OK;
	FractionDestroy(&copy);
	return Frac;
}

void EgyptianFractionDestroy (PtEgyptianFraction *pegyp)	/* destrutor - destructor */
{
	PtEgyptianFraction Frac = *pegyp;

	if(Frac==NULL){Error=NO_FRACTION;return;}
	for(int i = 0; i < Frac->Size ; i++)
	{
		FractionDestroy(&Frac->Array[i]);
	}
	free(Frac);
	*pegyp=NULL;
	Error = OK;

}

int  EgyptianFractionGetSize (PtEgyptianFraction pegyp)
{
	if(pegyp==NULL){Error=NO_FRACTION;return NULL;}
	Error = OK;
	return pegyp->Size;
}

int  EgyptianFractionIsComplete (PtEgyptianFraction pegyp)
{
	if(pegyp==NULL){Error=NO_FRACTION;return NULL;}
	Error = OK;
	return pegyp->Complete;
}

PtEgyptianFraction EgyptianFractionCopy (PtEgyptianFraction pegyp)	/* construtor c�pia - copy constructor */
{
	if(pegyp == NULL){Error = NO_FRACTION; return NULL;}
	PtFraction frac = EgyptianFractionToFraction(pegyp);

	PtEgyptianFraction Frac = EgyptianFractionCreate(frac);
	if(Frac == NULL){Error = NO_MEM;return NULL;}
	Error = OK;
	return Frac;
}

PtFraction EgyptianFractionToFraction (PtEgyptianFraction pegyp)
{
	if(pegyp == NULL){Error = NO_FRACTION; return NULL;}
	PtFraction frac = FractionCreate(0,1);
	if (frac == NULL){Error = NO_MEM; return NULL;}
	
	for(int i = 0; i < pegyp->Size; i++)
	{
		frac = FractionAddition(frac,pegyp->Array[i]);
	}
	Error = OK;
	return frac;
}

int EgyptianFractionEquals (PtEgyptianFraction pegy1, PtEgyptianFraction pegy2)
{
	if (pegy1 == NULL ||pegy2 == NULL){Error = NO_FRACTION;return NULL;}
	PtFraction frac1 = EgyptianFractionToFraction(pegy1);
	PtFraction frac2 = EgyptianFractionToFraction(pegy2);

	Error = OK;
	return FractionEquals(frac1,frac2);
}

int EgyptianFractionBelongs (PtEgyptianFraction pegyptian, PtFraction pfraction)
{
	if(pegyptian == NULL || pfraction == NULL){Error = NO_FRACTION; return NULL;}
	if(FractionIsProper(pfraction) == 0){Error= NOT_PROPER; return NULL;}
	Error = OK;
	for(int i = 0; i < pegyptian->Size; i++)
	{
		if (FractionEquals(pfraction,pegyptian->Array[i])) {
			return 1;
		}
	}
	return 0;
	
}

PtFraction EgyptianFractionGetPos (PtEgyptianFraction pegyp, int pindex)
{
	if(pegyp == NULL){Error = NO_FRACTION; return NULL;}
	if(pindex < 0 || pindex > pegyp->Size){Error = BAD_INDEX; return NULL;}

	Error = OK;
	return pegyp->Array[pindex];
}

/*********************** Defini��o das Fun��es Internas ***********************/
/*********************** Definition of Internal Functions *********************/

/*******************************************************************************
 Fun��o iterativa para decompor uma fra��o no seu primeiro termo unit�rio. Tem
 como par�metro de entrada uma fra��o (que se assume uma fra��o pr�pria positiva)
 e devolve a maior fra��o unit�ria existente ou uma refer�ncia nula no caso de 
 falta de mem�ria din�mica para alocar uma nova fra��o. A fun��o altera a fra��o 
 passada no par�metro de entrada para o valor do resto da fra��o que precisa de 
 continuar a ser convertida. Caso haja overflow no c�lculo do denominador desta 
 fra��o restante ent�o a fra��o de entrada fica com a refer�ncia nula para 
 indicar a impossibilidade de fazer a extra��o da fra��o unit�ria seguinte.
 Como o par�metro de entrada fica corrompido pela execu��o da fun��o, qualquer 
 fun��o que a utilize deve - depois das valida��es necess�rias - fazer uma c�pia 
 da fra��o e usar a c�pia, n�o se esquecendo de eliminar a fra��o no fim.
 
 Iterative function to decompose a fraction into its first unit term. Has as input
 parameter a fraction (assuming a positive proper fraction) and returns the largest
 existing unit fraction or a zero reference in the case of lack of dynamic memory 
 to allocate a new fraction. The function changes the input fraction to the value 
 of the remainder of the fraction that needs to continue to be converted. If there 
 is overflow in calculating the denominator of this remaining fraction, then the 
 input parameter is destoyed and assumes the null reference for indicate the 
 impossibility of extracting the next unit fraction. Because the input parameter is
 corrupted by the function execution, any function that uses it must - after the 
 necessary validations - make a copy of the fraction and use the copy, not forgetting
 to eliminate the fraction before terminating its execution.
*******************************************************************************/
static PtFraction CreateUnitFraction (PtFraction *pfraction)
{
	PtFraction UnitFraction; int Num, Den, NewNum, NewDen;

	Num = FractionGetNumerator (*pfraction);
	Den = FractionGetDenominator (*pfraction);

	NewNum = 1; NewDen = Den/Num;
	if (Den%Num != 0) NewDen++;

	if ((UnitFraction = FractionCreate (NewNum, NewDen)) == NULL)
	{ Error = FractionError (); return NULL; }

	unsigned long long Denominator = Den * NewDen;

	if (Denominator > INT_MAX) FractionDestroy (pfraction);
	else
	{
		NewNum = (-Den) % Num;
		if (NewNum < 0) NewNum += Num;
		NewDen = (int) Denominator;
		FractionSet (*pfraction, NewNum, NewDen);
		if (FractionError () != OK) FractionDestroy (pfraction);
	}

	return UnitFraction;
}
