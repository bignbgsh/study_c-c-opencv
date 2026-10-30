#include "contact_manager.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void inputcontacts (vector<contact> &contacts){
    contact c;
    cout<<"请输入姓名:";
    getline(cin, c.name);

    cout<<"请输入电话:";
    getline(cin, c.telephone);

    cout<<"请输入qq或邮箱:";
    getline(cin, c.qq_or_email);

    contacts.push_back(c);
    cout<<"添加成功"<<endl;

    

   }








void printcontacts(vector<contact>& contacts){
    if(contacts.empty()){
        cout<<"通讯录中暂无联系人"<<endl;
        return;
    }


    for (int i =0;i < contacts.size();i++){
        cout << contacts[i].name << "|"
        <<contacts[i].telephone<< "|"
        <<contacts[i].qq_or_email <<endl;
    }
}

void findcontacts(const vector<contact>& contacts){
    string name;
    cout<<"请输入你要查找的联系人："<<endl;
    getline(cin, name);

    bool find = false;

    for (int i = 0;i<contacts.size();i++){
        if(contacts[i].name == name){
             cout << contacts[i].name << "|"
                 << contacts[i].telephone << "|"
                 << contacts[i].qq_or_email << endl;
                 find = true;

        }
    }
    if(!find){
        cout<<"通讯录中无该联系人"<<endl;

    }
}

void erasecontacts(vector<contact>& contacts){
    string name;
    cout <<"请输入要删除的联系人"<<endl;
    getline(cin, name);

    for(int i = 0;i<contacts.size();i++){
        if (contacts[i].name == name){
            contacts.erase(contacts.begin() + i);
            cout<<"删除成功"<<endl;
            return;
        }
    }
    cout <<"未找到联系人"<<endl;

}



bool savecontacts(const vector<contact>& contacts) {
    ofstream file("contacts.txt");

    if (!file) {
        cout << "文件打开失败，未保存。" << endl;
        return false;
    }

    for (int i = 0; i < contacts.size(); i++) {
        file << contacts[i].name << "|"
             << contacts[i].telephone << "|"
             << contacts[i].qq_or_email << '\n';
    }

    file.close();

    if (!file) {
        cout << "保存失败，请重试。" << endl;
        return false;
    }

    cout << "保存成功！" << endl;
    return true;
    
}


void loadcontacts(vector<contact>& contacts){
    ifstream file("contacts.txt");

    if(!file){
        cout<<"无法打开通讯录，无法加载联系人"<<endl;
        return;
    }
    string line;
    while(getline(file,line)){
        if(line.empty()){
            continue;
        }
        stringstream ss(line);
        contact c;

        if(!getline(ss,c.name,'|')||
            !getline(ss,c.telephone,'|')||
            !getline(ss,c.qq_or_email)){
            continue;
            }
        if (c.name.empty() ||
            c.telephone.empty() ||
            c.qq_or_email.empty() ||
            c.qq_or_email.find('|') != string::npos) {
            continue;
        }
        contacts.push_back(c);
        
    }
    if(file.bad()){
        cout<<"读取文件错误。"<<endl;
    }
}