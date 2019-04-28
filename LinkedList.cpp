#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <regex>

using namespace std;

/**
 * 链表节点
 */
typedef struct Node {
    // 当前存储数值
    int number = 0;
    // 下一个节点
    Node *next = nullptr;
    // 上一个节点
    Node *prev = nullptr;
} Node;

/**
 * 双向链表
 */
class LinkedList {
private:
    // 是否负数
    bool isNegative = false;
    // 第一个元素
    Node *first = nullptr;
    // 最后一个元素
    Node *last = nullptr;
    // 元素个数
    int size = 0;

    // 追加一个节点
    void addNode(int number);

    // 设置一组字符串
    bool set(vector<string> vector);

    // 判断当前链是否为0
    bool isZero();

    // 比较2个链表
    int compareTo(LinkedList *link);

    // 加法
    LinkedList *plus(LinkedList *link);

    // 减法
    LinkedList *minus(LinkedList *link);

public:
    // 初始化
    LinkedList() {}

    // 设置字符串
    bool set(string str);

    // 获取存储数值串
    string get();

    /**
     * 操作当前链表与指定链表
     * @param link
     * @param symbol 操作符, +: 2个链表值相加 -: 2个链表值相减
     * @return
     */
    LinkedList *oper(char symbol, LinkedList *link);
};

void LinkedList::addNode(int number) {
    Node *node = new Node(); // 创建节点
    node->number = number; // 节点保存数值

    this->size++; // 元素数量自增

    Node *tmpLast = this->last; // 临时记录当前最后节点
    if (tmpLast == nullptr) {
        // 全新链表
        this->first = node;
        this->last = node;
        return;
    }

    this->last = node; // 修改最后节点指向
    node->prev = tmpLast; // 新节点的上一个节点指向最后节点
    tmpLast->next = node; // 最后节点的下一个节点指向新节点
}

vector<string> split(const string &str) {
    vector<string> result;

    for (int i = 0, pos, y = str.size(); i < y; ++i) {
        pos = str.find(',', i); // 获取,所在索引
        if (pos > -1) {
            string val = str.substr(i, pos - i);
            result.push_back(val);
            i = pos;
            continue;
        }
        // 保存之后字符串
        result.push_back(str.substr(i, str.size()));
        break;
    }

    return result;
}


bool isValid(const vector<string> vector) {
    if (vector.empty()) {
        // 容器为空
        return false;
    }

    for (int i = 0; i < vector.size(); ++i) {
        // 获取容器中每个字符串
        string str = vector.at(i);
        if (str.empty()) {
            // 当前字符串为空
            return false;
        }

        // 迭代每个字符
        for (int y = 0; y < str.size(); ++y) {
            // 当前字符串每个字符
            char c = str[y];

            if (i == 0 && y == 0) {
                // 第一组存下符号情况
                if (str[i] == '-' || str[i] == '+') {
                    // 第一个是正负号, 跳过
                    continue;
                }
            }

            if (c == '\0' && y + 1 == str.size()) {
                continue;
            }

            if (c < '0' || c > '9') {
                // 非数字
                cout << "不能包含非数字的其他字符" << endl;
                return false;
            }

        }
    }

    return true;
}

bool LinkedList::set(const vector <string> vector) {
    bool isStart = true;
    for (int i = 0; i < vector.size(); ++i) {
        string val = vector.at(i);
        if (i == 0 && ((isNegative = (val[0] == '-')) || val[0] == '+')) {
            // 跳过符号
            val = val.substr(1, val.size());
        }

        stringstream ss(val);
        int number = 0;
        ss >> number; // 赋值

        if (isStart && number == 0) {
            // 去除开始的0
            continue;
        }

        isStart = false;
        this->addNode(number);
    }
    if (isStart && this->last == nullptr) {
        // 兜底全部是0
        this->addNode(0);
    }
}

bool LinkedList::isZero() {
    Node *current = this->first;

    if (current != nullptr) {
        do {
            if (current->number != 0) {
                return false;
            }
            current = current->next;
        } while (current != nullptr);
    }

    return true;
}

bool LinkedList::set(const string str) {
    if (str.empty()) {
        cout << "字符串不能为空" << endl;
        return false;
    }

    regex rex("[-|+]?\\d{1,4}(\\,\\d{1,4})*");
    smatch result;
    if (!regex_match(str, result, rex)) {
        cout << "字符串格式错误" << endl;
        return false;
    }

    // 分割
    vector<string> vector = split(str);
    if (!isValid(vector)) {
        return false;
    }

    this->set(vector);

    return true;
}

