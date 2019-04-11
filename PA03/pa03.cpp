#include <iostream>
#include <math.h>
#include <time.h>
#include <ctime>
#include <stdio.h>
#include <gmp.h>
using namespace std;

void encryptPAI(mpz_t, gmp_randstate_t, mpz_t, mpz_t, mpz_t);
void decryptPAI(mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);
void encryptSHE(mpz_t, mpz_t, mpz_t, gmp_randstate_t, mpz_t tot);
void decryptSHE(mpz_t, mpz_t, mpz_t);

int main() {

   // input message, ciphertext, p, and q
   mpz_t mPai, c2Pai, p, q;
   mpz_inits(mPai, c2Pai, p, q, NULL);
   printf("input: ");
   gmp_scanf("%Zd,%Zd,%Zd,%Zd", mPai, c2Pai, p, q);

   // Paillier Cryptosystem 
   // compute n, g, tot (totient func), u
   // public key (n,g) private key (tot, u)
   mpz_t n, g, tot, u;
   mpz_inits(n, g, tot, u, NULL);

   mpz_mul(n, p, q);

   mpz_t one;
   mpz_init_set_str(one, "1", 10);
   mpz_add(g, n, one);

   mpz_t pSub, qSub;
   mpz_inits(pSub, qSub, NULL);
   mpz_sub(pSub, p, one);
   mpz_sub(qSub, q, one);
   mpz_mul(tot, pSub, qSub);

   mpz_invert(u, tot, n);

   // random state
   gmp_randstate_t state;
   gmp_randinit_default(state);
   gmp_randseed_ui(state, time(NULL));

 for (int i=0; i<5; i++) {

   // encrypt mPai
   mpz_t cPai;
   mpz_init(cPai);
   //mpz_set_ui(cPai, encryptPAI(state, n, g, mPai));
   encryptPAI(cPai, state, n, g, mPai);
   //gmp_printf("cPai: %Zd\n\n", cPai);

   // decrypt c2Pai
   mpz_t m2Pai;
   mpz_init(m2Pai);
   //mpz_set_ui(m2Pai, decryptPAI(c2Pai, tot, n, one, u));
   decryptPAI(m2Pai, c2Pai, tot, n, one, u);
   //gmp_printf("m2Pai: %Zd\n\n", m2Pai);
 
   // Somewhat homomorphic symmetric-key encryption
   mpz_t cSHE, sk, cSHEfinal;
   mpz_inits(cSHE, sk, cSHEfinal, NULL);
   encryptSHE(cSHE, sk, n, state, tot);
   //gmp_printf("cSHE: %Zd\n\n", cSHE);
   mpz_set(cSHEfinal, cSHE);

   // decrypt cSHE
   /*mpz_t m2SHE;
   mpz_init(m2SHE);
   decryptSHE(m2SHE, cSHE, sk);*/
   //gmp_printf("m2SHE: %Zd\n\n", m2SHE);

   gmp_printf("%Zd,%Zd,%Zd,%Zd,%Zd\n", cPai, m2Pai, cSHEfinal, p, q);   
   
 }

   return 0;
}

void encryptPAI(mpz_t cPai, gmp_randstate_t state, mpz_t n, mpz_t g, mpz_t mPai) {
   // select random r
   mpz_t r, zero;
   mpz_init(r);
   mpz_init_set_str(zero, "0", 10);
   
   mpz_urandomm(r, state, n);
   while (mpz_cmp(r, zero)==0)
      mpz_urandomm(r, state, n);

   // compute cPAI
   mpz_t gmod, rmod, nSq;
   mpz_inits(gmod, rmod, nSq, NULL);

   mpz_mul(nSq, n, n);
   mpz_powm(gmod, g, mPai, nSq);
   mpz_powm(rmod, r, n, nSq);
   mpz_mul(cPai, gmod, rmod);
   mpz_mod(cPai, cPai, nSq);

}

void decryptPAI(mpz_t m2Pai, mpz_t c2Pai, mpz_t tot, mpz_t n, mpz_t one, mpz_t u) {
   mpz_t cmod, onemod, nSq;
   mpz_inits(cmod, onemod, nSq, NULL);

   mpz_mul(nSq, n, n);
   mpz_powm(cmod, c2Pai, tot, nSq);
   mpz_mod(onemod, one, nSq);
   mpz_sub(cmod, cmod, onemod);
   mpz_mod(m2Pai, cmod, nSq);
   mpz_cdiv_q(m2Pai, m2Pai, n);
   mpz_mul(m2Pai, m2Pai, u);
   mpz_mod(m2Pai, m2Pai, n);

}

void encryptSHE(mpz_t cSHE, mpz_t sk, mpz_t n, gmp_randstate_t state, mpz_t tot) {
   // m=1
   mpz_t mSHE;
   mpz_init_set_str(mSHE, "1", 10);

   // set p (the secret key) equal to n
   mpz_set(sk, n);

   // generate random q and r
   mpz_t qSHE, rSHE, two, one;
   mpz_inits(qSHE, rSHE, NULL);
   mpz_init_set_str(two, "2", 10);
   mpz_init_set_str(one, "1", 10);
   int bits1 = (int)mpz_sizeinbase(sk,2)/2;
   mpz_urandomb(qSHE, state, bits1);

/*
   mpz_t div;
   mpz_init(div);
   mpz_root(div, sk, 256);
   mpz_sub(div, div, mSHE);
   mpz_cdiv_q(div, div, two);
   gmp_printf("Anna: %d %Zd\n\n", (int)mpz_sizeinbase(div, 2), div);
   int bits2 = (int)mpz_sizeinbase(div, 2);
*/ /*
   mpz_pow_ui(div, two, 16);
   mpz_cdiv_q(div, sk, div);
   mpz_root(div, div, 16);
   gmp_printf("Anna: %d %Zd\n\n", (int)mpz_sizeinbase(div, 2), div); 
   int bits2 = (int)mpz_sizeinbase(div, 2);
*/
   //mpz_urandomb(rSHE, state, bits2);

   //mpz_init_set_str(rSHE, "1", 10);

   mpz_urandomm(rSHE, state, two);
   mpz_add(rSHE, rSHE, one);
   
   // calculate ciphertext cSHE
   mpz_mul(cSHE, sk, qSHE);
   mpz_mul(rSHE, rSHE, two);
   mpz_add(cSHE, cSHE, rSHE);
   mpz_add(cSHE, cSHE, mSHE);

}

void decryptSHE(mpz_t m2SHE, mpz_t cSHE2, mpz_t sk) {
   mpz_t two;
   mpz_init_set_str(two, "2", 10);

   for (int i=0; i<8; i++)
      mpz_mul(cSHE2, cSHE2, cSHE2);

   mpz_mod(m2SHE, cSHE2, sk);
   mpz_mod(m2SHE, m2SHE, two);
}




