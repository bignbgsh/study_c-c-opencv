#include <stdio.h>

int main() {
    
    int nums[9][9];
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            nums[i][j] = (i + 1) * (j + 1);
        }
    }


    for (int i = 0; i < 9; i++) {
        for (int j = 0; j <= i; j++) {
            printf("%d*%d=%d\t", i + 1, j + 1, nums[i][j]);
        }
        printf("\n");
    }
    return 0;
}