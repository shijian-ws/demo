#include <iostream>
#include <sstream>
#include <math.h>
#include "LinkedList.cpp"


string get(string info) {
    string input;
    cout << info << endl;
    cin >> input;
    return input;
}

int main() {
    LinkedList *link1 = new LinkedList();
    while (true) {
        string first = get("请输入第一个整数");
        if (link1->set(first)) {
            break;
        }
    }

    char symbol;
    while (true) {
        string oper = get("请输入操作符");
        if (oper.empty() || oper.size() != 1) {
            cout << "操作符必须1位" << endl;
            continue;
        }
        if (oper.at(0) != '+' && oper.at(0) != '-') {
            cout << "目前操作符只支持 + 或 - " << endl;
            continue;
        }
        symbol = oper.at(0);
        break;
    }

    LinkedList *link2 = new LinkedList();
    while (true) {
        string second = get("请输入第二个整数");
        if (link2->set(second)) {
            break;
        }
    }

    LinkedList *result = link1->oper(symbol, link2);
    cout << "计算结果: " + result->get() << endl;

    return 0;
}
