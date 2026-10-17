#include <stdio.h>
int main(int argc, char *argv[]) {
    int score;
    printf("请输入成绩：");
    scanf("%d",&score);

    if(score > 100){
        printf("无效成绩\n");
    }

    else if(score >=90 && score <=100){
        printf("等级：优秀\n");
    }
    else if(score >=70){
        printf("等级：良好\n");
    }
    else if(score >=60){
        printf("等级：几个\n");
    }
    else if(score >=0){
        printf("等级：不及格\n");
    }
    else{
        printf("无效成绩\n");
    }
    return 0;

}