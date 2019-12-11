/*******************************************************************************

 Ficheiro de implementa��o do Tipo de Dados Abstracto Arvore Binaria de Pesquisa
 de Fra��es. A estrutura de dados da abp � uma estrutura, constitu�da pelo campo
 de tipo inteiro Size para armazenar o n�mero de n�s existentes na �rvore e pelo
 campo Root de tipo ponteiro para a raiz da �rvore.
 
 Implementation File of the Abstract Data Type Binary Search Tree of Fractions.
 The bst data structure is a structure, consisting of the integer field Size to
 store the number of nodes stored in the tree and the pointer field Root to point
 to the root of the tree. 

 Autor : Ant�nio Manuel Adrego da Rocha    Data : Maio de 2019

*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "abp.h"
#include "fraction.h"
#include "stack.h"
#include "queue.h"

typedef struct abpnode *PtABPNode;
struct abpnode /* estrutura do n� da �rvore - bst node */
{
  PtABPNode PtLeft;  /* ponteiro para o no esquerdo da arvore - left subtree */
  PtABPNode PtRight;  /* ponteiro para o no direito da arvore - right subtree */
  PtFraction PtElem;  /* ponteiro para o elemento (de tipo fra��o) - pointer to fraction */
};

struct abp	/* estrutura da �rvore abp - bst data structure */
{
	int Size;	/* numero de elemntos da �rvore - tree size */
	PtABPNode Root; /* ponteiro para a raiz da �rvore - pointer to bst root */
};

/*********************** Controlo Centralizado de Erro ************************/

static unsigned int Error = OK;    /* inicial		printf("%s\n",FractionToString(Node->PtElem));
iza��o do erro - error initialization */

static char *ErrorMsg[] = {	"sem erro - without error", "abp inexistente - bst nonexistent",
							"memoria esgotada - out of memory", "ponteiro nulo - null pointer",
							"arvore vazia - empty tree", "elemento repetido - repeated element",
                            "elemento inexistente - nonexistent element",
							"ficheiro inexistente - nonexistent file" };

static char *AbnormalErrorMsg = "erro desconhecido - unknown error";

/******* N representa o n�mero de mensagens de erro previstas no m�dulo *******/

#define N (sizeof (ErrorMsg) / sizeof (char *))

/************************ Alus�o �s Fun��es Auxiliares ************************/

static PtABPNode ABPNodeCreate (PtFraction);
static void ABPNodeDestroy (PtABPNode *);
static void DestroyTree (PtABPNode *);
unsigned int HeightTree (PtABPNode);
static PtFraction MaxTree (PtABPNode);
static PtFraction SearchTree (PtABPNode, PtFraction);
static void InsertTree (PtABPNode *, PtFraction);
static void ListInOrder (PtABPNode, PtFraction [], unsigned int *);
static void Balance (PtABP, PtFraction [], int , int);
static void DisplayTree (PtABPNode, unsigned int);
static void PrintTree (PtABPNode);
static void StoreFilePreOrder (PtABPNode, FILE *);
static PtQueue ABPFillQueue (PtABP);

/************** Fun��es das aulas 9 e 10 - Work of lessons 9 & 10 *************/ 

/* construtor da abp - bst constructor */
PtABP ABPCreate (void)
{
	PtABP tree = (PtABP) malloc(sizeof(struct abp));
	if(tree == NULL){Error = NO_ABP; return NULL;}
	tree->Size=0;
	tree->Root = NULL;
	Error = OK;
	return tree;
}

/* destrutor da abp - bst destructor */
void ABPDestroy (PtABP *ptree)
{
	PtABP Tree = *ptree;
	if (Tree == NULL) { Error = NO_ABP; return; }

	/* invoca��o da fun��o recursiva que atravessa a �rvore e liberta a mem�ria */
	/* invocation of the recursive function that traverse the tree and frees the memory */
	if (Tree->Size != 0) DestroyTree (&Tree->Root);

	free (Tree);
	Error = OK;
	*ptree = NULL;
}

