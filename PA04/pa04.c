#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include <pbc.h>

int main(int argc, char **argv) {

// SETUP
   // initialize pairing function
   // a.param file should be first command line argument
   pairing_t e;
   char param[BUFSIZ];
   FILE *fp = stdin;
   if (argc > 1) {
      fp = fopen(argv[1], "r");
      if (!fp) pbc_die("error opening file");
   }
   size_t count = fread(param, 1, BUFSIZ, fp);
   if (!count) pbc_die("input error");
   fclose(fp);
   if (pairing_init_set_buf(e, param, count)) pbc_die("pairing init failed");

   element_t g;
   element_t h;
   element_t h2;
   element_t Hm;
   element_t Hm2;
   element_t pk, sk;
   element_t sig;
   element_t sig2, pk2;
   element_t tmp1, tmp2, tmp3;

   element_init_G1(g, e); 
   element_init_G1(h, e);
   element_init_G1(h2, e);
   element_init_G1(pk, e);
   element_init_G1(sig, e);
   element_init_G1(sig2, e);
   element_init_G1(pk2, e);
   element_init_GT(tmp1, e);
   element_init_GT(tmp2, e);
   element_init_GT(tmp3, e);
   element_init_Zr(sk, e);
   element_init_Zr(Hm, e);
   element_init_Zr(Hm2, e);

   // get input values: g, m, sk, m', sig', pk'
   char gStr[BUFSIZ];
   char skStr[BUFSIZ];
   char msg[BUFSIZ];
   char sig2Str[BUFSIZ];
   char pk2Str[BUFSIZ];
   char msg2[BUFSIZ];

   printf("input:\n");
   fgets(gStr, BUFSIZ, stdin);
   gStr[strlen(gStr)-1] = '\0';
   scanf("%[^,],%s", msg, skStr);
   scanf("\n%[^,],%[^]]],%[^\n]", msg2, sig2Str, pk2Str);
   sig2Str[strlen(sig2Str)] = ']';
   sig2Str[strlen(sig2Str)] = '\0';
   //printf("A%sA A%sA A%sA\n", gStr, skStr, msg);
  
   element_set_str(g, gStr, 10);
   element_set_str(sk, skStr, 10);
   //element_printf("g: %B sk: %B\n", g, sk);
   
// KEYGEN
   // sk = x
   // public key = g^sk
   element_pow_zn(pk, g, sk);
   //element_printf("pk: %B\n", pk);

// SIGN
   element_from_hash(Hm, msg, strlen(msg));
   //element_printf("H(m): %B\n", Hm);
   element_pow_zn(h, g, Hm);
   //element_printf("%B\n", h);
   element_pow_zn(sig, h, sk);
   //element_printf("sig: %B\n", sig);
   element_printf("%B,%B\n", sig, g);

// VERIFY
   // new m, sig, and pk values
   element_set_str(sig2, sig2Str, 10);
   element_set_str(pk2, pk2Str, 10);
   //element_printf("%s %B %B\n", msg2, sig2, pk2);

   // compute new h value
   element_from_hash(Hm2, msg2, strlen(msg2));
   element_pow_zn(h2, g, Hm2);
   //element_printf("%B\n", h2);

   int b=0;
   element_pairing(tmp1, sig2, g);
   element_pairing(tmp2, h2, pk2);
   element_invert(tmp3, tmp1);
   //element_printf("%B %B %B\n", tmp1, tmp2, tmp3);
   if (!element_cmp(tmp1, tmp2) || !element_cmp(tmp3, tmp2)) {
      b=1;
   } else {
      b=0;
   }
   printf("%d\n", b);

   return 0;
} 



