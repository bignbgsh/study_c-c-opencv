#include <stdio.h>
int main(int argc, char *argv[]) {

    int a,b;
    char op;
    printf("请输入表达式：");
    scanf("%d %c %d",&a,&op,&b);
    switch(op){
        case '+':
            printf("结果是：%d\n",a+b);
            break;
        case '-':
            printf("结果是：%d\n",a-b);
            break;
        case '*':
            printf("结果是：%d\n",a*b);
            break;
        case '/':
            if(b != 0){
                printf("结果是：%.1f\n",(double)a/b);
                break;
            }
            else{
                printf("除数不能为0\n");
                break;
            }
            
        default:
            printf("运算符错误\n");
    }

   
    return 0;

}