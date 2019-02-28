#include <iostream>
#include <math.h>
#include <time.h>
#include <ctime>
#include <stdio.h>
#include <gmp.h>
using namespace std;

int main() {
   
   // Setup
   // input m value
   mpz_t m;
   mpz_init(m);
   printf("'m' value: ");
   gmp_scanf("%Zd", m);

   // choose multiplicative cyclip group G with generator g
   // find random prime number q and p
   gmp_randstate_t state;
   gmp_randinit_default(state);
   gmp_randseed_ui(state, time(NULL));

   mpz_t p, q, limit1, one, two;
   mpz_inits(q, limit1, one, two, NULL);
   mpz_set_str(one, "1", 10);
   mpz_set_str(two, "2", 10);

   mpz_urandomb(q, state, 1000);
   mpz_set_d(limit1, pow(2, 1000));
   mpz_add(q, q, limit1);
   mpz_nextprime(q, q);

   bool pPrime = false;
   while (!pPrime) {
      // p = 2q + 1
      mpz_init(p);
      mpz_mul(p, q, two);
      mpz_add(p, p, one);
   
      if (mpz_probab_prime_p(p, 25) > 0) pPrime = true;
      else {
         mpz_nextprime(q, q);
	 mpz_clear(p);
      }
   }

   //gmp_printf("q, p, p bits\n");
   //gmp_printf("%Zd\n%Zd\n%d\n\n", q, p, (int)mpz_sizeinbase(p, 2));


   // find generator of Z*p
   mpz_t g, pSub, exp1, exp2;
   mpz_inits(g, pSub, exp1, exp2, NULL);
   mpz_set_str(exp1, "1", 10);
   mpz_set_str(exp2, "1", 10);
   mpz_sub(pSub, p, one);
   while (mpz_cmp(exp1, one)==0 || mpz_cmp(exp2, one)==0) {
      // find g s.t. 1 < g < p-1
      mpz_urandomb(g, state, 1000);
      mpz_add(g, g, one);
      while (mpz_cmp(g, pSub) >= 0) {
         mpz_urandomb(g, state, 1000);
         mpz_add(g, g, one);
      }
      //mpz_div(exp1, pSub, two);
      //mpz_div(exp2, pSub, q);
      mpz_powm(exp1, g, q, p);
      mpz_powm(exp2, g, two, p);
   }
   //gmp_printf("g\n");
   //gmp_printf("%Zd\n%d\n\n", g, (int)mpz_sizeinbase(g, 2));
   //gmp_printf("%Zd\n%Zd\n\n", exp1, exp2);
   
   
   // Key Gen
   // private key x
   mpz_t x;
   mpz_init(x);
   mpz_set_str(x, "1234567890123456789012345678901234567890", 10);
   //gmp_printf("x\n%Zd\n\n", x);

   // find h
   mpz_t h, randMod, randMod1;
   mpz_inits(h, randMod, NULL);
   mpz_urandomb(randMod, state, 1000);
   mpz_mul(randMod1, randMod, x);
   //mpz_powm(h, g, x, randMod1); 
   //gmp_printf("h\n%Zd\n\n", h);
   mpz_powm(h, g, x, p);
   //gmp_printf("h\n%Zd\n\n", h);

   // Encryption
   mpz_t r, qSub, c1, c2, randMod2;
   mpz_inits(r, qSub, c1, c2, randMod2, NULL);

 for (int i=0; i<5; i++) {

   // Find a random r
   mpz_urandomb(r, state, 1000);
   mpz_add(r, r, one);
   mpz_mul(r, r, two);
   mpz_mod(r, r, pSub); 
   mpz_sub(qSub, q, one);
   while (mpz_cmp(r, qSub) >= 0) {
      mpz_urandomb(r, state, 1000);
      mpz_add(r, r, one);
      mpz_mul(r, r, two);
      mpz_mod(r, r, pSub);
   }
   //gmp_printf("r\n%Zd\n\n", r);
   
   // c1
   mpz_powm(c1, g, r, p);
   
   // c2
   mpz_powm(c2, h, r, p);
   mpz_mod(m, m, p);
   mpz_mul(c2, c2, m);
   mpz_mod(c2, c2, p);

   //gmp_printf("c1 c2 p\n");
   //gmp_printf("%Zd\n%Zd\n%Zd\n\n", c1, c2, p);

   // Try Decryption
   mpz_t t;
   mpz_init(t);
   mpz_powm(t, c1, x, p);
   mpz_invert(t, t, p);
   mpz_mul(t, t, c2);
   mpz_mod(t, t, p);
   
   //gmp_printf("%Zd\n%Zd\n%Zd\n%Zd\n", c1, c2, p, m);
   //gmp_printf("%d  %d\n", mpz_legendre(c1,p), mpz_legendre(c2,p));
   gmp_printf("%Zd,%Zd,%Zd\n\n", c1, c2, p);

 }

   //mpz_clear(g);
   //mpz_clear(q);
   //mpz_clear(h);
   //mpz_clear(r);

   return 0;
}







