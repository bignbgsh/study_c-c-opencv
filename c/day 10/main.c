#include <stdio.h>


void swap(int *a , int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(){
    int x,y;
    printf("请输入两个整数：");
    scanf("%d %d",&x,&y);
    printf("交换前：%d,%d\n",x,y);


    swap(&x,&y);
    printf("交换后：%d,%d\n",x,y);

    return 0;
}