/*******************************************************************************
  Fun��o recursiva que liberta a mem�ria din�nica ocupada pela arvore e elementos.
  Recursive function that frees the dynamic memory occupied by the tree and elements.
*******************************************************************************/
static void DestroyTree (PtABPNode *proot)
{	
	if(*proot !=NULL){
		DestroyTree(&(*proot)->PtLeft);
		DestroyTree(&(*proot)->PtRight);
		FractionDestroy(&(*proot)->PtElem);
		ABPNodeDestroy(proot);
	}
	
}


/* pesquisar a abp - bst search */
int ABPSearch (PtABP ptree, PtFraction pelem)
{
	
	if (ptree == NULL) { Error = NO_ABP; return 0; }
	if (ptree->Size == 0){Error = ABP_EMPTY; return 0;}
	if(pelem == NULL) {Error = NULL_PTR; return 0;}

	if(SearchTree(ptree->Root,pelem)== NULL){Error = NO_ELEM; return 0;}
	else {Error = OK; return 1;}

}

/*******************************************************************************
  Fun��o recursiva que pesquisa o elemento pretendido na arvore.
  Recursive function that searches the desired element in the tree.
*******************************************************************************/

static PtFraction SearchTree (PtABPNode proot, PtFraction pelem)
{
	if (proot == NULL) return NULL;	

	if (FractionEquals(proot->PtElem,pelem)) return proot->PtElem;	
	else{	
		if (FractionCompareTo(proot->PtElem,pelem) > 0)
			return SearchTree (proot->PtLeft, pelem);
		else	 
			return SearchTree (proot->PtRight, pelem);
	}
}

/* inser��o n�o recursiva na abp - non recursive in bst */
void ABPInsert (PtABP ptree, PtFraction pelem)	
{

	/* Insira o seu código - Insert your code */
	if(ptree == NULL){ Error = NO_ABP; return; }
	if(pelem == NULL){ Error = NULL_PTR; return; }

	PtABPNode NewNode = ABPNodeCreate(pelem);
	if(NewNode == NULL) { Error = NO_MEM; return; }

	NewNode->PtElem = FractionCopy(pelem);
	if(NewNode->PtElem == NULL) { return; }
	NewNode->PtLeft = NULL;
	NewNode->PtRight = NULL;

	PtABPNode Node = ptree->Root;
	PtABPNode Previous = NULL;

	if(Node == NULL)
	{
		ptree->Root = NewNode;
		ptree->Size++;
		return;
	}

	while(Node != NULL){
		int result = FractionCompareTo(Node->PtElem, pelem);
		if(result > 0){
			Previous = Node;
			Node = Node->PtLeft;
		}else if(result < 0){
			Previous = Node;
			Node = Node->PtRight;
		}else{
			Error = REP_ELEM;
			return;
		}
	}
	int result2 = FractionCompareTo(Previous->PtElem, pelem);
	if(result2 > 0){
		Previous->PtLeft = NewNode;
	}else{
		Previous->PtRight = NewNode;
	}
	ptree->Size++;
	Error = OK;


}

/* sele��o n�o recursiva do m�nimo da abp - non recursive selection of bst minimum */
PtFraction ABPMin (PtABP ptree)
{
	if (ptree == NULL) { Error = NO_ABP; return NULL; }
	if (ptree->Size == 0){Error = ABP_EMPTY; return NULL;}
	PtABPNode Node = ptree->Root;

	while (Node->PtLeft != NULL)
	{
		Node = Node->PtLeft;
	}
	Error = OK;
	return Node->PtElem;
}

/* sele��o recursiva do m�ximo da abp - recursive selection of bst maximum */
PtFraction ABPMax (PtABP ptree)
{
	if (ptree == NULL) { Error = NO_ABP; return NULL; }
	if (ptree->Size == 0){Error = ABP_EMPTY; return NULL;}
	Error = OK;
	return MaxTree(ptree->Root);
}

/*******************************************************************************
  Fun��o recursiva que seleciona o elemento m�ximo da arvore.
  Recursive function that selects the maximum element of the tree.
*******************************************************************************/

static PtFraction MaxTree (PtABPNode proot) 
{
	
	if (proot->PtRight == NULL) return proot->PtElem;
			else return MaxTree (proot->PtRight);
}

