#include <stdio.h>

int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

int main()
{
    int x, y;
    printf("请输入两个整数：") ;
    scanf("%d %d", &x, &y);
    printf("最大值为：%d\n", max(x, y));
}