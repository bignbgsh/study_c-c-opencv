#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct student{
   string name;
   string id;
   int score;
};



void inputstudents (vector<student> &students){

    int n;
    cout << "请输入学生的人数";
    cin >> n;
    for (int i = 0;i < n;i++){
        student s;
        cout<<"请输入学生的姓名，学号，成绩：";
        cin>>s.name>>s.id>>s.score;
        students.push_back(s);
    }}






void statistics(const vector<student>& students){
    if (students.empty()){
        cout<<"没有学生信息"<<endl;
    }


    double sum = 0;
    int max = students[0].score;
    int min = students[0].score;
    int pass = 0;
    int fail = 0;



    for(int i = 0;i < students.size();i++){
        sum += students[i].score;
        
        if (max <= students[i].score){
            max = students[i].score;
        }
        if(min >= students[i].score){
            min = students[i].score;
        }
        if (students[i].score >= 60){
            pass++;
        }
        if (students[i].score < 60){
            fail++;
        }

    }
    double average = static_cast<double>(sum)/students.size();
    cout << "======成绩统计======"<<endl;
    cout << "平均分"<<average<<endl;
    cout << "最高分"<<max<<endl;
    cout << "最低分"<<min<<endl;
    cout << "及格人数"<<pass<<endl;
    cout << "不及格人数"<<fail<<endl;
}




void sortStudents(vector<student>& students){
    sort(students.begin(),students.end(),[](const student &a,const student &b){
        if(a.score != b.score){
            return a.score > b.score;
        }
        return a.id > b.id;
    });
}

void printfStudents(vector<student>& students){
    for (int i =0;i < students.size();i++){
        cout << students[i].name << "  "
        <<students[i].id << "  "
        <<students[i].score <<endl;
    }
}

int main(){
    vector <student> students;
    inputstudents(students);

    statistics(students);

    cout<<"======排序后学生信息======"<<endl;
    sortStudents(students);
    printfStudents(students);
    return 0;
}




