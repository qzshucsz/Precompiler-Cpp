//
// Created by chen on 2018/5/3.
//

#include "IncludeHeadFilePath.h"

using namespace std;

IncludeHeadFilePath::IncludeHeadFilePath(string outputFilePath) {
    //将预编译处理后的结果保存到指定的输出文件中
    outputFilestream.open(outputFilePath,ios_base::app);
    if(!outputFilestream.is_open()){
        cout<<"can not open the output file"<<endl;
        return;
    }
    mHeadRegex = new MRegex();
}

void IncludeHeadFilePath::startPreprocess(string inputFile) {
    //递归遍历#include的所有文件
    cout<<"this file path:"<<inputFile<<endl;
    if(!hasCustomizedIncludeFile(inputFile)){
        //到达递归的base case，不包含其它自定义头文件
        ifstream innerInputFile(inputFile,ios_base::in);
        string tempPath = mStrProcessor.replaceSingleSlashToDoubleSlahes(inputFile);
        outputFilestream<<"# ";
        outputFilestream<<'"'<<tempPath<<'"'<<endl;
//        inputStreamProcess(innerInputFile,inputFile);
        while(getline(innerInputFile,mLine)){
            if(mHeadRegex->isPrecompilerInstruction(mLine)==0)
                outputFilestream<<mLine<<endl;
            else
                doJudgeAndProcess(mLine,mHeadRegex->isPrecompilerInstruction(mLine),inputFile);
        }
    }
    else if(hasCustomizedIncludeFile(inputFile)){
        ifstream outterInputFile(inputFile,ios_base::in);
        inputStreamProcess(outterInputFile,inputFile);
    }else{
        cout<<"special case"<<endl;
    }
}

bool IncludeHeadFilePath::hasCustomizedIncludeFile(string unjudugedFilePath){
    //判断当前文件是否include其它库文件
    ifstream inUnjudgedFile;
    inUnjudgedFile.open(unjudugedFilePath,ios_base::in);
    if(!inUnjudgedFile.is_open()) {
        cout << "can not open input file" << endl;
        return false;
    }
    string mThisLine;
    while(getline(inUnjudgedFile,mThisLine)) {
        if(mHeadRegex->hasStartWith("#include", mThisLine))
            return true;
    }
    return false;
}



void IncludeHeadFilePath::doJudgeAndProcess(const string &line, int judgeCode,const string &inputFileName) {
    //根据输入的代码行归属编号，进行处理
    switch (judgeCode){
        case 0://该行代码不包含任何预处理，直接写入预处理文件
            casezeroProcess(line);
            break;
        case 1://#include预处理
            includeProcess(line);
            break;
        case 2: //#define预处理
            defineProcess(line,inputFileName);
            break;
        case 3://#ifdef预处理
            hasMeetedIfdef=true;
            if(ifdefProcess(line)) {
                isIfdefTrue = true;
                isElseActivated = false;
                isIfndefTrue = false;
                isIfTrue  =false;
                hasMeetedIf =false;
                hasMeetedElse =false;
                hasMeetedIfndef = false;
                ifdefCounter++;
                cout<<"else to 0 in ifdef"<<endl;
            } else
                isIfdefTrue = false;

            break;
        case 4://#ifndef预处理
            hasMeetedIfndef = true;
            if(ifndefProcess(line)) {
                isIfndefTrue = true;
                isIfdefTrue = false;
                isElseActivated = false;
                isIfTrue = false;
                hasMeetedIf= false;
                hasMeetedIfdef =false;
                hasMeetedElse =false;
                ifndefCounter++;
                cout<<"else to 0 in ifndef"<<endl;
            } else
                isIfndefTrue =false;
            break;
        case 5://#undef预处理
            undefProcess(line);
            break;
        case 6://#if预处理
            hasMeetedIf = true;
            if(ifProcess(line)) {
                //cout<<"line :"<<line<<" makes if true"<<endl;
                ifCounter++;
                isIfTrue = true;
                isElseActivated = false;
                isIfdefTrue =false;
                isIfndefTrue =false;
                hasMeetedIfdef =false;
                hasMeetedIfndef =false;
                hasMeetedElse =false;
                cout<<"else to 0 in if"<<endl;
            } else
                isIfTrue = false;
            break;
        case 7://#else预处理
            hasMeetedElse = true;
            isElseActivated = !(isIfTrue || isIfdefTrue || isIfndefTrue);
            if(isElseActivated) {
                hasMeetedIf =false;
                hasMeetedIfdef =false;
                hasMeetedIfndef =false;
                elseCounter++;
            }
            cout<<"else is :"<<isElseActivated<<endl;
            if(!isElseActivated){
                isIfTrue = false;
                isIfdefTrue = false;
                isIfndefTrue = false;

                //cout<<"set isIfTrue to false"<<endl;
            }
            break;
        case 8://#endif预处理
            endifProcess();
            break;
        default:
            break;
    }
}

