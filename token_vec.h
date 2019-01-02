//
// Created by Benjamin Mouer on 10/17/18.
//
using namespace std;
#include <vector>
#include "token.h"
#ifndef INC_333_LEXANALYZER_TOKEN_VEC_H
#define INC_333_LEXANALYZER_TOKEN_VEC_H


class token_vec {
public:

    token_vec();

    explicit token_vec(vector<token> vec);

    void addToVec(token tok);

    string lastToken();

    void printTokens();

    token firstToken();

    token nextToken(token curr);

    token prevToken(token curr);

private:
    vector<token> tokens;

};


#endif //INC_333_LEXANALYZER_TOKEN_VEC_H
