#include <iostream>
#include <math.h>
#include <time.h>
#include <ctime>
#include <stdio.h>
#include <gmp.h>
using namespace std;

int main() {

   //input m value
   mpz_t m;
   mpz_init(m);

   printf("'m' value: ");
   gmp_scanf("%Zd", m);

   // generate p and q
   gmp_randstate_t state;
   gmp_randinit_default(state);
   gmp_randseed_ui(state, time(NULL));  

   mpz_t p, q, limit1;
   mpz_inits(p, q, limit1, NULL);
   mpz_urandomb(p, state, 275);
   mpz_urandomb(q, state, 275);
   
   mpz_set_d(limit1, pow(2, 275));
   mpz_add(p, p, limit1);
   mpz_add(q, q, limit1);
   
   mpz_nextprime(p, p);
   mpz_nextprime(q, q);

   //gmp_printf("p and q\n");
   //gmp_printf("%Zd\n%Zd\n\n", p, q);
   
   // calculate n
   mpz_t n;
   mpz_init(n);
   mpz_mul(n, p, q);
   //gmp_printf("n\n");
   //gmp_printf("%Zd  %d\n\n", n, (int)mpz_sizeinbase(n, 2));

   // key generation
   // compute Euler's totient function
   mpz_t totientFunc, pSub, qSub, one;
   mpz_inits(totientFunc, pSub, qSub, NULL);
   mpz_init_set_str(one, "1", 10);
   mpz_sub(pSub,p,one);
   mpz_sub(qSub,q,one);
   mpz_mul(totientFunc, pSub, qSub);
   //gmp_printf("totient func\n");
   //gmp_printf("%Zd  %d\n\n", totientFunc, (int)mpz_sizeinbase(totientFunc,2));

   // generate e
   mpz_t e, two, gcd;
   mpz_inits(e, gcd, NULL);
   mpz_init_set_str(two, "2", 10);
   mpz_urandomb(e, state, (int)mpz_sizeinbase(totientFunc,2));
   mpz_add(e, e, two);
   
   mpz_gcd(gcd, e, totientFunc);
   while (mpz_cmp(e, totientFunc)>=0 || mpz_cmp(gcd, one)!=0) {
      mpz_urandomb(e, state, (int)mpz_sizeinbase(totientFunc,2));
      mpz_add(e, e, two);
      mpz_gcd(gcd, e, totientFunc);
   }
   //gmp_printf("e\n");
   //gmp_printf("%Zd  %d\n\n", e, (int)mpz_sizeinbase(e,2));

   // generate d 
   mpz_t d;
   mpz_init(d);
   mpz_invert(d, e, totientFunc); 
   //gmp_printf("d\n");
   //gmp_printf("%Zd\n\n", d);

   // encrypt message
   mpz_t c;
   mpz_init(c);
   mpz_powm(c, m, e, n);
   //gmp_printf("c\n");
   //gmp_printf("%Zd\n", c); 
   
   // decrypt message
   mpz_t mNew;
   mpz_init(mNew);
   mpz_powm(mNew, c, d, n);
   //gmp_printf("decrypted message....\n");
   //gmp_printf("%Zd\n", mNew);

   gmp_printf("%Zd, %Zd, %Zd, %Zd\n", c, e, d, n);

   return 0;
}




