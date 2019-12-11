#include <stdio.h>
#include <assert.h>

#include "fraction.h"

int main (void)
{
	PtFraction f1 = FractionCreate (3, 4);
	PtFraction f2 = FractionCreate (1, 2);

	printf ("f1 = %s\n", FractionToString (f1));
	printf ("f2 = %s\n", FractionToString (f2));

	printf ("Teste do IsZero (f1 != 0 ? true)\n");
	assert (FractionIsZero (f1) == 0);
	printf ("Teste do IsProper (f2 < 1 ? true)\n");
	assert (FractionIsProper (f2) == 1);

	printf ("Teste do CompareTo (f1 > f2 ? resultado > 0)\n");
	assert (FractionCompareTo (f1, f2) > 0);
	printf ("Teste do CompareTo (f2 < f1 ? resultado < 0)\n");
	assert (FractionCompareTo (f2, f1) < 0);
	printf ("Teste do CompareTo (f1 == f1 ? resultado = 0)\n");
	assert (FractionCompareTo (f1, f1) == 0);

	printf ("Teste do Copy (f3 = f1) e do Equals (f3 == f1 ? true)\n");
	PtFraction f3 = FractionCopy (f1);
	assert (FractionEquals (f3, f1) == 1);
	//printf ("f3 = %s\n", FractionToString (f3));

	printf ("Teste do Symmetrical (f4 = -f1) e do Equals (f4 != f1 ? true)\n");
	PtFraction f4 = FractionSymmetrical (f3);
	assert (FractionGetNumerator (f3) == -FractionGetNumerator (f4) && FractionGetDenominator (f3) == FractionGetDenominator (f4));
	assert (FractionEquals (f4, f1) == 0);
	//printf ("f4 = %s\n", FractionToString (f4));

	printf ("Teste do Set (f3 = 0/3)\n");
	FractionSet (f3, 0, 3);
	//printf ("f3 = %s\n", FractionToString (f3));
	printf ("Teste do IsZero (f3 == 0 ? true)\n");
	assert (FractionIsZero (f3) == 1);

	FractionSet (f3, -2, 3);
	//printf ("f3 = %s\n", FractionToString (f3));
	printf ("Teste do IsProper (|f3| < 1 ? true) para ver se tambem funciona com numerador negativo\n");
	assert (FractionIsProper (f3) == 1);

	printf ("Teste do Set (f4 = 4/3)\n");
	FractionSet (f4, 4, 3);
	//printf ("f4 = %s\n", FractionToString (f4));
	printf ("Teste do IsProper (!(f4 > 1) ? true)\n");
	assert (FractionIsProper (f4) == 0);

	printf ("Teste do Destroy (f3 == NULL ? true)\n");
	FractionDestroy (&f3);
	assert (f3 == NULL);

	FractionDestroy (&f1);
	FractionDestroy (&f2);
	FractionDestroy (&f3);
	FractionDestroy (&f4);
	printf ("Passou em todos os testes\n");
	return 0;
}







