#include <stdio.h>


typedef struct 
{
    char name[20];
    int id;
    float score;


    
}Student;

int main(){
    Student students[3];
    printf("请输入学生的信息：");
    for(int i = 0;i<3;i++){
        printf("学生%d号,姓名：",i+1);
        scanf("%s",&students[i].name);
        printf("id:");
        scanf("%d",&students[i].id);
        printf("成绩:");
        scanf("%2f",&students[i].score);
    
    }
    

    for(int i=0;i<3;i++){
        printf("学生号姓名：%s，id：%d，成绩：%2f",students[i].name,students[i].id,students[i].score);
        printf("\n");
    }








}