/* construtor c�pia da abp fazendo uma travessia n�o recursiva em pr�-ordem */
/* bst copy constructor making a non recursive traverse in preorder */
PtABP ABPCopy (PtABP ptree)
{
	PtABP copyTree;

	if (ptree == NULL) { Error = NO_ABP; return NULL; }
	if((copyTree = ABPCreate())==NULL){Error = NO_MEM; return NULL;}
	if (ptree->Root == NULL){Error = OK;return copyTree;}
	

	PtStack Pilha; 
	PtABPNode Node = ptree->Root;
	PtFraction frac;

	if((Pilha = StackCreate(sizeof(PtABPNode))) == NULL){Error = NO_MEM;ABPDestroy(&copyTree);return NULL;}

	StackPush(Pilha,&Node);
	while (!StackIsEmpty(Pilha))
	{
		StackPop(Pilha,&Node);
		
		if((frac =FractionCopy(Node->PtElem))==NULL){Error = NO_MEM;ABPDestroy(&copyTree);StackDestroy(&Pilha);return NULL;}
		ABPInsert(copyTree,frac);
		
		
		if(Node->PtRight != NULL) StackPush(Pilha, &Node->PtRight); 
		
		if(Node->PtLeft != NULL) StackPush(Pilha, &Node->PtLeft);
		
	}
	
	Error = OK;
	StackDestroy(&Pilha);
	return copyTree;
}

/*******************************************************************************
 Fun��o auxiliar para preencher uma fila com os elementos da abp. Cria uma fila 
 com os elementos retirados de cada n�vel da �rvore. Devolve a refer�ncia da fila
 criada ou NULL em caso de inexist�ncia de mem�ria. Se a �rvore estiver vazia deve
 devolver uma fila vazia. Valores de erro: OK, NO_ABP ou NO_MEM.

 Auxiliar function for filling a queue with the elements of the bst. Creates a queue
 with the elements taken from each level of the tree. Returns a reference to the
 queue or NULL if there isn't enough memory. If the tree is empty it should return 
 an empty queue. Error codes: OK, NO_ABP or NO_MEM.
*******************************************************************************/
static PtQueue ABPFillQueue (PtABP ptree)
{
	PtABPNode Node = ptree->Root; PtQueue Queue; PtQueue QueueTemp;

	if (ptree == NULL) { Error = NO_ABP; return NULL;	}
	if ((Queue = QueueCreate (sizeof (PtABPNode))) == NULL) { Error = NO_MEM ; return NULL; }
	if ((QueueTemp = QueueCreate (sizeof (PtABPNode))) == NULL) { Error = NO_MEM ; return NULL; }

	QueueEnqueue (QueueTemp, &Node);	

	while (!QueueIsEmpty (QueueTemp))
	{
		QueueDequeue (QueueTemp, &Node);
		QueueEnqueue(Queue,&Node);


		if (Node->PtLeft != NULL) QueueEnqueue (QueueTemp, &Node->PtLeft);

		
		if (Node->PtRight != NULL) QueueEnqueue (QueueTemp, &Node->PtRight);
	}

	QueueDestroy (&QueueTemp);	
  Error = OK;
	return Queue;
}

/* Reuni�o de duas abps usando a fun��o ABPFillQueue - Reunion of two bsts using ABPFillQueue */
void ABPReunion (PtABP ptree1, PtABP ptree2)
{
	if (ptree1 == NULL) { Error = NO_ABP; return;	}
	if (ptree2 == NULL) { Error = NO_ABP; return;	}
	
	PtQueue Queue = ABPFillQueue(ptree2);
	if(Queue == NULL){Error = NO_MEM; return;}
	
	PtABPNode Node;
	while (!QueueIsEmpty(Queue))
	{
		QueueDequeue(Queue,&Node);

		ABPInsert(ptree1,Node->PtElem);
	}
	QueueDestroy(&Queue);
	Error = OK;	
}

/* Diferen�a de duas abps usando a fun��o ABPFillQueue - Difference of two bsts using ABPFillQueue */
void ABPDifference (PtABP ptree1, PtABP ptree2)
{
	if (ptree1 == NULL) { Error = NO_ABP; return;	}
	if (ptree2 == NULL) { Error = NO_ABP; return;	}
	
	PtQueue Queue = ABPFillQueue(ptree2);
	if(Queue == NULL){Error = NO_MEM; return;}
	
	PtABPNode Node;
	while (!QueueIsEmpty(Queue))
	{
		QueueDequeue(Queue,&Node);
		ABPDelete(ptree1,Node->PtElem);
	}
	Error = OK;	
}

