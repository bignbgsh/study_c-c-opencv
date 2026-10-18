#include <stdio.h>

int main(){
    int nuns[5];
    int sum = 0;
    
    printf("请填入5个整数:\n");
    for(int i=0; i<5; i++){
        scanf("%d", &nuns[i]);
        sum += nuns[i];
    }
    printf("整数和为：%d\n", sum);
    printf("整数平均值为：%.2f\n", sum / 5.0);
    return 0;
}