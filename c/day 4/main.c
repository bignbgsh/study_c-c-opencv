#include <stdio.h>

int main() {
    
    printf("Å¼Êý£º");
    for (int i=1;i <= 100; i++) {
        if (i % 2 == 0) {
            printf("%d ", i);
        }
    }
    printf("\n");

    printf("ÆæÊý£º");
    for (int i=1;i <= 100; i++) {
        if (i % 2 != 0) {
            printf("%d ", i);
        }
    }

    return 0;
}