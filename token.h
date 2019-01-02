//
// Created by Benjamin Mouer on 10/17/18.
//

#ifndef INC_333_LEXANALYZER_TOKEN_H
#define INC_333_LEXANALYZER_TOKEN_H
#include <iostream>
#include <string>
using namespace std;
#include <vector>

class token {

public:

    token();

    token(bool accept, string theToken, string opName, string name, int value);

    void printToken();

    int getIndex();

    string getTheToken();

    void setIndex(int i);

    string getName();

    bool getValid();

    int getVal();
    int setVal(int i);



private:

    bool valid;
    string theToken;
    string opName;
    string name;
    int value;
    int tok_index;

};


#endif //INC_333_LEXANALYZER_TOKEN_H
