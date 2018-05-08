#include <iostream>
#include "IncludeHeadFilePath.h"
using namespace std;
int main(int argc, char*  argv[]) {
    string inputFilePath;
    string outputFilePath;
    cout<<"arguments:"<<argc<<endl;
    cout<<"args:"<<endl;
    for (int i = 0; i < argc; i++) {
        cout<<argv[i]<<endl;
    }
    if(argc!=3){
        cout<<"Usage: unprecompilercppname outputfilename"<<endl;
        return 1;
    }else {
        for (int i = 0; i < argc; i++) {
            inputFilePath = argv[1];
            outputFilePath = argv[2];
        }
        IncludeHeadFilePath *mUnpreprocessdFile = new IncludeHeadFilePath(outputFilePath);
        mUnpreprocessdFile->startPreprocess(inputFilePath);
        mUnpreprocessdFile->end();
        cout<<"precompiler '"<<inputFilePath<<"' finished!"<<endl;
    }
    return 0;
}