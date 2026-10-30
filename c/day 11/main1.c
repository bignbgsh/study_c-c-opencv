#include <stdio.h>

typedef int  (*operation)(int, int);

int add(int a, int b) {
    return a + b;
}   

int substract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    if (b == 0) {
        printf("被除数不能为0\n");
        return 0; // Return 0 or handle the error as needed
    }
    return a / b;
}


int main(void){

    int choice;
    int a, b;
    operation op = NULL;

    printf("请输入两个整数：");
    scanf("%d %d", &a, &b);

    printf("请选择操作：\n");
    printf("1. 加法\n");
    printf("2. 减法\n");
    printf("3. 乘法\n");
    printf("4. 除法\n");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            op = add;
            break;
        case 2:
            op = substract;
            break;
        case 3:
            op = multiply;
            break;
        case 4:
            op = divide;
            break;
        default:
            printf("无效选择\n");
            return 1;
    }

    int result = op(a, b);
    printf("结果：%d\n", result);

    return 0;
}