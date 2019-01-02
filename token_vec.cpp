//
// Created by Benjamin Mouer on 10/17/18.
//
#include <iostream>
#include <vector>
#include "token_vec.h"
using namespace std;


token_vec::token_vec(){
    tokens;
}




void token_vec::addToVec(token tok) {
    tokens.push_back(tok);
    tokens.at(tokens.size()-1).setIndex(tokens.size()-1);
}


token token_vec::firstToken(){
    return tokens.at(0);
}


token token_vec::nextToken(token curr){
    bool complete;
    token next;
    do{
        next = tokens.at(curr.getIndex()+1);
        complete = next.getValid();
    }while(!complete);
    return next;
}

token token_vec::prevToken(token curr){
    bool complete;
    token prev;
    do{
        prev = tokens.at(curr.getIndex()-1);
        complete = prev.getValid();
    }while(!complete);
    return prev;
}


string token_vec::lastToken(){
    return tokens.at(tokens.size()-1).getTheToken();
}




void token_vec::printTokens() {
    for (auto &token : tokens) {
        token.printToken();
    }
}




token_vec::token_vec(vector<token> vec) {
    tokens = std::move(vec);
};
