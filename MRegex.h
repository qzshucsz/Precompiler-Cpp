//
// Created by chen on 2018/5/3.
//
#include <string>

#ifndef HEADMATCH_MREGEX_H
#define HEADMATCH_MREGEX_H


using namespace std;

class MRegex {
public:
    int isPrecompilerInstruction(string lineForJudge);
    bool hasStartWith(string singlePattern,string unjudgedLine);
    bool isDigit(const string &isDigit);
};


#endif //HEADMATCH_MREGEX_H
