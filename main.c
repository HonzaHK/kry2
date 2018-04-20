#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "kry_rsa.h"

typedef enum { 
	EGenerate,
	EEncrypt,
	EDecrypt,
	EAttack
} EMode;

typedef struct { //structure for storing command line arguments
	EMode mode; //user has to specify mode
	char* publicModulusRequiredSize; // B
	char* publicExponentValue; // E
	char* publicModulusValue; // N
	char* privateExponentValue; // D
	char* plaintextValue; // M
	char* ciphertextValue; // C
} clargs_t;

void printHelp(){
	printf("HELP\n");
}

bool parseArgs(int argc, char* argv[], clargs_t *clargs){

	if(argc==3 && strcmp(argv[1],"-g")==0){
		clargs->mode = EGenerate;
		clargs->publicModulusRequiredSize = argv[2];
		return true;
	}
	else if(argc==5 && strcmp(argv[1],"-e")==0){
		clargs->mode = EEncrypt;
		clargs->publicExponentValue = argv[2];
		clargs->publicModulusValue = argv[3];
		clargs->plaintextValue = argv[4];
		return true;
	}
	else if(argc==5 && strcmp(argv[1],"-d")==0){
		clargs->mode = EDecrypt;
		clargs->privateExponentValue = argv[2];
		clargs->publicModulusValue = argv[3];
		clargs->ciphertextValue = argv[4];
		return true;
	}
	else if(argc==3 && strcmp(argv[1],"-b")==0){
		clargs->mode = EAttack;
		clargs->publicModulusValue = argv[2];
		return true;
	}
	else{
		return false;
	}
}

int main(int argc, char* argv[]){

	if(argc==2 && strcmp(argv[1],"-h")==0){
		printHelp();
		return 0;
	}

	clargs_t clargs;
	int isArgsOk = parseArgs(argc,argv,&clargs);
	if(!isArgsOk){
		printf("Wrong arguments. Run ./kry -h to display help.\n");
		return 0;
	}

	//initialize struct with all params needed
	rsa_props_t rsa_props;
	rsa_props_t_init(&rsa_props);
	
	if(clargs.mode==EGenerate){
		generate(&rsa_props, clargs.publicModulusRequiredSize);
		gmp_printf(
			"%#Zx %#Zx %#Zx %#Zx %#Zx",
			rsa_props.p,
			rsa_props.q,
			rsa_props.n,
			rsa_props.e,
			rsa_props.d
		);
	}
	else if(clargs.mode==EEncrypt){
		mpz_set_str(rsa_props.m,clargs.plaintextValue,0);
		mpz_set_str(rsa_props.e,clargs.publicExponentValue,0);
		mpz_set_str(rsa_props.n,clargs.publicModulusValue,0);
		encrypt(rsa_props.c, rsa_props.m, rsa_props.e, rsa_props.n);
		gmp_printf("%#Zx",rsa_props.c);
	}
	else if(clargs.mode==EDecrypt){
		mpz_set_str(rsa_props.c,clargs.ciphertextValue,0);
		mpz_set_str(rsa_props.d,clargs.privateExponentValue,0);
		mpz_set_str(rsa_props.n,clargs.publicModulusValue,0);
		encrypt(rsa_props.m, rsa_props.c, rsa_props.d, rsa_props.n);
		gmp_printf("%#Zx",rsa_props.m);
	}
	else if(clargs.mode==EAttack){

	}
	
	//free memory
	rsa_props_t_clear(&rsa_props);
}