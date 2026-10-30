#include <stdio.h>
int main(int argc, char *argv[]) {

    double c, f;
    printf("what's the c:");
    scanf("%lf",&c);
    f = c * 1.8 + 32;
    printf("F:%.1f\n", f);
    return 0;

}