string LinkedList::get() {
    if (this->first == nullptr) {
        return nullptr;
    }

    stringstream ss;
    if (this->isNegative) {
        ss << '-'; // 写入负号
    }

    bool isStart = true;
    Node *current = this->first;
    do {
        int number = current->number;
        if (!isStart && number < 1000) {
            // 非开头并且当前节点如果小于4位, 用0补齐
            char buffer[5];
            sprintf(buffer, "%04d", number);

            ss << buffer;
        } else {
            ss << number;
        }

        current = current->next;

        if (current != nullptr) {
            ss << ',';
        }

        isStart = false;
    } while (current != nullptr);

    return ss.str();
}

pair<int, int> calcPlus(int number) {
    int remain = 0; // 剩余
    if (number > 9999) {
        remain = number / 10000; // 截取后四位之前
        number = number % 10000; // 截取最后四位
    }
    return make_pair(remain, number);
}

LinkedList *LinkedList::plus(LinkedList *link) {
    int remain = 0; // 剩余

    int length = max(this->size, link->size); // 取最多节点

    Node *thisCurrent = this->last;
    Node *otherCurrent = link->last;

    vector<string> vector;

    do {
        int number = remain;
        if (thisCurrent != nullptr) {
            number += thisCurrent->number;
            thisCurrent = thisCurrent->prev;
        }
        if (otherCurrent != nullptr) {
            number += otherCurrent->number;
            otherCurrent = otherCurrent->prev;
        }

        pair<int, int> pair = calcPlus(number);
        remain = pair.first;
        vector.insert(vector.begin(), to_string(pair.second));
    } while (--length > 0);

    while (remain > 0) {
        // 处理额外剩余
        pair<int, int> pair = calcPlus(remain);
        remain = pair.first;
        vector.insert(vector.begin(), to_string(pair.second));
    }

    LinkedList *newLink = new LinkedList();
    newLink->set(vector);
    newLink->isNegative = this->isNegative;
    return newLink;
}

int LinkedList::compareTo(LinkedList *link) {
    if (link == nullptr) {
        return 1;
    }
    if (this->size > link->size) {
        return 1;
    } else if (this->size < link->size) {
        return -1;
    }

    int length = max(this->size, link->size);
    Node *thisCurrent = this->first;
    Node *otherCurrent = link->first;

    do {
        if (thisCurrent == nullptr && otherCurrent == nullptr) {
            return 0;
        } else if (thisCurrent != nullptr && otherCurrent == nullptr) {
            return 1;
        } else if (thisCurrent == nullptr) {
            return -1;
        }

        int number = thisCurrent->number - otherCurrent->number;
        if (number > 0) {
            return 1;
        } else if (number < 0) {
            return -1;
        }

        thisCurrent = thisCurrent->prev;
        otherCurrent = otherCurrent->prev;

    } while (--length > 0);
    return 0;
}

LinkedList *LinkedList::minus(LinkedList *link) {
    bool isLessThan = false;

    int borrow = 0; // 借位
    int length = this->size;
    Node *maxNode = this->last;
    Node *minNode = link->last;
    if (this->compareTo(link) < 0) {
        maxNode = link->last;
        minNode = this->last;
        length = link->size;
        isLessThan = true;
    }

    vector<string> vector;
    do {
        int number = borrow;
        if (maxNode != nullptr) {
            number += maxNode->number;
            maxNode = maxNode->prev;
        }
        if (minNode != nullptr) {
            number -= minNode->number;
            minNode = minNode->prev;
        }

        if (number < 0) {
            borrow = -1;
            number += 10000;
        } else {
            borrow = 0;
        }

        vector.insert(vector.begin(), to_string(number));
    } while (--length > 0);

    LinkedList *newLink = new LinkedList();
    newLink->set(vector);
    newLink->isNegative = isLessThan;
    return newLink;
}

LinkedList *LinkedList::oper(char symbol, LinkedList *link) {
    if (symbol != '+' && symbol != '-') {
        cout << "暂不支持操作" << endl;
        return nullptr;
    }

    if (link == nullptr) {
        // 拷贝当前
        LinkedList *newLink = new LinkedList();
        newLink->set(this->get());
        return newLink;
    }

    if (this->isZero()) {
        // 拷贝参数
        LinkedList *newLink = new LinkedList();
        newLink->set(link->get());
        return newLink;
    }

    if (symbol == '+') {
        // 相加
        if (this->isNegative == link->isNegative) {
            // 加法
            return plus(link);
        }
        // 减法
        return minus(link);
    }
    // 相减
    if (this->isNegative != link->isNegative) {
        // 加法
        return plus(link);
    }
    // 减法
    return minus(link);
}