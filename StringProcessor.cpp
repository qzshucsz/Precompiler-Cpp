//
// Created by chen on 2018/5/8.
//

#include <iostream>
#include <sstream>
#include <iterator>
#include "StringProcessor.h"
using namespace std;
//字符串处理器，提供的借口有
//replaceSingleSlashToDoubleSlahes(const string &strWithSingleSlash)将路径分割符中的'/'替换成'//'
//split(const string &mLine),根据分割符将字符串分割成多个词
//keyOfTwo(const string& twoKeyline)，分离预编译命令和其后面的内容

string StringProcessor::replaceSingleSlashToDoubleSlahes(const string &strWithSingleSlash) {
    //将单斜杠路径(/)同一替换成双斜杠(//)路径分隔符
    string mTempStr = strWithSingleSlash;
    cout<<"the passed path:"<<mTempStr<<endl;
    for(unsigned int i=0;i<mTempStr.length();i++){
        if(mTempStr[i]=='\\') {
            cout << "pos:" << i << endl;
            mTempStr.replace(i,1,"\\\\");
            ++i;
        }
    }
    cout<<"after replace:"<<mTempStr<<endl;
    return mTempStr;
}

vector<string> StringProcessor::split(const string &mLine) {
    //根据空格将字符串分割成多个词
    istringstream mStrStream(mLine);
    vector<string> strs{istream_iterator<string>(mStrStream),istream_iterator<string>()};
    return strs;
}

string  StringProcessor::keyOfTwo(const string& twoKeyline){
    //分离预编译命令和其后面的内容
    vector<string> keys = split(twoKeyline);
    bool isDefineSign = true;
    for(auto &s: keys) {
        if (isDefineSign) {
            isDefineSign = false;
            continue;
        } else {
            return s;
        }
    }
}