/* Interse��o de duas abps usando a fun��o ABPFillQueue - Intersection of two bsts using ABPFillQueue */
void ABPIntersection (PtABP ptree1, PtABP ptree2)
{
	if (ptree1 == NULL) { Error = NO_ABP; return;	}
	if (ptree2 == NULL) { Error = NO_ABP; return;	}

	PtQueue Queue = ABPFillQueue(ptree1);
	if(Queue == NULL){Error = NO_MEM; return;}

	PtABPNode Node;
	while (!QueueIsEmpty(Queue))
	{
		QueueDequeue(Queue,&Node);
		if(!ABPSearch(ptree2,Node->PtElem)){ printf("Nao pertence\n"); ABPDelete(ptree1,Node->PtElem);}
	}
	QueueDestroy(&Queue);
	Error = OK;
	
}

/******************************************************************************
 Fun��es e fun��es internas auxiliares que j� est�o implementadas (que podem ser
 usadas) mas que em caso algum podem ser alteradas.
 
 Functions and auxiliary internal functions that are already implemented (that 
 can be used) but that cannot be changed.
******************************************************************************/

/*************************** Defini��o das Fun��es ****************************/ 

int ABPErrorCode (void)
{
    return Error;
}

/******************************************************************************/

char *ABPErrorMessage (void)
{
    if (Error < N) return ErrorMsg[Error];
    else return AbnormalErrorMsg;    /* n�o h� mensagem de erro - no error message */
}

/******************************************************************************/

int ABPEmpty (PtABP ptree)
{
	if (ptree == NULL) { Error = NO_ABP; return 0; }
	else { Error = OK; return ptree->Root == NULL; }
}

/******************************************************************************/

unsigned int ABPSize (PtABP ptree)                                    
{
	if (ptree == NULL) { Error = NO_ABP; return 0; }
	else { Error = OK; return ptree->Size; }
}

/******************************************************************************/

unsigned int ABPHeight (PtABP ptree)
{
	if (ptree == NULL) { Error = NO_ABP; return 0; }
	else { Error = OK; return HeightTree (ptree->Root); }
}

/******************************************************************************/

void ABPDelete (PtABP ptree, PtFraction pelem)	/* remo��o repetitiva */
{
	if (ptree == NULL) { Error = NO_ABP; return ; } /* arvore inexistente */
	if (ptree->Size == 0) { Error = ABP_EMPTY; return ; }	/* arvore vazia */
	if (pelem == NULL) { Error = NULL_PTR; return ; } /* ponteiro invalido */

	PtABPNode Del = ptree->Root, Prev = NULL, Node, Temp, PrevTemp;

	while (Del != NULL && !FractionEquals (Del->PtElem, pelem))
	{			/* travessia at� encontrar o elemento a remover */
		Prev = Del;
		if (FractionCompareTo (Del->PtElem, pelem) > 0) Del = Del->PtLeft;
		else Del = Del->PtRight;
	}

	if (Del == NULL) { Error = NO_ELEM; return; }	/* elemento inexistente na arvore */
	Node = Del;

	Error=OK;
	if (Node->PtRight == NULL) Node = Node->PtLeft;	/* � esquerda */
	else	if (Node->PtLeft == NULL) Node = Node->PtRight;	/* � direita */
			else	/* com subarvores esquerda e direita */
			{	/* procurar menor elemento da subarvore direita */
				Temp = Node->PtRight; PrevTemp = Node;
				while (Temp->PtLeft != NULL)
				{ PrevTemp = Temp; Temp = Temp->PtLeft; }

				/* substituir pelo menor elemento */
				FractionSet (Node->PtElem, FractionGetNumerator (Temp->PtElem), FractionGetDenominator (Temp->PtElem));
				
				/* desligar o menor elemento e ajustar as liga��es */
				if (PrevTemp == Node) PrevTemp->PtRight = Temp->PtRight;
				else PrevTemp->PtLeft = Temp->PtRight;
				ABPNodeDestroy (&Temp);
				ptree->Size--;
				return;
			}
	/* ajustar o pai do elemento removido que s� tem um filho */
	if (Del == ptree->Root) ptree->Root = Node;	/* se foi eliminado da raiz */
	else	if (Prev->PtLeft == Del) Prev->PtLeft = Node;
			else Prev->PtRight = Node;
	ABPNodeDestroy (&Del);
	ptree->Size--;
}

