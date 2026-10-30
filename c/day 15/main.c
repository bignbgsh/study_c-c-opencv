#include <stdio.h>
#include <stdlib.h>

int main(){
    int n;
    printf("请输入数字的个数：");
    if(scanf("%d", &n) != 1 || n <= 0) {
        printf("输入无效，请输入一个正整数。\n");
        return 1;
    }

    int *arr = malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("内存分配失败。\n");
        return 1;
    }

    printf("请输入%d个数字：",n);


    int sum = 0;
    for (int i = 0;i<n;i++){
        if(scanf("%d",&arr[i]) != 1){
            printf("输入错误");
            free(arr);
            return 1;
        }
        sum += arr[i];
    }


    double average = (double)sum / n;
    printf("总和为：%d",sum);
    printf("平均数为：%lf",average);

    free(arr);
    arr ==  NULL;
    return 0;
}