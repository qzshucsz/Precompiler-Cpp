//
// Created by chen on 2018/5/3.
//
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <Regex>
#include "MRegex.h"
#include "StringProcessor.h"

#ifndef HEADMATCH_INCLUDEHEADFILEPATH_H
#define HEADMATCH_INCLUDEHEADFILEPATH_H

using namespace std;
typedef struct kvNode{
    string key;
    string value;
}mKvNode;

typedef pair <string,struct kvNode> _KV_;
class IncludeHeadFilePath {
public:
    explicit IncludeHeadFilePath(string outputFilePath);
    void startPreprocess(string inputFilePath);
    void end();
    MRegex *mHeadRegex;

private:
    bool hasCustomizedIncludeFile(string unjudugedFilePath);
    void doJudgeAndProcess(const string &line,int judgeCode,const string &inputFileName);
    void includeProcess(const string &includeLine);
    void defineProcess(const string &defineLine,const string &defineFileName);
    bool ifdefProcess(const string &ifdefLine);
    bool ifndefProcess(const string &ifndefLine);
    void undefProcess(const string &undefLine);
    bool ifProcess(const string &ifLine);

    void endifProcess();
    void inputStreamProcess(ifstream &inputStream,const string &inputFileName);

    bool isDefined(const string &defineKey);
    string convertSignToDigit(const string &sign);
    void casezeroProcess(const string &line);
    string replaceDefine(const string &line);

    ofstream outputFilestream;
    string mLine;
    multimap<string,kvNode> mMap;
    int signOfLine;
    bool isElseActivated = false;

    bool isIfTrue = false;
    bool isIfdefTrue = false;
    bool isIfndefTrue = false;

    string ifLinesToWrite;
    string elseLinesToWrite;
    string ifdefLinesToWrite;
    string ifndefLinesToWrite;
    bool isFirstTimeWriteFileName = true;
    bool hasMeetedIfdef = false;
    bool hasMeetedIf = false;
    bool hasMeetedIfndef = false;
    bool hasMeetedElse = false;

    int ifCounter = 0;
    int elseCounter = 0;
    int ifdefCounter = 0;
    int ifndefCounter =0;

    //字符串处理器
    StringProcessor mStrProcessor = StringProcessor();


};


#endif //HEADMATCH_INCLUDEHEADFILEPATH_H
