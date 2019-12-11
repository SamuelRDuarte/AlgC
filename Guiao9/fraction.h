/*******************************************************************************

 Ficheiro de interface do Tipo de Dados Abstracto FRACTION (fraction.h). A fra��o 
 � composta pelo numerador e pelo denominador. No caso de uma fra��o negativa, o
 sinal deve estar associado ao numerador, sendo que o denominador deve ser sempre
 positivo. A implementa��o providencia um construtor para criar e inicializar uma
 fra��o. � da responsabilidade da aplica��o, invocar o destrutor para libertar a
 mem�ria atribu�da ao objecto. O m�dulo providencia um controlo centralizado de 
 erro, disponibilizando uma fun��o para obter o �ltimo erro ocorrido, uma fun��o 
 para obter uma mensagem de erro elucidativa e uma fun��o para limpar o erro.

 Autor : Ant�nio Manuel Adrego da Rocha    Data : Mar�o de 2019

 Interface file of the abstract data type FRACTION (fraction.h). A fraction is
 composed of numerator and denominator. In the case of a negative fraction, the 
 negative sign must be associated with the numerator, and the denominator should
 always be positive. The implementation provides a constructor to initialize 
 a fraction. The application has the responsibility to invoke the destructor, in
 order to release the dynamic memory allocated the object. The data-type has a 
 central control error mechanism, providing functions for obtaining the last error
 occurred and the correspondent error message and a function to clean the error.

*******************************************************************************/

#ifndef _FRACTION
#define _FRACTION

/************ Defini��o do Tipo Ponteiro para um N�mero Fracion�rio ***********/

typedef struct fraction *PtFraction;

/************************ Defini��o de C�digos de Erro ************************/

#define	OK          0	/* opera��o realizada com sucesso - operation with success */
#define	NO_FRACTION 1	/* a(s) fra��o (fra��es) n�o existe(m) - fraction(s) do not exist */
#define	NO_MEM      2	/* mem�ria esgotada - out of memory */
#define	ZERO_DIV    3	/* divis�o por zero - division by zero */
#define NULL_DEN    4	/* denominador nulo - null denominator */

/************************* Prot�tipos dos Subprogramas ************************/

void FractionClearError (void);
/*******************************************************************************
 Inicializa��o do erro. Error initialization.
*******************************************************************************/

int FractionErrorCode (void);
/*******************************************************************************
 Devolve o c�digo do �ltimo erro ocorrido. Returns the error code.
*******************************************************************************/

char *FractionErrorMessage (void);
/*******************************************************************************
 Devolve uma mensagem esclarecedora da causa do �ltimo erro ocorrido.
 Returns an error message.
*******************************************************************************/

PtFraction FractionCreate (int pnum, int pden);
/*******************************************************************************
 Cria a fra��o pnum/pden. O denominador n�o pode ser nulo e o sinal negativo deve
 ser sempre associado ao numerador. Devolve a refer�ncia da nova fra��o ou NULL,
 caso n�o consiga criar a fra��o, por falta de mem�ria. Valores de erro: OK, 
 NO_FRACTION ou NO_MEM.

 Creates the fraction pnum/pden. The denominator cannot be zero and the negative 
 sign must always be associated to the numerator. Returns the reference of the new
 fraction or NULL for lack of memory. Error codes: OK, NO_FRACTION or NO_MEM.
*******************************************************************************/

int  FractionGetNumerator (PtFraction pfrac);
/*******************************************************************************
 Devolve o numerador da fra��o. Valores de erro: OK ou NO_FRACTION.

 Returns the numerator of the fraction. Error codes: OK or NO_FRACTION.
*******************************************************************************/

int  FractionGetDenominator (PtFraction pfrac);
/*******************************************************************************
 Devolve o denominador da fra��o. Valores de erro: OK ou NO_FRACTION.

 Returns the numerator of the fraction. Error codes: OK or NO_FRACTION.
*******************************************************************************/

void FractionDestroy (PtFraction *pfrac);
/*******************************************************************************
 Destrui��o de uma fra��o. Valores de erro: OK ou NO_FRACTION.

 Destroys the fraction and releases the memory. Error codes: OK or NO_FRACTION.
*******************************************************************************/

PtFraction FractionCopy (PtFraction pfrac);
/*******************************************************************************
 Copia a fra��o. Devolve a refer�ncia da nova fra��o ou NULL, caso n�o consiga
 fazer a c�pia por falta de mem�ria. Valores de erro: OK, NO_FRACTION ou NO_MEM.

 Copies the fraction. Returns the new fraction or NULL for lack of memory.
 Error codes: OK, NO_FRACTION or NO_MEM.
*******************************************************************************/

PtFraction FractionSymmetrical (PtFraction pfrac);
/*******************************************************************************
 Cria a fra��o sim�trica. Devolve a refer�ncia da nova fra��o ou NULL, 
 caso n�o consiga fazer a c�pia. Valores de erro: OK, NO_FRACTION ou NO_MEM.

 Creates the symemtrical fraction. Returns the new fraction or NULL for lack of
 memory. Error codes: OK, NO_FRACTION or NO_MEM.
*******************************************************************************/

