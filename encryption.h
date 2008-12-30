// RC4 encryptie
// URL: http://en.wikipedia.org/wiki/RC4
// afhankelijkheden: screenlib charsign

#ifndef _SCREENLIB_
#include "screenlib.h"
#endif
#ifndef _ENCRYPTION_
#define _ENCRYPTION_

#endif
#ifndef _CHARSIGN_
#include "charsign.h"
#endif


unsigned char S[256];
unsigned int ei, ej;
 
/* KSA */
void rc4_init(unsigned char *key, unsigned int key_length) {
    for (ei = 0; ei < 256; ei++)
        S[ei] = ei;
 
    for (ei = ej = 0; ei < 256; ei++) {
        ej = (ej + key[ei % key_length] + S[ei]) & 255;
 
        unsigned char temp = S[ei];
        S[ei] = S[ej];
        S[ej] = temp;
    }
 
    ei = ej = 0;
}
 
/* PRGA */
unsigned char rc4_output() {
    ei = (ei + 1) & 255;
    ej = (ej + S[ei]) & 255;
 
    unsigned char temp = S[ei];
    S[ei] = S[ej];
    S[ej] = temp;
 
    return S[(S[ei] + S[ej]) & 255];
}