/******************************************************************************/

PtABP ABPCreateFile (char *nomef)
{
    PtABP ABP; FILE *PtF; PtFraction Fraction;
	unsigned int NElem, I; int Num, Den;

    /* abertura com validacao do ficheiro para leitura - opening the file */
    if ( (PtF = fopen (nomef, "r")) == NULL) { Error = NO_FILE; return NULL; }

    ABP = ABPCreate ();    /* cria a arvore vazia - create an empty tree */

    /* leitura do n�mero de elementos do ficheiro - read the number of elements */
    fscanf (PtF, "%u", &NElem);
    if (NElem < 1) { Error = OK; fclose (PtF); return NULL; }

    /* leitura do ficheiro linha a linha - read the file line by line */
    for (I = 0; I < NElem; I++)
    {
    	/* ler a fra��o do ficheiro - read fraction from file */
        fscanf (PtF, "%d %d", &Num, &Den);

        /* criar a fra��o - create fraction */
        Fraction = FractionCreate (Num, Den);
        if (FractionErrorCode () == NO_MEM)
        { ABPDestroy (&ABP); Error = NO_MEM; return NULL; }

        /* inserir a fra��o na abp - insert fraction in bst */
        ABPInsertRec (ABP, Fraction);
        if (ABPErrorCode () == NO_MEM)
        { ABPDestroy (&ABP); Error = NO_MEM; return NULL; }
    }

    fclose (PtF);  /* fecho do ficheiro - close file */

    Error = OK;
    return ABP;  /* devolve a arvore criada - return the tree */
}

/******************************************************************************/

void ABPInsertRec (PtABP ptree, PtFraction pelem)  /* inser��o recursiva */
{
	if (ptree == NULL) { Error = NO_ABP; return ; } /* arvore inexistente */
	if (pelem == NULL) { Error = NULL_PTR; return ; } /* elemento inexistente */

	InsertTree (&ptree->Root, pelem);
	if (ABPErrorCode () == OK) ptree->Size++;
}

/******************************************************************************/

void ABPStoreFile (PtABP ptree, char *nomef)
{
    FILE *PtF; unsigned int NElem;

	if (ptree == NULL) { Error = NO_ABP; return ; }
	if (ptree->Root == NULL) { Error = ABP_EMPTY; return ; }

    /* abertura com validacao do ficheiro para escrita */
    if ((PtF = fopen (nomef, "w")) == NULL) { Error = NO_FILE; return ; }

    NElem = ABPSize (ptree);
    /* escrita do n�mero de elementos da arvore no ficheiro */
    fprintf (PtF, "%u\n", NElem);

    /* escrita dos elementos da arvore no ficheiro */
    StoreFilePreOrder (ptree->Root, PtF);

    fclose (PtF);  /* fecho do ficheiro */
    Error = OK;
}

/******************************************************************************/

PtABP ABPBalance (PtABP ptree)
{
	PtFraction *List; PtABP NewABP;
	unsigned int Count = 0, n = ABPSize (ptree);	/* n�mero de nos */

	if (ptree == NULL) { Error = NO_ABP; return NULL; }	/* a arvore n�o existe */
	if (ptree->Size == 0) { Error = ABP_EMPTY; return NULL; }	/* a arvore est� vazia */
	if ((NewABP = ABPCreate ()) == NULL) { Error = NO_MEM; return NULL; }
				/* criar a sequ�ncia */
	if ((List = (PtFraction *) calloc (n, sizeof (PtFraction))) == NULL)
	{ Error = NO_MEM ; ABPDestroy (&NewABP); return NULL; }	

    Error = OK;
	ListInOrder (ptree->Root, List, &Count);	/* preencher a sequ�ncia */
	Balance (NewABP, List, 0, n-1);	/* balancear a arvore */
	if (Error == NO_MEM) { free (List); ABPDestroy (&NewABP); return NULL; }

	free (List);	/* libertar a sequ�ncia */
	return NewABP;
}

