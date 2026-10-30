#include <stdio.h>
#include <string.h>

int main(){
    char str[100];
    printf("请输入一个字符串：");
    fgets(str, sizeof(str),stdin);
    
    str[strcspn(str, "\n")] = '\0'; // 去掉换行符

    int len = strlen(str);

    printf("反转后");
    for(int i = len-1; i >= 0; i--){
        printf("%c", str[i]);
    }

    printf("\n");




    

}