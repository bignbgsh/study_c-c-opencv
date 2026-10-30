#include <stdio.h>
#include <string.h>

#define MAX_CONTACT 100

typedef struct{
    char name[50];
    char phone[11];
}Contact;

Contact contacts[MAX_CONTACT];
int count = 0;

void addContact() {
    if (count >= MAX_CONTACT) {
        printf("通讯录已满\n");
        return;
    }
    printf("姓名");
    scanf("%s", contacts[count].name);
    printf("电话号码: ");
    scanf("%s", contacts[count].phone);
    count++;
    printf("联系人添加成功!\n");
}

void showContact(){
    printf("联系人列表:\n");
    for(int i = 0; i < count; i++) {
        printf("姓名: %s, 电话号码: %s\n", contacts[i].name, contacts[i].phone);
    }
}

int main() 
{          

    int choice;
    while(1) {
        printf("1. 添加联系人\n");
        printf("2. 显示联系人\n");
        printf("3. 退出\n");
        printf("请选择操作: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                addContact();
                break;
            case 2:
                showContact();
                break;
            case 3:
                return 0;
            default:
                printf("无效的选择，请重新输入。\n");
        }
    }
    return 0;
}   