void IncludeHeadFilePath::includeProcess(const string &includeLine) {
    //处理预编译命令#include
    string includeFilePath;//保存遍历时得到的#include文件名
    //判断是标准库还是自定义库
    if(includeLine.find("<")!=string::npos){//若是标准库文件则不管
        cout<<"a standard lib"<<includeLine<<endl;
    }else{//若是自定义库，则继续
        unsigned int mFirstPosition = includeLine.find_first_of('"');
        unsigned int mSecondPosition = includeLine.find_first_of('"',mFirstPosition+1);
        includeFilePath = includeLine.substr(mFirstPosition+1,mSecondPosition-mFirstPosition-1);
        if(includeFilePath.find(".h")!=string::npos) {
            cout<<"path:"<<includeFilePath<<endl;
            startPreprocess("D:\\lab2\\" + includeFilePath);
            isFirstTimeWriteFileName = true;
        }
    }
}

void IncludeHeadFilePath::defineProcess(const string &defineLine,const string &defineFileName) {
    //处理预编译命令#define
    //利用multimap保存#define内容，宏的名称存储在自定义结构kvNode的key中
    //对应的字符存储在kvNode的value中
    mKvNode myNode;
    vector<string> mStrs = mStrProcessor.split(defineLine);
    bool isDefineSign = true;
    bool hasKeyStored = false;
    for(auto &s:mStrs){
        if(isDefineSign) {
            isDefineSign = false;
            continue;
        }
        else if(!hasKeyStored){
            myNode.key = s;
            hasKeyStored=true;
        }else{
            myNode.value = s;
        }
    }
    mMap.insert(_KV_(defineFileName.substr(0,defineFileName.find_last_of('.')),myNode));
}

bool IncludeHeadFilePath::ifdefProcess(const string &ifdefLine) {
    //处理预编译命令#ifdef
    string keyOfIfdef = mStrProcessor.keyOfTwo(ifdefLine);
    return  isDefined(keyOfIfdef);

}

bool IncludeHeadFilePath::ifndefProcess(const string &ifndefLine) {
    //处理预编译命令#ifndef
    string keyOfIfndef = mStrProcessor.keyOfTwo(ifndefLine);
    return !isDefined(keyOfIfndef);
}

void IncludeHeadFilePath::undefProcess(const string &undefLine) {
    //处理预编译命令#undef
    string keyOfUndef = mStrProcessor.keyOfTwo(undefLine);
    if(isDefined(keyOfUndef)){
        auto it=mMap.begin();
        while(it!=mMap.end()){
            if(it->second.key.compare(keyOfUndef)==0){
                mKvNode nullNode;
                nullNode.key="deleted";
                nullNode.value="0";
                it->second = nullNode;
                break;
            }
            it++;
        }
    }else{
        cout<<"error: no define the key '"<<keyOfUndef<<"' before undef"<<endl;
    }

}

bool IncludeHeadFilePath::ifProcess(const string &ifLine) {
    //处理预编译命令#if
    cout<<"content of if:"<<ifLine<<endl;
    string keyOfIf = mStrProcessor.keyOfTwo(ifLine);
    string digitOfIf = convertSignToDigit(keyOfIf);
    if(digitOfIf=="0"){
        cout<<"if is false"<<endl;
        return false;
    }else if(digitOfIf=="1"){
        cout<<"if is true"<<endl;
        return true;
    }else{
        cout<<"neither 0 or 1"<<endl;
    }


}

//void IncludeHeadFilePath::elseProcess() {
//    if(isElseActivated && !hasReachEndif){
//        outputFilestream<<mLine<<endl;
//    }
//    isElseActivated = false;
//}

