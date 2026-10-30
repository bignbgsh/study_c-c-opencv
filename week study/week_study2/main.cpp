#include <iostream>
#include "contact_manager.h"
#include <limits>


using namespace std;



int main() {
    vector<contact> contacts;
    loadcontacts(contacts);
    int choice;
    

    while (true) {
        cout << "\n======联系人管理系统======" << endl;
        cout << "1. 添加联系人" << endl;
        cout << "2. 显示所有联系人" << endl;
        cout << "3. 按姓名查找联系人" << endl;
        cout << "4. 删除联系人" << endl;
        cout << "5. 保存并退出" << endl;
        cout << "请选择：";
        cin >>choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(choice>5||choice<1){
            cout << "输入有误，请输入1到5。" << endl;
            break;
        }
        switch(choice){
            case 1:
                inputcontacts(contacts);
                break;
            case 2:
                printcontacts(contacts);
                break;
            case 3:
                findcontacts(contacts);
                break;
            case 4:
                erasecontacts(contacts);
                break;
            case 5:
                savecontacts(contacts);
                return 0;


        }
        



    }
    return 0;
}



