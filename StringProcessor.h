//
// Created by chen on 2018/5/8.
//
#include <string>
#include <vector>

#ifndef HEADMATCH_STRINGPROCESSOR_H
#define HEADMATCH_STRINGPROCESSOR_H

using namespace std;
class StringProcessor {
public:
   string replaceSingleSlashToDoubleSlahes(const string &strWithSingleSlash);
    vector<string> split(const string &mLine);
    string keyOfTwo(const string& twoKeyline);

};


#endif //HEADMATCH_STRINGPROCESSOR_H
