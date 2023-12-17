#include <stdio.h>
#include <stdlib.h>

int main() {
    long long int gigabyte = 1024LL * 1024LL * 1024LL;

    void *ptr = malloc(gigabyte); 

    if (ptr == NULL) {
        printf("Impossible d'allouer de la mémoire.\n");
        return 1;
    }
    for (long long int i = 0; i < gigabyte; i++) {
        printf("Indice : %lld - Valeur de l'octet pour ptr : %d - Valeur de l'octet pour ptr2 : %d\n", i, ptr[i], ptr2[i]);
    }

    free(ptr);

    return 0;
}
