/*******************************************************************************
 Ficheiro de implementa��o do Tipo de Dados Abstracto EGYPTIAN FRACTION.
 A estrutura de dados de suporte da fra��o eg�pcia � uma estrutura, constitu�da
 pelo campo inteiro Size para indicar o n�mero de fra��es existentes, pelo campo
 inteiro Complete para indicar se a fra��o eg�pcia est� completa/incompleta, e
 pelos ponteiros Head e Tail para aceder � lista ligada que armazena as fra��es 
 unit�rias contituintes da fra��o eg�pcia.

 Autor : Ant�nio Manuel Adrego da Rocha    Data : Mar�o de 2019
 
 Implementation file of the abstract data type EGYPTIAN FRACTION.
 The supporting data structure of the egyptian fraction is a structure, constituted
 by the integer field Size to indicate the number of fractions, by the integer field
 Complete to indicate if the egyptian fraction is complete/incomplete, and by the
 Head and Tail pointers to access the linked list that stores the unit fractions
 constituent of the egyptian fraction.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "egyptianfraction.h"	/* Ficheiro de interface do TDA - ADT Interface file */

/********** Defini��o da Estrutura de Dados Interna da Fra��o Eg�pcia *********/
/************ Egiptian Fraction's Internal Data Structure Definition **********/

typedef struct node *PtNode;
struct node /* defini��o do n� da lista ligada - link list node definition */
{
	PtFraction PtElem;  /* elemento da lista - list element */
	PtNode PtNext;  /* ponteiro para o n� seguinte - next node pointer */
};

struct egyptianfraction
{
	int Size;	/* n�mero de fra��es unit�rias - number of unit fractions */
	int Complete;	/* fra��o eg�pcia completa/incompleta - complete/incomplete egyptian fraction */
	PtNode Head, Tail;  /* ponteiros para a lista de fra��es unit�rias - pointers to the list of unit fractions */
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

PtNode NodeCreate (PtFraction);
void NodeDestroy (PtNode *);
void ListDestroy (PtNode *);
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
	PtNode previousNode;

	while(complete == 0){
		PtFraction ptFrac = CreateUnitFraction(&copy);
		
		if(ptFrac == NULL){
			Error = NO_MEM;
			FractionDestroy(&copy);
			EgyptianFractionDestroy(&Frac);
			return NULL;
		}
		
		PtNode newNode = NodeCreate(ptFrac);
		if(newNode == NULL){Error = NO_MEM;return NULL;}

		if(i == 0){Frac->Head=newNode;}
		else{
			previousNode->PtNext=newNode;
		}

		previousNode = newNode;
		i++;

		if(copy == NULL){Frac->Tail=newNode; break;}
		if(FractionIsNull(copy)) {Frac->Tail=newNode; complete=1;}	
			
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
	
	ListDestroy(&Frac->Head);

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
	PtNode no = pegyp->Head;
	int i = 0;
	while(i<pegyp->Size){
		frac = FractionAddition(frac,no->PtElem);
		no = no->PtNext;
		i++;
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

	PtNode no = pegyptian->Head;
	int i = 0;

	while(i < pegyptian->Size){

		if (FractionEquals(pfraction,no->PtElem)) {
			return 1;
		}
		no= no->PtNext;
		i++;
	}
	return 0;
}

PtFraction EgyptianFractionGetPos (PtEgyptianFraction pegyp, int pindex)
{
	if(pegyp == NULL){Error = NO_FRACTION; return NULL;}
	if(pindex < 0 || pindex > pegyp->Size){Error = BAD_INDEX; return NULL;}
	
	int i = 0;
	PtNode no = pegyp->Head;
	while(i<pindex){
		
		no = no->PtNext;
		i++;
	}

	Error = OK;
	return no->PtElem;
	
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

/*******************************************************************************
 Fun��o auxiliar para criar um n� da lista ligada. Valores de erro: OK ou NO_MEM.
 
 Auxiliary function to create a list node. Error codes: OK or NO_MEM.
*******************************************************************************/
PtNode NodeCreate (PtFraction pelem)	/* aloca��o do n� - node allocation */
{
	PtNode Node;

	if ((Node = (PtNode) malloc (sizeof (struct node))) == NULL)
    { Error = NO_MEM; return NULL; }

	Node->PtElem = pelem;		/* copiar o elemento - copy the element */
	Node->PtNext = NULL;	/* apontar para a frente para NULL - next is null */

	Error = OK;
	return Node;
}

/*******************************************************************************
 Fun��o auxiliar para libertar um n� da lista ligada. Valores de erro: OK ou NULL_PTR.
 
 Auxiliary function to free a link list node. Error codes: OK or NULL_PTR.
*******************************************************************************/
void NodeDestroy (PtNode *pnode)	/* liberta��o do n� - node releasing */
{
	if (*pnode == NULL) { Error = NULL_PTR; return; }
	free (*pnode);	/* liberta��o do n� - free the node */
	*pnode = NULL;	/* colocar o ponteiro a nulo - set the pointer to null */
	Error = OK;
}

/*******************************************************************************
 Fun��o auxiliar para destruir uma lista ligada. Valores de erro: OK ou NULL_PTR.
 
 Auxiliary function to destroy a link list. Error codes: OK or NULL_PTR.
*******************************************************************************/
void ListDestroy (PtNode *phead)
{
	PtNode TmpHead = *phead; PtNode Node;

	if (TmpHead == NULL) { Error = NULL_PTR; return; }
	while (TmpHead != NULL)
	{
		Node = TmpHead; TmpHead = TmpHead->PtNext;
		FractionDestroy (&Node->PtElem);
		NodeDestroy (&Node);
	}
	Error = OK; 
}

