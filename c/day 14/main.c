#include <stdio.h>

int main() {

FILE *fp = fopen("score.txt", "w");

if(fp == NULL){
    printf("文件打开失败");
    return 1;
}


fprintf(fp,"张三 75\n");
fprintf(fp,"李四 56\n");
fprintf(fp,"王五 91\n");
fclose(fp);

fp = fopen("score.txt", "r");
if(fp == NULL){
    printf("文件打开失败");
    return 1;
}

char name[20];
int score;  

while(fscanf(fp, "%s %d", name, &score) != EOF){
    printf("%s %d\n", name, score);
}
fclose(fp);

return 0;
}