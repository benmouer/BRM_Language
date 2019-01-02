//
// Created by Benjamin Mouer on 12/11/18.
//

#ifndef INC_333_LEXANALYZER_SYMBOL_TABLE_H
#define INC_333_LEXANALYZER_SYMBOL_TABLE_H
using namespace std;
#include <vector>
#include "symbol.h"


class symbol_table {
public:

    symbol_table();

    explicit symbol_table(vector<symbol> vec);

    void addToTable(symbol sym);

    string lastSymbol();

    void printSymbols();

    symbol findSymbol(string id);

    symbol nextSymbol(symbol curr);

    void changeVal(string s, int i);

    int findVal(string s);

private:
    vector<symbol> symbols;

};


#endif //INC_333_LEXANALYZER_SYMBOL_TABLE_H
