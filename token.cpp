//
// Created by Benjamin Mouer on 10/17/18.
//

#include "token.h"
#include <sstream>



token::token(){
    valid = false;
    theToken = "";
    opName = "";
    name = "";
    value = 0;
    tok_index = 0;
}




token::token(bool accept, string tok, string op, string n, int val){
    valid = accept;
    theToken = std::move(tok);
    opName = std::move(op);
    name = std::move(n);
    value = val;
    tok_index = 0;
    stringstream str;
    string s1 = name;
    str << s1;
    str >> value;
}






void token::printToken() {
    if(valid) {
        if (theToken == "identifier") {
            cout << theToken << " (" << name << ")" << "\n"; }
        if (theToken == "reserved word") {
            cout << name << "\n"; }
        if (theToken == "number") {
            stringstream str;
            string s1 = name;
            str << s1;
            str >> value;
            cout << theToken << " (" << value << ")" << "\n"; }
        if (theToken == "operator") {
            cout << opName << "\n"; }
        if (theToken == "hexadecimal") {
            stringstream str;
            string s1 = name;
            str << s1;
            str >> hex >> value;
            cout << "number" << " (" << value << ")" << "\n"; }
        if (theToken == "nomore"){
            cout << theToken << "\n";
        }
    }


    else {

        cout << theToken << " (" << name << ")" << "\n";
        if ((theToken == "Malformed number:") || (theToken == "Lexical error:")){
            cout << "nothing" << "\n";
        }
    }
}






int token::getIndex() {
    return tok_index;
}

string token::getTheToken(){
    if (theToken == "reserved word") {
        return name;
    }
    else if (theToken == "identifier"){
        return theToken;
    }
    else if (theToken == "number" || theToken == "hexadecimal"){
        return "number";
    }
    else if (theToken == "operator"){
        return opName;
    }
    else if (theToken == "nomore"){
        return "nomore";
    }
    else cout << "ERROR IN GETTING TOKEN FOR PARSER..." << endl;
    exit(-1);
}


void token::setIndex(int i){
    tok_index = i;
}

bool token::getValid() {
    return valid;
}

string token::getName() {
    return name;
}

int token::getVal() {
    return value;
}

int token::setVal(int i) {
    value = i;
}






