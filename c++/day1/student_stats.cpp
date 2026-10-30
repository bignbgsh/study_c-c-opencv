#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

struct Student {
    std::string name;
    std::string id;
    int score;
};

// 输入：成功返回 true；遇到无效数据返回 false。
bool inputStudents(std::vector<Student>& students) {
    int n = 0;
    if (!(std::cin >> n) || n < 0) {
        std::cerr << "学生人数必须是非负整数。\n";
        return false;
    }
    for (int i = 0; i < n; ++i) {
        Student student;
        if (!(std::cin >> student.name >> student.id >> student.score)) {
            std::cerr << "学生信息格式错误，应为：姓名 学号 整数成绩。\n";
            return false;
        }
        if (student.score < 0 || student.score > 100) {
            std::cerr << "成绩必须在 0 到 100 之间。\n";
            return false;
        }
        students.push_back(student);
    }
    return true;
}

// 只读遍历：输出当前顺序，不修改学生数据。
// 任务书示例里不带表头，这里保持与示例一致（序号靠空格分隔）。
void printStudents(const std::vector<Student>& students) {
    for (const Student& student : students) {
        std::cout << student.name << ' ' << student.id
                  << ' ' << student.score << '\n';
    }
}

// 统计函数也能独立处理空列表，避免访问空元素与除零。
void printStatistics(const std::vector<Student>& students) {
    if (students.empty()) {
        std::cout << "当前没有学生数据。\n";
        return;
    }
    double total = 0.0;
    int highest = students.front().score;
    int lowest = students.front().score;
    int passed = 0;
    for (const Student& student : students) {
        total += student.score;
        if (student.score > highest) highest = student.score;
        if (student.score < lowest) lowest = student.score;
        if (student.score >= 60) ++passed;
    }
    double average = total / students.size();
    std::cout << "===== 成绩统计 =====\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "平均分：" << average << '\n';
    std::cout << "最高分：" << highest << '\n';
    std::cout << "最低分：" << lowest << '\n';
    std::cout << "及格人数：" << passed << '\n';
    std::cout << "不及格人数：" << students.size() - passed << '\n';
}

// 返回 true 表示 a 应排在 b 前面；同分时按学号字典序升序。
bool higherScoreFirst(const Student& a, const Student& b) {
    if (a.score != b.score) {
        return a.score > b.score;
    }
    return a.id < b.id;
}

void sortStudents(std::vector<Student>& students) {
    std::sort(students.begin(), students.end(), higherScoreFirst);
}

int main() {
#ifdef _WIN32
    // Windows 控制台默认编码是 GBK(936)，而本文件是 UTF-8 编码。
    // 不先切到 UTF-8，下面的中文在 cmd / VS Code 终端里会显示成乱码。
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    std::vector<Student> students;
    if (!inputStudents(students)) {
        return 1;
    }
    if (students.empty()) {
        std::cout << "当前没有学生数据。\n";
        return 0;
    }
    std::cout << "===== 输入的学生信息 =====\n";
    printStudents(students);
    std::cout << '\n';
    printStatistics(students);
    std::cout << '\n';
    sortStudents(students);
    std::cout << "===== 排序后的学生信息 =====\n";
    printStudents(students);
    return 0;
}
