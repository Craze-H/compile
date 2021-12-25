#include "getword.h"
struct lVarNode{
    char* type{};
    char* name{};
    int register_order{};
    bool isConst{};
    int constValue{};
    bool isGlobal = false;
    std::vector<int> array;
    std::vector<int> each_dimension;
    std::vector<int> array_elements;
    lVarNode(int register_order,const char* name, const char* type, bool isConst){
        this->register_order = register_order;
        this->name = new char [strlen(name) + 1];
        strcpy(this->name, name);
        this->type = new char [5];
        strcpy(this->type, type);
        this->isConst = isConst;
    }
    lVarNode(int register_order,const char* name, const char* type, bool isConst, int constValue){
        this->register_order = register_order;
        this->name = new char [strlen(name) + 1];
        this->constValue = constValue;
        strcpy(this->name, name);
        this->type = new char [5];
        strcpy(this->type, type);
        this->isConst = isConst;
    }
};
std::vector<lVarNode>lVarVector;
std::vector<int>judgeStack;
std::vector<int>typeStack;
std::vector<int>loopStack;
bool localFlag = true;
bool iniFlag = false;
bool constFlag = false;
bool storeFlag = false;
bool arrayFlag = false;
int register_num = 0;
int judge_num = 0;
int loop_num = 0;
char reserved[][10] = {"getint","getch","getarray","putint","putch","putarray"};

int getLVar(char* name);
bool checkRepeat(int tmp_pos);
int checkReserved(char* ident_name);

int getLVar(char* name){
    for (int i = lVarVector.size() - 1; i >= 0 ; --i) {
        if (strcmp(lVarVector[i].name, name) == 0){
            return i;
        }
    }
    return -1;
}

bool checkRepeat(int tmp_pos){
    for (int i = lVarVector.size() - 1; i >= 0 ; --i) {
        if (lVarVector[i].register_order == 0){
            return false;
        }
        if (strcmp(lVarVector[i].name, words[tmp_pos].name) == 0){
            return true;
        }
    }
    return false;
}

int checkReserved(char* ident_name){
    for (int i = 0; i < 6; ++i) {
        if (strcmp(ident_name, reserved[i]) == 0){
            return i;
        }
    }
    return -1;
}