//
// Created by Benjamin Mouer on 12/11/18.
//

#include <iostream>
#include <vector>
#include "symbol_table.h"
using namespace std;


symbol_table::symbol_table(){
    symbols;
}




void symbol_table::addToTable(symbol sym) {
    bool check = true;
    for (auto &symbol : symbols) {
        if (symbol.getName() == sym.getName()){
            check = false;
        }
    }
    if (check) {
        symbols.push_back(sym);
        symbols.at(symbols.size() - 1).setIndex(symbols.size() - 1);
    }
    else{
        cout << "WARNING: VARIABLE OR CONSTANT DECLARED MORE THAN ONCE: " << sym.getName() << "\n";
    }
}



symbol symbol_table::nextSymbol(symbol curr){
    bool complete;
    symbol next;
    do{
        next = symbols.at(curr.getIndex()+1);
        complete = true;
    }while(!complete);
    return next;
}


string symbol_table::lastSymbol(){
    return symbols.at(symbols.size()-1).getName();
}




void symbol_table::printSymbols() {
    for (auto &symbol : symbols) {
        symbol.printsymbol();
    }
}




symbol_table::symbol_table(vector<symbol> vec) {
    symbols = std::move(vec);
}

symbol symbol_table::findSymbol(string id) {
    for (auto &symbol : symbols) {
        if (symbol.getName() == id) {
            return symbol;
        }
    }
    cout << "symbol '" << id << "' not declared before begin." << endl;
}

void symbol_table::changeVal(string s, int i) {
    for (auto &symbol : symbols) {
        if (symbol.getName() == s) {
            symbol.setValue(i);
        }
    }
};

int symbol_table::findVal(string s) {
    for (auto &symbol : symbols) {
        if (symbol.getName() == s) {
            return symbol.getValue();
        }
    }
};
