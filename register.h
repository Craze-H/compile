#include "getword.h"
struct lVarNode{
    char* type{};
    char* name{};
    int register_order{};
    bool isConst{};
    int constValue{};
    lVarNode(int register_order,const char* name, const char* type, bool isConst){
        this->register_order = register_order;
        this->name = new char [1005];
        strcpy(this->name, name);
        this->type = new char [5];
        strcpy(this->type, type);
        this->isConst = isConst;
    }
    lVarNode(int register_order,const char* name, const char* type, bool isConst, int constValue){
        this->register_order = register_order;
        this->name = new char [1005];
        this->constValue = constValue;
        strcpy(this->name, name);
        this->type = new char [5];
        strcpy(this->type, type);
        this->isConst = isConst;
    }
};
std::vector<lVarNode>lVarVector;
bool constFlag = false;
bool storeFlag = false;
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