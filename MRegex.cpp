//
// Created by chen on 2018/5/3.
//

#include "MRegex.h"
#include <Regex>
#include <iostream>

//MRegex用判断读入的代码中是否包含预编译命令
using namespace std;
string mPatterns[]={//lab要求实现的10条预编译命令，对其进行了编号
        "#include",//预编译命令1
        "#define",//预编译命令2
        "#ifdef",//预编译命令3
        "#ifndef",//预编译命令4
        "#undef",//预编译命令5
        "#if",//预编译命令6
        "#else",//预编译命令7
        "#endif"//预编译命令8
};
int MRegex::isPrecompilerInstruction(string lineForJudge) {
    //判断该代码行属于预编译命令还是一般代码行
    //如果是预编译命令，是何种预编译命令
   for(int i=0;i<mPatterns->length();i++){
       if(hasStartWith(mPatterns[i],lineForJudge))
           return i+1;
   }
    return 0;
}

bool MRegex::isDigit(const string &isDigit){
    //方便#if 预编译命令的判断
    if((isDigit.length()==1)&&(isDigit[0]>='0'&&(isDigit[0]<='9'))){
        cout<<isDigit<<" is a digit"<<endl;
        return true;
    }else{
        return false;
    }

}

bool MRegex::hasStartWith(string singlePattern,string unjudgedLine) {
    //判断给定的代码是否以预编译命令开头
    return unjudgedLine.compare(0,singlePattern.length(),singlePattern)==0;
}