void IncludeHeadFilePath::endifProcess() {
    //处理预编译命令#endif
    if(ifCounter>0){
        isIfTrue = true;
    }
    if(ifdefCounter>0){
        isIfdefTrue = true;
    }
    if(ifndefCounter>0){
        isIfndefTrue = true;
    }
    if(elseCounter>0){
        isElseActivated =true;
    }
    if(isIfTrue){
        outputFilestream<<ifLinesToWrite<<endl;
        ifLinesToWrite.clear();
    }else if(isIfdefTrue){
        outputFilestream<<ifdefLinesToWrite<<endl;
        ifdefLinesToWrite.clear();
    }else if(isIfndefTrue){
        outputFilestream<<ifndefLinesToWrite<<endl;
        ifndefLinesToWrite.clear();
    }else if(isElseActivated){
        outputFilestream<<elseLinesToWrite<<endl;
        elseLinesToWrite.clear();
    }
    hasMeetedIfdef = false;
    hasMeetedIf = false;
    hasMeetedIfndef = false;
    hasMeetedElse = false;
    isElseActivated = false;
    cout<<"else to 0 in endif"<<endl;
    isIfTrue = false;
    isIfndefTrue = false;
    isIfdefTrue = false;
    ifCounter = 0;
    ifdefCounter = 0;
    ifndefCounter =0;
    elseCounter =0;
}

void IncludeHeadFilePath::inputStreamProcess(ifstream &inputStream, const string &inputFileName) {
    //处理输入的代码文件
    while(getline(inputStream,mLine)){
        if(!mHeadRegex->hasStartWith("#",mLine)&&isFirstTimeWriteFileName){
            outputFilestream<<"# ";
            outputFilestream<<'"'<<mStrProcessor.replaceSingleSlashToDoubleSlahes(inputFileName)<<'"'<<endl;
            isFirstTimeWriteFileName = false;
        }
        signOfLine = mHeadRegex->isPrecompilerInstruction(mLine);
        doJudgeAndProcess(mLine,signOfLine,inputFileName);
    }
    inputStream.close();
}

bool IncludeHeadFilePath::isDefined(const string &defineKey)  {
    //判断宏#define xxx是否已定义
    auto it= mMap.begin();
    while(it!=mMap.end()){
        if(defineKey.compare(it->second.key)==0) {
            cout<<"has defined key:"<<defineKey<<endl;
            return true;
        }
        it++;
    }
    return false;
}


string IncludeHeadFilePath::convertSignToDigit(const string &sign) {
    //将#define预定义的宏替换成对应的单个数字形式
    //cout<<"convert content:"<<sign<<endl;
    if(mHeadRegex->isDigit(sign))
        return sign;
    auto it = mMap.begin();
    while(it!=mMap.end()){
        if(it->second.key==sign){
            return convertSignToDigit(it->second.value);
        }
        it++;
    }

}

void IncludeHeadFilePath::casezeroProcess(const string &line) {
    //处理不包含预处理命令的代码行
    string replacedStr = replaceDefine(line);
    if(hasMeetedIf&&isIfTrue){
        //cout<<"if append:"<<line<<endl;
        ifLinesToWrite.append(replacedStr+"\n");
    }else if(hasMeetedIf&&!isIfTrue){
        return;
    }
    else if(hasMeetedIfdef&&isIfdefTrue){
        ifdefLinesToWrite.append(replacedStr+"\n");
    }else if(hasMeetedIfdef&&!isIfdefTrue){
        return;
    }
    else if(hasMeetedIfndef&&isIfndefTrue){
        ifndefLinesToWrite.append(replacedStr+"\n");
    }else if(hasMeetedIfndef&&!isIfndefTrue){
        return;
    }else if(hasMeetedElse && isElseActivated){
        cout<<"else append:"<<replacedStr<<endl;
        elseLinesToWrite.append(replacedStr+"\n");
    }else if(hasMeetedElse &&!isElseActivated){
        return;
    }else{
//        cout<<"write out directly:"<<line<<endl;
        outputFilestream << replacedStr << endl;
    }
}

string IncludeHeadFilePath::replaceDefine(const string &line) {
    //用来替换已定义的宏
    auto it = mMap.begin();
    while(it!=mMap.end()){
        if(line.find(it->second.key)!=string::npos){
            cout<<"find define content:"<<it->second.key<<endl;
            string afterReplacedStr = line;
            afterReplacedStr.replace(line.find(it->second.key),(it->second.key).length(),"");
            afterReplacedStr.insert(line.find(it->second.key),it->second.value);
            cout<<"replace to:"<<afterReplacedStr<<endl;
            return afterReplacedStr;
        }
        it++;
    }
    return line;
}

void IncludeHeadFilePath::end(){
    outputFilestream.close();
}