/******************************************************************************/

void ABPDisplay (PtABP ptree)
{
	if (ptree == NULL) { Error = NO_ABP; return; }
	if (ptree->Root == NULL) { Error = ABP_EMPTY; return; }

	DisplayTree (ptree->Root, 1);
	Error = OK;
}

/******************************************************************************/

void ABPPrint (PtABP ptree)
{
	if (ptree == NULL) { Error = NO_ABP; return; }
	if (ptree->Root == NULL) { Error = ABP_EMPTY; return; }

	PrintTree (ptree->Root);
	Error = OK;
}

/******************************************************************************/

int ABPEquals (PtABP ptree1, PtABP ptree2) /* verifica se duas arvores sao iguais */
{
    PtQueue Queue; int Enqueue;

    if (ptree1 == NULL || ptree2 == NULL) { Error = NO_ABP; return 0; }
	if (ptree1->Size != ptree2->Size) { Error = OK; return 0; } 
	
    if ((Queue = QueueCreate (sizeof (PtABPNode))) == NULL) { Error = NO_MEM; return 0; }
	
	/* Ser�o percorridos todos os elementos da 2� �rvore e, */
	/* se n�o pertencerem � 1� ent�o as �rvores n�o s�o iguais */ 
	PtABPNode Node = ptree2->Root;
    Enqueue = QueueEnqueue (Queue, &Node);
	if (Enqueue == NO_MEM)
	{ Error = NO_MEM ; QueueDestroy (&Queue); return 0; }

    while (!QueueIsEmpty (Queue))
    {
        QueueDequeue (Queue, &Node);

		if (SearchTree (ptree1->Root, Node->PtElem) == NULL)
		{ QueueDestroy (&Queue); Error = OK; return 0; }
		
        if (Node->PtLeft != NULL)
		{
			Enqueue = QueueEnqueue (Queue, &Node->PtLeft);
			if (Enqueue == NO_MEM)
			{ Error = NO_MEM; QueueDestroy (&Queue); return 0; } 			
		}
        if (Node->PtRight != NULL)
		{
			Enqueue = QueueEnqueue (Queue, &Node->PtRight);
			if (Enqueue == NO_MEM)
			{ Error = NO_MEM; QueueDestroy (&Queue); return 0; } 			
		}		
		
    }

    QueueDestroy (&Queue);
	Error=OK;
	return 1;
}

/************************* Fun��es internas auxiliares ************************/

/*******************************************************************************
  Fun��o que cria o n� da arvore bin�ria de pesquisa.
*******************************************************************************/
static PtABPNode ABPNodeCreate (PtFraction pelem)	/* aloca��o do no compacto */
{
	PtABPNode Node;

	if ((Node = (PtABPNode) malloc (sizeof (struct abpnode))) == NULL)
	{ Error = NO_MEM; return NULL; }

	Node->PtElem = pelem;	/* copiar a informa��o */
	Node->PtLeft = NULL;	/* apontar para a esquerda para NULL */
	Node->PtRight = NULL;	/* apontar para a direita para NULL */

	Error=OK;
	return Node;
}

/*******************************************************************************
  Fun��o que liberta a memoria do n� da arvore bin�ria de pesquisa.
*******************************************************************************/
static void ABPNodeDestroy (PtABPNode *pelem)	/* libertar o no da abp */
{
	FractionDestroy (&(*pelem)->PtElem);	/* libertar o elemento */
	free (*pelem);	/* libertar o no */
	*pelem = NULL;	/* colocar o ponteiro a nulo */
}

