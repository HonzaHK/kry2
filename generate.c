#include <stdio.h>
#include <gmp.h>

#include "generate.h"

void printHex(mpz_t value){
	gmp_printf("%#Zx",value);
}

void generate(char* bitSizeArg){

	int bitSize=0;
	sscanf(bitSizeArg,"%i",&bitSize);


	mpz_t p;
	mpz_init2(p,20);
	mpz_set_ui(p,17);


	printHex(p);
	printf("\n");
	mpz_clear(p);
}