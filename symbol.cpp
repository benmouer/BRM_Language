//
// Created by Benjamin Mouer on 12/11/18.
//

#include "symbol.h"
#include <sstream>



symbol::symbol(){
    id = "";
    address = 0;
    value = 0;
    tok_index = 101;
    cons = false;
}




symbol::symbol(string name, int * add, int val, bool constant){
    id = name;
    address = add;
    value = val;
    tok_index = 101;
    cons = constant;
}






void symbol::printsymbol() {
    if(cons){
        cout << "constant - " << id  << ": " << value << ", @ address: " << address << "\n";
    }
    else {
        cout << "variable - " << id << ": " << value << ", @ address: " << address << "\n";
    }
}







int symbol::getIndex() {
    return tok_index;
}

string symbol::getName(){
    return id;
}


void symbol::setIndex(int i){
    tok_index = i;
}

int * symbol::getAddress() {
    return address;
}

int symbol::getValue() {
    return value;
}

int symbol::setValue(int v) {
    value = v;
}

bool symbol::getCons() {
    return cons;
}
