#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <time.h>

#include "kry_rsa.h"

void rsa_props_t_init(rsa_props_t* rsa_props){
	mpz_init(rsa_props->p);
	mpz_init(rsa_props->q);
	mpz_init(rsa_props->n);
	mpz_init(rsa_props->e);
	mpz_init(rsa_props->d);

	mpz_init(rsa_props->m);
	mpz_init(rsa_props->c);
}

void rsa_props_t_clear(rsa_props_t* rsa_props){
	mpz_clear(rsa_props->p);
	mpz_clear(rsa_props->q);
	mpz_clear(rsa_props->n);
	mpz_clear(rsa_props->e);
	mpz_clear(rsa_props->d);

	mpz_clear(rsa_props->m);
	mpz_clear(rsa_props->c);
}

//encrypts plaintext m with public key (n,e)
void encrypt(mpz_t res, mpz_t m, mpz_t e, mpz_t n){
	mpz_powm(res,m,e,n);
}

//decrypts ciphertext c with private key (n,d)
void decrypt(mpz_t res, mpz_t c, mpz_t d, mpz_t n){
	mpz_powm(res,c,d,n);
}

//used the algo from referenced literature, didnt work, the one from SO seems to work
//https://stackoverflow.com/questions/23279208/rsa-calculate-d
void multiplicative_inverse(mpz_t res, mpz_t e, mpz_t phi){

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
//value: tested number; k: test precision (the bigger, the better and slower)
//https://cs.wikipedia.org/wiki/Miller%C5%AFv%E2%80%93Rabin%C5%AFv_test_prvo%C4%8D%C3%ADselnosti
bool my_isprime(mpz_t value, int k){
	
	//create local copy, so the changes wont propagate out of the function
	mpz_t value_local;
	mpz_init(value_local);
	mpz_set(value_local,value);

	//we operate with value decremented by 1 (n-1)
	mpz_t value_local_dec;
	mpz_init(value_local_dec);
	mpz_sub_ui(value_local_dec,value_local,1);

	//odd remained after repeating division by 2
	mpz_t d;
	mpz_init(d);

	//value_local_dec_remaining contains value after repeated division by 2
	mpz_t value_local_dec_reamining;
	mpz_init(value_local_dec_reamining);
	mpz_set(value_local_dec_reamining,value_local_dec);

	//count, how many times we divided by 2
	int s=0;
	while(true){

		//divide by 2
		mpz_fdiv_q_ui(value_local_dec_reamining,value_local_dec_reamining,2);
		
		//increase count
		s++;

		//if the remaining value is odd, we are done; assign this value into d
		if(mpz_odd_p(value_local_dec_reamining)){
			mpz_set(d,value_local_dec_reamining);
			break;
		}
	}


	//initialize variables needed by the algo
	mpz_t a;
	mpz_init(a);
	mpz_t x;
	mpz_init(x);

	for(int i=0; i<k; i++){

		//generate random number a from interval [2;n-2]
		gmp_randstate_t state;
		gmp_randinit_default(state);
		gmp_randseed_ui(state,0);
		//mpz_urandomm returns number from 0 to NUM-1, so NUM=n-1 is correct upper bound
		mpz_urandomm(a,state,value_local_dec); 
		if(mpz_cmp_ui(a,2)<0){
			//mpz_urandomm lower bound cannot be set, so modify the result conditionally
			mpz_add_ui(a,a,2);
		}

		//x = a^d mod n
		mpz_powm(x,a,d,value_local);

		//if x==1 or x==n-1, goto LOOP (next loop increases result correctness probability)
		if(mpz_cmp_ui(x,1)==0 || mpz_cmp(x,value_local_dec)==0){
			continue;
		}

		for(int r=1; r<s; r++){
			
			//x = x^2 mod n
			mpz_powm_ui(x,x,2,value_local);
			
			//if x==1, the number is not a prime
			if(mpz_cmp_ui(x,1)==0){
				return false;
			}
			
			//if x==n-1, goto LOOP (next loop increases result correctness probabilty)
			if(mpz_cmp(x,value_local_dec)==0){
				break;
			}
		}

		return false;
	}

	return true;
}

void my_nextprime(mpz_t res, mpz_t value){
	
	//create local copy, so any changes wont propagate out
	mpz_t value_local;
	mpz_init(value_local);
	mpz_set(value_local,value);

	if(mpz_even_p(value_local)!=0){
		//if number is even, do +1 to get odd (only odds may be primes)
		mpz_add_ui(value_local,value_local,1);
	}
	else{
		//if number is odd, it might be a prime, and we want the next one, so increment at least by 2
		mpz_add_ui(value_local,value_local,2);
	}
	
	while(true){
		//as stated here: https://gmplib.org/manual/Number-Theoretic-Functions.html#Number-Theoretic-Functions
		//reasonable values for k (probability of false positive) are between 15 and 50
		if(my_isprime(value_local,15)){
			//if number is prime, assign it to result and stop
			mpz_set(res,value_local);
			break;
		}
		else{
			//if number is not prime, do +2 and try again
			mpz_add_ui(value_local,value_local,2);
			continue;
		}
	}

}

void generate_prime_n_bits(mpz_t res, gmp_randstate_t randstate, mp_bitcnt_t n_bits){

	while(true){

		//assign it random value of n bits
		mpz_urandomb(res,randstate,n_bits);

		//msb has to be 1, if not, generate new random value
		if(mpz_tstbit(res,n_bits-1)!=1){
			continue;
		}

		//get first prime number larger than value generated
		my_nextprime(res,res);

		//if the prime number size has exceeded the predefined size, generate again 
		if(mpz_sizeinbase(res,2)>n_bits){
			continue;
		}

		break;
	}
}

void generate(rsa_props_t* rsa_props, char* modulusBitSizeRequiredArg){

	size_t modulusBitSizeRequired=0;
	sscanf(modulusBitSizeRequiredArg,"%zu",&modulusBitSizeRequired);

	size_t quotientBitSize = modulusBitSizeRequired/2;
	//if required modulus bit size is odd, the quotient bit size has to be ceiled
	if(modulusBitSizeRequired%2==1){
		quotientBitSize+=1;
	}


	//init randstate, use time as a seed
	gmp_randstate_t randstate;
	gmp_randinit_default(randstate);
	gmp_randseed_ui(randstate,0);


	//init p,q
	mpz_t p,q,n;
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);

	while(true){

		generate_prime_n_bits(p,randstate,quotientBitSize);
		generate_prime_n_bits(q,randstate,quotientBitSize);
		mpz_mul(n,p,q);

		if(mpz_sizeinbase(n,2)==modulusBitSizeRequired){
			break;
		}
	}


	mpz_t dec_p,dec_q,phi;
	mpz_init(dec_p);
	mpz_init(dec_q);
	mpz_init(phi);

	mpz_sub_ui(dec_p,p,1);
	mpz_sub_ui(dec_q,q,1);
	mpz_mul(phi,dec_p,dec_q);

	mpz_t e;
	mpz_init(e);
	mpz_set_ui(e,65537);

	mpz_t remainder;
	mpz_init(remainder);
	while(true){

		//perform remainder = phi % e
		mpz_mod(remainder,phi,e);

		//if there is a reminder, value of e is okay
		if(mpz_sgn(remainder)>0){
			break;
		}

		//if phi is divisible by e without a reminder, assign into e next prime and try again
		my_nextprime(e,e);
	}

	mpz_t d;
	mpz_init(d);
	multiplicative_inverse(d,e,phi);

	mpz_set(rsa_props->p,p);
	mpz_set(rsa_props->q,q);
	mpz_set(rsa_props->n,n);
	mpz_set(rsa_props->e,e);
	mpz_set(rsa_props->d,d);

	// mpz_t message;
	// mpz_init(message);
	// mpz_set_ui(message,0x123456789);

	// mpz_t ciphertext;
	// mpz_init(ciphertext);
	// encrypt(ciphertext,message,e,n);

	// mpz_t decr;
	// mpz_init(decr);
	// decrypt(decr,ciphertext,d,n);

	// gmp_printf("decrypted: %#Zx\n",decr);
}
