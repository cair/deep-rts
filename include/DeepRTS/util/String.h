//
// Created by per on 1/11/21.
//

#ifndef DEEPRTS_STRING_H
#define DEEPRTS_STRING_H
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
class StringUtil{


public:
    static std::vector<std::string> split(const std::string& str,const std::string& sep){
        char* cstr=const_cast<char*>(str.c_str());
        char* current;
        std::vector<std::string> arr;
        current=strtok(cstr,sep.c_str());
        while(current!=NULL){
            arr.emplace_back(current);
            current=strtok(NULL,sep.c_str());
        }
        return arr;
    }
};

#endif //DEEPRTS_STRING_H
