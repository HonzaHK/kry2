#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "generate.h"

typedef enum { 
	EGenerate,
	EEncode,
	EDecode,
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
	// else if(argc==5 && strcmp(argv[1],"-e")==0){
	// 	sscanf(argv[2],"%i",&clargs->publicModulusSize);
	// 	return true;
	// }
	// else if(argc==5 && strcmp(argv[1],"-d")==0){
	// 	sscanf(argv[2],"%i",&clargs->publicModulusSize);
	// 	return true;
	// }
	// else if(argc==3 && strcmp(argv[1],"-b")==0){
	// 	sscanf(argv[2],"%i",&clargs->publicModulusSize);
	// 	return true;
	// }
	else{
		return false;
	}
}

int main(int argc, char* argv[]){

	clargs_t clargs;
	int isArgsOk = parseArgs(argc,argv,&clargs);
	if(!isArgsOk){
		printHelp();
		return 0;
	}

	if(clargs.mode==EGenerate){
		generate(clargs.publicModulusRequiredSize);
	}
}