/*******************************************************************************
  Fun��o recursiva auxiliar que insere efectivamente um no na arvore.
*******************************************************************************/
static void InsertTree (PtABPNode *proot, PtFraction pelem)
{
	Error = OK;

	if (*proot == NULL)	/* inserir o elemento */
	{ if ((*proot = ABPNodeCreate (pelem)) == NULL) return; }
	else if (FractionCompareTo ((*proot)->PtElem, pelem) > 0)	/* subarvore esquerda */
				InsertTree (&(*proot)->PtLeft, pelem);
			else if (FractionCompareTo ((*proot)->PtElem, pelem) < 0)	/* subarvore direita */
						InsertTree (&(*proot)->PtRight, pelem);
					else { Error = REP_ELEM; return; }	/* o elemento j� existe */
}

/*******************************************************************************
  Fun��o recursiva que escreve os elementos da arvore em pre-ordem num ficheiro.
*******************************************************************************/
static void StoreFilePreOrder (PtABPNode proot, FILE *fich)
{
    if (proot != NULL)
    {
        fprintf (fich, "%d %d\n", FractionGetNumerator (proot->PtElem), FractionGetDenominator (proot->PtElem));
        StoreFilePreOrder (proot->PtLeft, fich);
        StoreFilePreOrder (proot->PtRight, fich);
    }
}

/*******************************************************************************
  Fun��o recursiva que armazena os elementos da arvore em ordem num array.
*******************************************************************************/
static void ListInOrder (PtABPNode proot, PtFraction plist[], unsigned int *pcount)
{
	if (proot != NULL)
	{
		ListInOrder (proot->PtLeft, plist, pcount);	/* arvore esquerda */
		plist[(*pcount)++] = proot->PtElem;	/* colocar elemento */
		ListInOrder (proot->PtRight, plist, pcount);	/* arvore direita */
	}
}

/*******************************************************************************
  Fun��o recursiva que insere os elementos de uma lista numa arvore de modo que
  ela fique balanceada.
*******************************************************************************/
static void Balance (PtABP ptree, PtFraction plist[], int pbegin, int pend)
{
	unsigned int Median; PtFraction Fraction;

	if (pbegin <= pend)
	{
		Median = (pbegin + pend) >> 1;	/* (pbegin+pend)/2 */
		Fraction = FractionCopy (plist[Median]);
		if (Fraction == NULL) { Error = NO_MEM; return ; }
		ABPInsertRec (ptree, Fraction);	/* inserir elemento central */
		if (Error == NO_MEM) return ;
		Balance (ptree, plist, pbegin, Median-1);	/* subarvore esquerda */
		Balance (ptree, plist, Median+1, pend);	/* subarvore direita */
	}
}

/*******************************************************************************
  Fun��o recursiva que escreve os elementos da arvore de forma hierarquica.
*******************************************************************************/
static void DisplayTree (PtABPNode proot, unsigned int plevel)
{
	unsigned int i;

	if (proot == NULL)
	{
		for (i = 0; i < plevel; i++) printf ("\t");
		printf ("*\n");
        return;
	}

	DisplayTree (proot->PtRight, plevel+1);

	for (i = 0; i < plevel; i++) printf ("\t");
	
	char *Str = FractionToString (proot->PtElem);
	printf ("%s\n", Str);	/* imprimir o elemento */
	free (Str);

	DisplayTree (proot->PtLeft, plevel+1);
}


/*******************************************************************************
  Fun��o recursiva que escreve os elementos da arvore por ordem crescente.
*******************************************************************************/
static void PrintTree (PtABPNode proot)
{
	if (proot != NULL)
	{
		PrintTree (proot->PtLeft);
		char *Str = FractionToString (proot->PtElem);
		printf ("%s\t", Str);	/* imprimir o elemento */
		free (Str);
		PrintTree (proot->PtRight);
	}
}

/*******************************************************************************
  Fun��o recursiva que calcula a altura de uma arvore.
*******************************************************************************/
unsigned int HeightTree (PtABPNode proot)
{
	unsigned int LeftHeight, RightHeight;

	if (proot == NULL) { Error = ABP_EMPTY; return 0; } /* no externo no n�vel 0 */
	else
	{
		LeftHeight = HeightTree (proot->PtLeft);	/* subarvore esquerda */
		RightHeight = HeightTree (proot->PtRight);	/* subarvore direita */

		Error = OK;
		if (LeftHeight > RightHeight) return LeftHeight + 1;
		else return RightHeight + 1;
	}
}
