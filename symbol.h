//
// Created by Benjamin Mouer on 12/11/18.
//

#ifndef INC_333_LEXANALYZER_SYMBOL_H
#define INC_333_LEXANALYZER_SYMBOL_H

#include <iostream>
#include <string>
using namespace std;
#include <vector>

class symbol {

public:

    symbol();

    symbol(string id, int * address, int value, bool cons);

    string getName();

    void printsymbol();

    int * getAddress();

    int getValue();

    int setValue(int v);

    int getIndex();

    void setIndex(int i);

    bool getCons();





private:

    string id;
    int * address;
    int value;
    int tok_index;
    bool cons;

};

#endif //INC_333_LEXANALYZER_SYMBOL_H