PtFraction FractionAddition (PtFraction pfrac1, PtFraction pfrac2);
/*******************************************************************************
 Adi��o de duas fra��es. A fra��o resultante deve ser reduzida ao menor denominador 
 poss�vel. Valores de erro: OK, NO_FRACTION ou NO_MEM.

 Addition of two fractions. The resulting fraction must be reduced to the smallest
 denominator. Error codes: OK, NO_FRACTION or NO_MEM.
*******************************************************************************/

PtFraction FractionSubtraction (PtFraction pfrac1, PtFraction pfrac2);
/*******************************************************************************
 Subtra��o de duas fra��es. A fra��o resultante deve ser reduzida ao menor 
 denominador poss�vel. Valores de erro: OK, NO_FRACTION ou NO_MEM.

 Subtraction of two fractions. The resulting fraction must be reduced to the 
 smallest denominator. Error codes: OK, NO_FRACTION or NO_MEM.
*******************************************************************************/

PtFraction FractionMultiplication (PtFraction pfrac1, PtFraction pfrac2);
/*******************************************************************************
 Multiplica��o de duas fra��es. A fra��o resultante deve ser reduzida ao menor 
 denominador poss�vel. Valores de erro: OK, NO_FRACTION ou NO_MEM.

 Multiplication of two fractions. The resulting fraction must be reduced to the 
 smallest denominator. Error codes: OK, NO_FRACTION or NO_MEM.
*******************************************************************************/

PtFraction FractionDivision (PtFraction pfrac1, PtFraction pfrac2);
/*******************************************************************************
 Divis�o de duas fra��es. A fra��o resultante deve ser reduzida ao menor 
 denominador poss�vel.Valores de erro: OK, NO_FRACTION, ZERO_DIV ou NO_MEM.

 Division of two fractions. The resulting fraction must be reduced to the 
 smallest denominator. Error codes: OK, NO_FRACTION, ZERO_DIV or NO_MEM.
*******************************************************************************/

int FractionIsNull (PtFraction pfrac);
/*******************************************************************************
 Verifica se a fra��o � nula (ou seja se o numerador � zero). Devolve 1 em caso
 afirmativo e 0 em caso contr�rio. Valores de erro: OK ou NO_FRACTION.

 Verifies if fraction pfrac is zero (that is if the numerator is zero). Returns 1
 in affirmative case and 0 otherwise. Error codes: OK or NO_FRACTION.
*******************************************************************************/

int FractionEquals (PtFraction pfrac1, PtFraction pfrac2);
/*******************************************************************************
 Verifica se duas fra��es s�o iguais. Devolve 1 em caso afirmativo e 0 em caso
 contr�rio. Valores de erro: OK ou NO_FRACTION.

 Verifies if two fractions are equal. Returns 1 in affirmative case and 0 otherwise.
 Error codes: OK or NO_FRACTION.
*******************************************************************************/

int FractionCompareTo (PtFraction pfrac1, PtFraction pfrac2);
/*******************************************************************************
 Compara duas fra��es. Devolve um valor: positivo caso pfrac1 seja maior do que
 pfrac2; 0 caso as fra��es forem iguais; e negativo caso pfrac1 seja menor do que
 pfrac2. Valores de erro: OK ou NO_FRACTION.

 Compares two fractions are equal. Returns a value: positive if pfrac1 is greater
 than pfrac2; 0 if the fractiosn are equal; and negative if pfrac1 is smaller than
 pfrac2. Error codes: OK or NO_FRACTION.
*******************************************************************************/

int FractionIsProper (PtFraction pfrac);
/*******************************************************************************
 Verifica se a fra��o � uma fra��o pr�pria (ou seja se o seu valor absoluto � 
 menor do que 1). Devolve 1 em caso afirmativo e 0 em caso contr�rio. Valores de
 erro: OK ou NO_FRACTION.

 Verifies if fraction pfrac is a proper fraction (that is if its absolute value is
 smaller than one). Returns 1 in affirmative case and 0 otherwise.
 Error codes: OK or NO_FRACTION.
*******************************************************************************/

void FractionSet (PtFraction pfrac, int pnum, int pden);
/*******************************************************************************
 Modifica o numerador e o denominador da fra��o para, respectivamente pnum e pden.
 Valores de erro: OK ou NO_FRACTION.

 Modifies the numerator and denominator of the fraction to respectively pnum and pden.
 Error codes: OK or NO_FRACTION.
*******************************************************************************/

char * FractionToString (PtFraction pfrac);
/*******************************************************************************
 Devolve uma sequ�ncia de caracteres que representa a fra��o no formato Num/Den
 ou uma sequ�ncia de caracteres nula, caso n�o exista mem�ria.
 Valores de erro: OK, NO_FRACTION ou NO_MEM.

 Returns a string representing the fraction in the format Num/Den or a null string
 if there is no memory. Error values: OK, NO_FRACTION or NO_MEM.
*******************************************************************************/

#endif
