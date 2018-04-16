#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>

#include "generate.h"

void printHex(mpz_t value){
	gmp_printf("%#Zx",value);
}

void printDec(mpz_t value){
	gmp_printf("%#Zd",value);
}



void encrypt(mpz_t res, mpz_t m, mpz_t e, mpz_t n){
	mpz_powm(res,m,e,n);
}

void decrypt(mpz_t res, mpz_t c, mpz_t d, mpz_t n){
	mpz_powm(res,c,d,n);
}

//used the algo from referenced literature, didnt work, the one from SO seems to work
//https://stackoverflow.com/questions/23279208/rsa-calculate-d
void inverse(mpz_t res, mpz_t e, mpz_t phi){

	mpz_t local_e;
	mpz_init(local_e);
	mpz_set(local_e,e);

	mpz_t a,b,u;
	mpz_init(a);
	mpz_init(b);
	mpz_init(u);
	mpz_set_ui(a,0);
	mpz_set(b,phi);
	mpz_set_ui(u,1);

	mpz_t q;
	mpz_init(q);
		mpz_t q_times_u;
		mpz_init(q_times_u);
		mpz_t tmp_x;
		mpz_init(tmp_x);
		mpz_t tmp_a;
		mpz_init(tmp_a);

	while(mpz_cmp_ui(local_e,0)>0){
		mpz_fdiv_q(q,b,local_e);

		mpz_set(tmp_x,local_e);
		mpz_mod(local_e,b,local_e);

		mpz_set(tmp_a,a);
		mpz_set(a,u);
		mpz_set(b,tmp_x);

		mpz_mul(q_times_u,q,u);
		mpz_sub(u,tmp_a,q_times_u);
	}
	mpz_mod(res,a,phi);
}

//miller-rabin primality test
//https://cs.wikipedia.org/wiki/Miller%C5%AFv%E2%80%93Rabin%C5%AFv_test_prvo%C4%8D%C3%ADselnosti
bool isPrime(mpz_t value, int k){
	
	mpz_t value_local;
	mpz_init(value_local);
	mpz_set(value_local,value);

	mpz_t value_local_dec;
	mpz_init(value_local_dec);
	mpz_sub_ui(value_local_dec,value_local,1);

	mpz_t d;
	mpz_init(d);
	int s=0;
	while(true){
		mpz_fdiv_q_ui(value_local_dec,value_local_dec,2);
		s++;

		if(mpz_tstbit(value_local_dec,0)==1){
			//if the quotient after division is odd (==d), break
			mpz_set(d,value_local_dec);
			break;
		}
	}


	mpz_t a;
	mpz_init(a);
	mpz_t x;
	mpz_init(x);
	for(int i=0; i<k; i++){
		gmp_randstate_t state;
		gmp_randinit_default(state);
		gmp_randseed_ui(state,666);
		//todo: modify bounds (rand from 2 to n-2)
		mpz_urandomm(a,state,value_local);

		mpz_powm(x,a,d,value_local);

		if(mpz_cmp_ui(x,1)==0 || mpz_cmp(x,value_local_dec)){
			continue;
		}

		for(int r=1; r<s; r++){
			mpz_powm(x,x,2,value_local);
			if(mpz_cmp_ui(x,1)==0){
				return false;
			}
			if(mpz_cmp(x,value_local_dec)){
				break;
			}
		}
	}

	// printDec(value_local_dec);


	return true;
}

int cyclesBreak = 200;
void my_nextprime(mpz_t res, mpz_t value){
	
	mpz_t local_value;
	mpz_init(local_value);
	mpz_set(local_value,value);

	int lsb = mpz_tstbit(local_value,0);
	if(lsb==0){
		mpz_add_ui(local_value,local_value,1);
		mpz_set(res,local_value);
	}
	
	while(true){

		if(cyclesBreak--==0){
			printf("PRIME NOT FOUND\n");
			break;
		}
		if(isPrime(local_value,15)){
			printf("JOO\n");
			mpz_set(res,local_value);
			break;
		}
		else{
			mpz_add_ui(local_value,local_value,2);
		}
	}

}

void generate(char* bitSizeArg){

	int bitSize=0;
	sscanf(bitSizeArg,"%i",&bitSize);




	mpz_t p;
	mpz_init2(p,512);
	gmp_randstate_t state_p;
	gmp_randinit_default(state_p);
	gmp_randseed_ui(state_p,30);
	mpz_urandomb(p,state_p,512);

	printf("BEFORE\n");
	printDec(p);
	printf("\n");
	my_nextprime(p,p);
	printf("AFTER\n");
	printDec(p);
	printf("\n");


	mpz_t q;
	mpz_init2(q,512);
	gmp_randstate_t state_q;
	gmp_randinit_default(state_q);
	gmp_randseed_ui(state_q, 40);
	mpz_urandomb(q,state_q,512);

	mpz_nextprime(q,q);

	int msb_p = mpz_tstbit(p,511);
	int msb_q = mpz_tstbit(q,511);
	
	mpz_t n;
	mpz_init2(n,1024);
	mpz_mul(n,p,q);

	mpz_t dec_p,dec_q,phi;
	mpz_init2(dec_p,512);
	mpz_init2(dec_q,512);
	mpz_init2(phi,1024);

	mpz_sub_ui(dec_p,p,1);
	mpz_sub_ui(dec_q,q,1);
	mpz_mul(phi,dec_p,dec_q);

	mpz_t e;
	mpz_init(e);
	mpz_set_ui(e,65537);

	mpz_t d;
	mpz_init(d);
	inverse(d,e,phi);

	mpz_t message;
	mpz_init(message);
	mpz_set_ui(message,123);

	mpz_t ciphertext;
	mpz_init(ciphertext);
	encrypt(ciphertext,message,e,n);

	mpz_t decr;
	mpz_init(decr);
	decrypt(decr,ciphertext,d,n);

	// printDec(decr);
	// printf("\n");

	// mpz_clear(p);
	// mpz_clear(q);
}
