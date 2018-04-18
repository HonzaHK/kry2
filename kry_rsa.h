#include <gmp.h>

typedef struct {
	mpz_t p;
	mpz_t q;
	mpz_t n;
	mpz_t e;
	mpz_t d;

	mpz_t m;
	mpz_t c;
} rsa_props_t;

void rsa_props_t_init(rsa_props_t* rsa_props);
void rsa_props_t_clear(rsa_props_t* rsa_props);

void generate(rsa_props_t* rsa_props, char* bitSize);
void encrypt(mpz_t res, mpz_t m, mpz_t e, mpz_t n);
void decrypt(mpz_t res, mpz_t c, mpz_t d, mpz_t n);