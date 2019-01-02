/* Benjamin Mouer
 * Lexical Analyzer
 * 10/17/18
 * CSC 333
*/


#include<iostream>
#include<fstream>
#include <deque>
#include <sstream>
#include <queue>
#include <utility>
#include "token.h"
#include "token_vec.h"
#include "symbol.h"
#include "symbol_table.h"
using namespace std;





//defining my functions
//also creating an array of my reserved words
int isKeyword(const string &token_string);
bool isOp(char ch);
token_vec analyze();
bool parse(token_vec tok);
queue<char> removeMal(queue<char> q, char mal);
queue<char> removeLast(queue<char> q);
const char keywords[19][20] = {"const","var","begin","end","if","then","else","while","do","for",
                         "to","and","or","not","read","write","readchar","writechar","EOF"};
symbol_table symbs;
token Program(token tok, token_vec vec);
token ProgramP(token tok, token_vec vec);
token ProgramPP(token tok, token_vec vec);
token ProgramPPP(token tok, token_vec vec);
token ProgramPPPP(token tok, token_vec vec);
token CS(token tok, token_vec vec);
token CSP(token tok, token_vec vec);
token stmt(token tok, token_vec vec);
token stmtP(token tok, token_vec vec);
token ioS(token tok, token_vec vec);
token lVal(token tok, token_vec vec);
token lValP(token tok, token_vec vec);
token val(token tok, token_vec vec);
token valP(token tok, token_vec vec);
token BE(token tok, token_vec vec);
token BEP(token tok, token_vec vec);
token BT(token tok, token_vec vec);
token BTP(token tok, token_vec vec);
token BF(token tok, token_vec vec);
token cond(token tok, token_vec vec);
token condP(token tok, token_vec vec);
token exp(token tok, token_vec vec);
token expP(token tok, token_vec vec);
token term(token tok, token_vec vec);
token termP(token tok, token_vec vec);
token fact(token tok, token_vec vec);
token factP(token tok, token_vec vec);
token Const(token tok, token_vec vec);







int main()
{
    token_vec tokens;
    tokens = analyze();
    bool parseCheck;
    parseCheck = parse(tokens);
}

token_vec analyze() {


    //reads my file and exits if file is not read.
    ifstream fin;
    fin.open("/Users/BenjaminMouer/CLionProjects/333_LexAnalyzer/program.txt", ios::in);

    if (!fin.is_open()) {
        cout << "Could not open program file." << endl;
        exit (EXIT_FAILURE);
    }





    /*Defining the state, the char to be read, and two states needed to stop reading a lexeme
     * also an object vector for my tokens*/
    int state; //machine state
    char ch; //Input char
    int dead_state = 5;
    int end_state = 6;
    int prev_case;
    int num;
    stringstream str;
    string s1;


    token_vec allTokens;





    /*the beginning of the proccess, will read chars and ultimately lexemes until EOF*/
        fin.get(ch);
        string token_string;
        queue<char> buffer;
        queue<char> backupBuffer;
        state = 0;
        int newValue;
        int oldValue;
        bool hex = false;






    while (!fin.eof()) {


        s1 = "";
        num = 0;

        //clearing the buffer and vars for safety
        while (!buffer.empty()) {
                buffer.pop();
        }
        while (!backupBuffer.empty()) {
            backupBuffer.pop();
        }



        //clearing out my values
        token_string = "";
        state = 0;
        newValue = 0;
        oldValue = 0;
        hex = false;







        //If begins with a letter, start the DFA to determine the full identifier (or reserved word)
        if (isalpha(ch)) {
            do {
                buffer.push(ch);
                if ((ch != ' ') && (ch != '\n')) {
                    switch (state) {
                        case 0:
                            if (isalpha(ch)) {
                                state = 1;
                                prev_case = state;
                            } else state = dead_state;
                            break;
                        case 1:
                            if (isalpha(ch)) {
                                state = 1;
                                prev_case = state;
                            } else if (isdigit(ch)) {
                                state = 1;
                                prev_case = state;
                            } else if (ch == '_') {
                                state = 1;
                                prev_case = state;
                            } else state = dead_state;
                            break;
                        default:
                            state = dead_state;
                            break;
                    }
                }
                if(state != dead_state){fin.get(ch);}
            } while ((ch != ' ') && (state != dead_state) && (ch != '\n') && !fin.eof());



            if ((((state == 5)&&(isOp(ch))) || ((ch == ' ') || (ch == '\n')) || fin.eof())) {
                if (isOp(ch)){buffer = removeLast(buffer);}
                while (!buffer.empty()) {
                    token_string.push_back((buffer.front()));
                    buffer.pop();
                }
                //determines what kind of lexeme (identifier or reserved word) and prints
                int token_ident = isKeyword(token_string);
                if (token_ident == -1) {
                    token tok(true, "identifier", "", token_string, '\0');
                    allTokens.addToVec(tok);
                } else {
                    token tok(true, "reserved word", keywords[token_ident], token_string, 0);
                    allTokens.addToVec(tok);
                }
            }



            else {
                char mal = ch;
                string s(1, mal);
                fin.get(ch);
                while ((ch != ' ') && (ch != '\n') && !isOp(ch) && !fin.eof()) {
                    buffer.push(ch);
                    fin.get(ch);
                }
                token tok(false, "Illegal character:", "", s , '\0');
                allTokens.addToVec(tok);
                buffer = removeMal(buffer, mal);
                //creates string from the chars in buffer before a dead state occured
                while (!buffer.empty()) {
                    token_string.push_back((buffer.front()));
                    buffer.pop();
                }
                //determines what kind of lexeme (identifier or reserved word) and prints
                int token_ident = isKeyword(token_string);
                if (token_ident == -1) {
                    token tok(true, "identifier", "", token_string, '\0');
                    allTokens.addToVec(tok);
                } else {
                    token tok(true, "reserved word", keywords[token_ident], token_string, 0);
                    allTokens.addToVec(tok);
                }
                ch = ' ';
            }
        }
















        //same process for numbers
        else if (isdigit(ch) && (ch != '0')) {
            do {
                buffer.push(ch);
                if ((ch != ' ') && (ch != '\n')) {
                    switch (state) {
                        case 0:
                            if (isdigit(ch)) {
                                state = 1;
                                prev_case = state;
                            } else state = dead_state;
                            break;
                        case 1:
                            if (isdigit(ch)) {
                                state = 1;
                                prev_case = state;
                            } else state = dead_state;
                            break;
                        default:
                            state = dead_state;
                            break;
                    }
                }
                if(state != dead_state){fin.get(ch);}
            } while ((ch != ' ') && (state != dead_state) && (ch != '\n') && !fin.eof());



            if ((((state == 5) && (isOp(ch))) || ((ch == ' ') || (ch == '\n') || !isalpha(ch) || fin.eof()))) {
                if (isOp(ch)){buffer = removeLast(buffer);}
                while (!buffer.empty()) {
                    token_string.push_back((buffer.front()));
                    buffer.pop();
                }

                s1 = token_string;
                str << s1;
                str >> num;

                token tok(true, "number", "", token_string, num);
                allTokens.addToVec(tok);
            }


            else{
                fin.get(ch);
                while((ch != ' ') && (ch != '\n') && !isOp(ch) && !fin.eof()){
                    buffer.push(ch);
                    fin.get(ch);
                }
                while (!buffer.empty()) {
                    token_string.push_back((buffer.front()));
                    buffer.pop();
                }
                token tok(false, "Malformed number:", "", token_string , '\0');
                allTokens.addToVec(tok);
                ch = ' ';
            }

        }



















        //this is for positive hex numbers
        else if (isdigit(ch) && (ch == '0')) {
                do {
                buffer.push(ch);
                if ((ch != ' ') && (ch != '\n')) {
                    switch (state) {
                        case 0:
                            if (ch == '0'){
                                state = 1;
                                prev_case = state;
                            }
                            break;
                        case 1:
                            if (ch == 'x'){
                                state = 2;
                                prev_case = state;
                                hex = true;
                            }
                            else if (isdigit(ch)){
                                state = 3;
                                prev_case = state;
                            }
                            else {
                                state = dead_state;
                            }
                            break;
                        case 2:
                            if ((ch == 'a')||(ch == 'b')||(ch == 'c')||(ch == 'd')||
                                    (ch == 'e')||(ch == 'f')||isdigit(ch)) {
                                state = 2;
                                prev_case = 2;

                            }
                            else{
                                state = dead_state;
                            }
                            break;
                        case 3:
                            if (isdigit(ch)) {
                                state = 1;
                                prev_case = state;}
                            else state = dead_state;
                            break;
                        default:
                            state = dead_state;
                            break;
                    }
                }
                    if(state != dead_state){fin.get(ch);}
            } while ((ch != ' ') && (state != dead_state) && (ch != '\n') && !fin.eof());




            if(((ch == ' ') || (ch == '\n') || !isalpha(ch)) || fin.eof() || ((state == 5) && (isOp(ch)))){
                if (isOp(ch)){buffer = removeLast(buffer);}
                while (!buffer.empty()) {
                    token_string.push_back((buffer.front()));
                    buffer.pop();
                }
                if (token_string == "0x"){
                    token tok(false, "Lexical error:", "", token_string , '\0');
                    allTokens.addToVec(tok);
                }
                else if (!hex) {

                    s1 = token_string;
                    str << s1;
                    str >> num;

                    token tok(true, "number", "", token_string, num);
                    allTokens.addToVec(tok);
                }
                else if (hex){

                    s1 = token_string;
                    str << s1;
                    str >> hex >> num;
                    token tok(true, "hexadecimal", "", token_string, num);
                    allTokens.addToVec(tok);
                }




            } else {
                fin.get(ch);
                while((ch != ' ') && (ch != '\n') && !isOp(ch) && !fin.eof()){
                    buffer.push(ch);
                    fin.get(ch);
                }
                while (!buffer.empty()) {
                    token_string.push_back((buffer.front()));
                    buffer.pop();
                }
                token tok(false, "Malformed number:", "", token_string , '\0');
                allTokens.addToVec(tok);
                ch = ' ';
            }

        }
















        //this is for getting negative numbers, both hex and decimal
        else if (ch == '-') {
            string opName;
            do {
                buffer.push(ch);
                if ((ch != ' ') && (ch != '\n')) {
                    switch (state) {
                        case 0:
                            if (ch == '-') {
                                state = 1;
                                prev_case = state;
                                opName = "subtract";
                            }
                            break;
                        case 1:
                            if (isdigit(ch) && (ch !='0')) {
                                state = 4;
                                prev_case = state;
                            }
                            else if(ch == '0'){
                                state = 2;
                                prev_case = state;
                            }
                            else if (isalpha(ch)) {
                                state = 7;
                            }
                            break;
                        case 2:
                            if (ch == 'x'){
                                state = 3;
                                prev_case = state;
                                hex = true;
                            }
                            else if (isdigit(ch)){
                                state = 4;
                                prev_case = state;
                            }
                            else{
                                state = dead_state;
                            }
                            break;
                        case 3:
                            if ((ch == 'a')||(ch == 'b')||(ch == 'c')||(ch == 'd')||
                                (ch == 'e')||(ch == 'f')||isdigit(ch)) {
                                state = 3;
                            }
                            else{
                                state = dead_state;
                            }
                            break;
                        case 4:
                            if (isdigit(ch)) {
                                state = 4;
                                prev_case = state;
                            }
                            else state = dead_state;
                            break;
                        case 7:
                            state = dead_state;
                        default:
                            state = dead_state;
                            break;
                    }
                }
                    if(state != dead_state){fin.get(ch);}
                } while ((ch != ' ') && (state != dead_state) && (ch != '\n') && !fin.eof());



            if(((state == 5) && (isOp(ch))) || (((ch == ' ') || (ch == '\n') || fin.eof()) && (!isalpha(ch)))){
                if (isOp(ch)){buffer = removeLast(buffer);}
                char special = buffer.back();
                while (!buffer.empty()) {
                    token_string.push_back((buffer.front()));
                    buffer.pop();
                }
                if (hex){

                    s1 = token_string;
                    str << s1;
                    str >> hex >> num;

                    token tok(true, "hexadecimal", "", token_string, num);
                    allTokens.addToVec(tok);
                    ch = ' ';
                }
                else {
                    if(state == 7){
                        token tok(true, "operator", "subtract", "-", '\0');
                        allTokens.addToVec(tok);
                        ch = special;
                    }
                    else if (state == 1) {
                        token tok(true, "operator", opName, token_string, '\0');
                        allTokens.addToVec(tok);
                        ch = ' ';
                    }
                    else if (state == 4) {
                        if((allTokens.lastToken() == "identifier") || (allTokens.lastToken() == "number")
                                                                     || (allTokens.lastToken() == "hexadecimal")) {
                            token tok(true, "operator", "subtract", "-", '\0');
                            allTokens.addToVec(tok);
                            token_string.replace(0, 1, "");
                            s1 = token_string;
                            str << s1;
                            str >> num;
                            token tok2(true, "number", "", token_string, num);
                            allTokens.addToVec(tok2);
                            ch = ' ';
                        }
                        else {
                            s1 = token_string;
                            str << s1;
                            str >> num;
                            token tok(true, "number", "", token_string, num);
                            allTokens.addToVec(tok);
                            ch = ' ';
                        }
                    }
                    else if (state == 2) {
                        token tok(true, "operator", "subtract", "-", '\0');
                        allTokens.addToVec(tok);
                        token tok2(true, "number", "", "0", 0);
                        allTokens.addToVec(tok2);
                        ch = ' ';
                    }
                }
            }


            else {
                fin.get(ch);
                while((ch != ' ') && (ch != '\n') && !isOp(ch) && !fin.eof()){
                    buffer.push(ch);
                    fin.get(ch);
                }
                while (!buffer.empty()) {
                    token_string.push_back((buffer.front()));
                    buffer.pop();
                }
                token tok(false, "Malformed number:", "", token_string, '\0');
                allTokens.addToVec(tok);
                ch = ' ';
            }
        }


















        //here is for my operators, after a full operator is read, i stop reading.
        else if ((ch == '.') || (ch == ',') || (ch == ';') || (ch == '+') ||  (ch == '*') ||
                   (ch == '/') || (ch == '%') || (ch == '(') ||
                   (ch == ')') || (ch == '[') || (ch == ']') || (ch == ':') || (ch == '<') || (ch == '>') ||
                   (ch == '=')) {
            string opName;
            do {
                buffer.push(ch);
                if ((ch != ' ')&&(ch != '\n')) {
                    switch (state) {
                        case 0:
                            if (ch == '.') {
                                state = end_state;
                                opName = "period";
                            } else if (ch == ',') {
                                state = end_state;
                                opName = "comma";
                            } else if (ch == ';') {
                                state = end_state;
                                opName = "semicolon";
                            } else if (ch == ':') {
                                state = 1;
                                prev_case = state;
                                opName = "nothing";
                            } else if (ch == '+') {
                                state = end_state;
                                opName = "add";
                            }  else if (ch == '*') {
                                state = end_state;
                                opName = "multiply";
                            } else if (ch == '/') {
                                state = end_state;
                                opName = "divide";
                            } else if (ch == '%') {
                                state = end_state;
                                opName = "modulo";
                            } else if (ch == '(') {
                                state = end_state;
                                opName = "left-parenthesis";
                            } else if (ch == ')') {
                                state = end_state;
                                opName = "right-parenthesis";
                            } else if (ch == '[') {
                                state = end_state;
                                opName = "left-bracket";
                            } else if (ch == ']') {
                                state = end_state;
                                opName = "right-bracket";
                            } else if (ch == '<') {
                                state = 2;
                                prev_case = state;
                                opName = "less";
                            } else if (ch == '>') {
                                state = 2;
                                prev_case = state;
                                opName = "greater";
                            } else if (ch == '=') {
                                state = 4;
                                prev_case = state;
                                opName = "equal";
                            }
                            break;
                        case 1:
                            if (ch == '=') {
                                state = end_state;
                                opName = "assign";
                            } else state = dead_state;
                            break;
                        case 2:
                            if (ch == '>') {
                                state = end_state;
                                opName = "not-equal";
                            } else if (ch == '=') {
                                state = end_state;
                                opName = "less-or-equal";
                            } else state = dead_state;
                            break;
                        case 3:
                            if (ch == '<') {
                                state = end_state;
                                opName = "not-equal";
                            } else if (ch == '=') {
                                state = end_state;
                                opName = "greater-or-equal";
                            } else state = dead_state;
                            break;
                        default:
                            state = dead_state;
                            break;
                    }
                }
                //file.get(ch);
                if((state != dead_state)&&(state != end_state)){fin.get(ch);}
            } while ((ch != ' ') && (state != dead_state) && (state != end_state) && (ch != '\n') && !fin.eof());





            if((state == 5)||(opName == "nothing")){
                token tok(false, "Lexical error:", "", "operand malformed" , '\0');
                allTokens.addToVec(tok);
                ch = ' ';
            }



            else{
                while (!buffer.empty()) {
                    token_string.push_back((buffer.front()));
                    buffer.pop(); }
                token tok (true, "operator", opName, token_string, '\0');
                allTokens.addToVec(tok);
                ch = ' ';
            }
        }













            //If char isnt recognized, error.
        else if ((ch != ' ')&&(ch != '\n')&&(ch != '\0')){
            string lex(1, ch);
            token tok(false, "Illegal character:", "", lex , '\0');
            allTokens.addToVec(tok);
            while(ch != ' ') {
                fin.get(ch);
            }
        }













        //this is just to get the next char
        if ((ch == ' ')||(ch == '\n')||(ch == '\0')){
            fin.get(ch);
        }
    }



    //print all my tokens
    token tok(true, "nomore", "nomore", "nomore", '\0');
    allTokens.addToVec(tok);
    cout << "Would you like to see lexical analyzer output (all tokens)? [Y | N]" << endl << endl;
    char des;
    cin >> des;
    if (des == 'Y') {
        allTokens.printTokens();
        cout << "\n \n";
    }
    else if (des != 'N'){
        cout << "Invalid input, outputting lexical analyzer data..." << endl << endl;
        allTokens.printTokens();
        cout << "\n \n";
    }


    return allTokens;
}


bool parse(token_vec tok){
    token_vec tokens = std::move(tok);
    token curr = tokens.firstToken();
    cout << "WARNING OR ERROR MESSAGES:" << endl;
    Program(curr, tokens);
    cout << "\n \nSUCCESSFUL PARSING: NO CRITITCAL ERRORS. \n \n" << endl;
    cout << "Would you like to see the symbol table? Only variables defined before 'begin' are valid... [Y | N]" << endl << endl;
    char des;
    cin >> des;
    if (des == 'Y') {
        symbs.printSymbols();
        cout << "\n \n";
    }
    else if (des != 'N'){
        cout << "Invalid input, outputting lexical analyzer data..." << endl << endl;
        symbs.printSymbols();
        cout << "\n \n";
    }
    return true;
}


//function for determining if lexeme is a reserved word, uses the constant array
int isKeyword(const string &token_string) {
    for (int i = 0; i < 19; i++) {
        if (keywords[i] == (token_string)) {
            return i;
        }
    }
    return -1;
}


//removes the most recently added char in the buffer
queue<char> removeLast(queue<char> q) {
    char last = q.back();
    char current = '\0';
    bool flag = true;
    while (flag) {
        current = q.front();
        q.pop();
        if (current != last) {
            q.push(current);
        }else{flag = false;}
    }
    return q;
}


//function for removing an invalid char from an identifier
queue<char> removeMal(queue<char> q, char mal) {
    char rem = mal;
    char current = '\0';
    for(int i = 0; i <= (q.size()); i++) {
        current = q.front();
        q.pop();
        if ((current != rem) /*&& (!done)*/) {
            q.push(current);
        }
    }
    return q;
}


//function to determine of a char is an operand
bool isOp(char ch){
    return ((ch == '.') || (ch == ',') || (ch == ';') || (ch == '+') ||  (ch == '*') ||
        (ch == '/') || (ch == '%') || (ch == '(') ||
        (ch == ')') || (ch == '[') || (ch == ']') || (ch == ':') || (ch == '<') || (ch == '>') ||
        (ch == '=') || ( ch == '-'));
}


token Program(token tok, token_vec vec) {
    token curr = std::move(tok);
    if (curr.getTheToken() == "const") {
        curr = vec.nextToken(curr);
        if (curr.getTheToken() == "identifier") {
            string tempN = curr.getName();
            curr = vec.nextToken(curr);
            if (curr.getTheToken() == "equal") {
                curr = vec.nextToken(curr);
                if (curr.getTheToken() == "number") {
                    int tempI = curr.getVal();
                    curr = vec.nextToken(curr);
                    int * tempA;
                    tempA = &tempI;
                    symbol cons(tempN, tempA, tempI, true);
                    symbs.addToTable(cons);
                    curr = ProgramP(curr, vec);
                } else {
                    cout << "PARSING ERROR: EXPECTING NUMBER" << endl;
                    exit(-1);
                }
            } else {
                cout << "PARSING ERROR: EXPECTING EQUALS" << endl;
                exit(-1);
            }
        } else {
            cout << "PARSING ERROR: EXPECTING IDENTIFIER" << endl;
            exit(-1);
        }
    } else if (curr.getTheToken() == "var") {
        curr = vec.nextToken(curr);
        if (curr.getTheToken() == "identifier") {
            string CtempN = curr.getName();
            int CtempI = 0;
            int *CtempA = &CtempI;
            symbol val(CtempN, CtempA, CtempI, false);
            curr = vec.nextToken(curr);
            symbs.addToTable(val);
            curr = ProgramPPP(curr, vec);
        } else {
            cout << "PARSING ERROR: EXPECTING IDENTIFIER" << endl;
            exit(-1);
        }
    } else if (curr.getTheToken() == "begin") {
        curr = CS(curr, vec);
        curr = vec.nextToken(curr);
        if (curr.getTheToken() != "period") {
            cout << "PARSING ERROR: EXPECTING PERIOD." << endl;
            exit(-1);
        }
    } else {
        cout << "PARSING ERROR: EXPECTING CONST, VAR, OR BEGIN." << endl;
        exit(-1);
    }
    return curr;
}
token ProgramP(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "comma") {
        curr = vec.nextToken(curr);
        if (curr.getTheToken() == "identifier") {
            string tempN = curr.getName();
            curr = vec.nextToken(curr);
            if (curr.getTheToken() == "equal") {
                curr = vec.nextToken(curr);
                if (curr.getTheToken() == "number") {
                    int tempI = curr.getVal();
                    curr = vec.nextToken(curr);
                    int * tempA;
                    tempA = &tempI;
                    symbol cons(tempN, tempA, tempI, true);
                    symbs.addToTable(cons);
                    curr = ProgramP(curr, vec);
                } else {
                    cout << "PARSING ERROR: EXPECTING NUMBER" << endl;
                    exit(-1);
                }
            } else {
                cout << "PARSING ERROR: EXPECTING EQUALS" << endl;
                exit(-1);
            }
        } else {
            cout << "PARSING ERROR: EXPECTING IDENTIFIER" << endl;
            exit(-1);
        }
    }
    else if (curr.getTheToken() == "semicolon"){
        curr = vec.nextToken(curr);
        curr = ProgramPP(curr, vec);
    }else{
        cout << "PARSING ERROR: EXPECTING COMMA OR SEMICOLON." << endl;
        exit(-1);

    }
    return curr;
}
token ProgramPP(token tok, token_vec vec) {
    token curr = std::move(tok);
    if (curr.getTheToken() == "var") {
        curr = vec.nextToken(curr);
        if (curr.getTheToken() == "identifier") {
            string CtempN = curr.getName();
            int CtempI = 0;
            int *CtempA = &CtempI;
            symbol val(CtempN, CtempA, CtempI, false);
            symbs.addToTable(val);
            curr = vec.nextToken(curr);
            curr = ProgramPPP(curr, vec);
        } else {
            cout << "PARSING ERROR: EXPECTING IDENTIFIER" << endl;
            exit(-1);
        }
    } else if (curr.getTheToken() == "begin") {
        curr = CS(curr, vec);
        curr = vec.nextToken(curr);
        if (curr.getTheToken() != "period") {
            cout << "PARSING ERROR: EXPECTING PERIOD." << endl;
            exit(-1);
        }
    } else {
        cout << "PARSING ERROR: EXPECTING VAR OR BEGIN." << endl;
        exit(-1);
    }
    return curr;
}
token ProgramPPP(token tok, token_vec vec) {
    token curr = std::move(tok);
    if (curr.getTheToken() == "left-bracket") {
        curr = vec.nextToken(curr);
        curr = Const(curr, vec);
        curr = vec.nextToken(curr);
        if (curr.getTheToken() == "right-bracket") {
            curr = vec.nextToken(curr);
            curr = ProgramPPPP(curr, vec);
        } else {
            cout << "PARSING ERROR: EXPECTING RIGHT-BRACKET" << endl;
            exit(-1);
        }
    } else if (curr.getTheToken() == "comma") {
        curr = ProgramPPPP(curr, vec);
    } else if (curr.getTheToken() == "semicolon") {
        curr = ProgramPPPP(curr, vec);
    } else {
        cout << "PARSING ERROR: EXPECTING LEFT-BRACKET, COMMA, OE SEMICOLON." << endl;
        exit(-1);
    }
    return curr;
}
token ProgramPPPP(token tok, token_vec vec) {
    token curr = std::move(tok);
    if (curr.getTheToken() == "comma") {
        curr = vec.nextToken(curr);
        if (curr.getTheToken() == "identifier") {
            string CtempN = curr.getName();
            int CtempI = 0;
            int *CtempA = &CtempI;
            symbol val(CtempN, CtempA, CtempI, false);
            symbs.addToTable(val);
            curr = vec.nextToken(curr);
            curr = ProgramPPP(curr, vec);
        } else {
            cout << "PARSING ERROR: EXPECTING IDENTIFIER" << endl;
            exit(-1);
        }
    } else if (curr.getTheToken() == "semicolon") {
        curr = vec.nextToken(curr);
        curr = CS(curr, vec);
        curr = vec.nextToken(curr);
        if (curr.getTheToken() != "period") {
            cout << "PARSING ERROR: EXPECTING PERIOD." << endl;
            exit(-1);
        }
    }
    return curr;
}
token CS(token tok, token_vec vec) {
    token curr = std::move(tok);
    if (curr.getTheToken() == "begin") {
        curr = vec.nextToken(curr);
        curr = stmt(curr, vec);
        curr = CSP(curr, vec);

        if (curr.getTheToken() != "end") {
            cout << "PARSING ERROR, EXPECTING END." << endl;
            exit(-1);
        }
    }else{
        cout << "PARSING ERROR, EXPECTING BEGIN." << endl;
        exit(-1);
    }
    return curr;
}
token CSP(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "semicolon") {
        curr = vec.nextToken(curr);
        curr = stmt(curr, vec);
        curr = CSP(curr, vec);
    }
    else if(curr.getTheToken() != "end"){
        cout << "PARSING ERROR: EXPECTING SEMICOLON OR END." << endl;
        exit(-1);
    }
    return curr;
}
token stmt(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "identifier") {
        string temp1 = curr.getName();
        curr = lVal(curr, vec);
        if (curr.getTheToken() == "assign") {
            curr = vec.nextToken(curr);
            curr = exp(curr, vec);
            int i = curr.getVal();
            if (symbs.findSymbol(temp1).getCons() != true) {
                symbs.changeVal(temp1, i);
            }else {
                cout << "WARNING: ATTEMPTED ALTERATION OF A CONSTANT: " << temp1 << endl;
            }

        }else{
            cout << "PARSING ERROR: EXPECTING ASSIGNMENT" << endl;
            exit(-1);
        }
    }
    else if (curr.getTheToken() == "begin") {
        curr = CS(curr, vec);
    }
    else if (curr.getTheToken() == "if") {
        curr = vec.nextToken(curr);
        curr = BE(curr, vec);
        curr = vec.nextToken(curr);
        if (curr.getTheToken() == "then"){
            curr = vec.nextToken(curr);
            curr = stmt(curr, vec);
            curr = stmtP(curr, vec);
        } else {
            cout << "PARSING ERROR: EXPECTING THEN." << endl;
            exit(-1);
        }
    }
    else if (curr.getTheToken() == "while") {
        curr = vec.nextToken(curr);
        curr = BE(curr, vec);
        curr = vec.nextToken(curr);
        if (curr.getTheToken() == "do"){
            curr = vec.nextToken(curr);
            curr = stmt(curr, vec);
        } else {
            cout << "PARSING ERROR: EXPECTING DO." << endl;
            exit(-1);
        }
    }
    else if (curr.getTheToken() == "for") {
        curr = vec.nextToken(curr);
        if (curr.getTheToken() == "identifier"){
            curr = vec.nextToken(curr);
            if (curr.getTheToken() == "assignment"){
                curr = vec.nextToken(curr);
                curr = exp(curr, vec);
                curr = vec.nextToken(curr);
                if (curr.getTheToken() == "to"){
                    curr = vec.nextToken(curr);
                    curr = exp(curr, vec);
                    curr = vec.nextToken(curr);
                    if (curr.getTheToken() == "do") {
                        curr = vec.nextToken(curr);
                        curr = stmt(curr, vec);
                    }else {
                        cout << "PARSING ERROR: EXPECTING DO." << endl;
                        exit(-1);
                    }
                }else {
                    cout << "PARSING ERROR: EXPECTING TO." << endl;
                    exit(-1);
                }
            }else {
                cout << "PARSING ERROR: EXPECTING ASSIGNMENT." << endl;
                exit(-1);
            }
        }else {
            cout << "PARSING ERROR: EXPECTING IDENTIFIER." << endl;
            exit(-1);
        }
    }
    else if (curr.getTheToken() == "readchar") {
        curr = ioS(curr, vec);
    }
    else if (curr.getTheToken() == "writechar") {
        curr = ioS(curr, vec);
    }
    else if (curr.getTheToken() == "read") {
        curr = ioS(curr, vec);
    }
    else if (curr.getTheToken() == "write") {
        curr = ioS(curr, vec);
    }
    else{
        cout << "PARSING ERROR IN STATEMENT." << endl;
        exit(-1);
    }
    return curr;
}
token stmtP(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "semicolon") {
    }
    else if (curr.getTheToken() == "end") {
    }
    else if (curr.getTheToken() == "else") {
        curr = vec.nextToken(curr);
        curr = stmt(curr, vec);
    }else {
        cout << "PARSING ERROR IN STATEMENT." << endl;
        exit(-1);
    }
    return curr;
}
token ioS(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "readchar") {
        curr = vec.nextToken(curr);
        curr = lVal(curr, vec);
    }
    else if (curr.getTheToken() == "writechar") {
        curr = vec.nextToken(curr);
        curr = val(curr, vec);
    }
    else if (curr.getTheToken() == "read") {
        curr = vec.nextToken(curr);
        curr = lVal(curr, vec);
        curr = vec.nextToken(curr);
        if (curr.getTheToken() == "comma") {
            curr = vec.nextToken(curr);
            curr = lVal(curr, vec);
        }else {
            cout << "PARSING ERROR IN IO STATEMENT." << endl;
            exit(-1);
        }
    }
    else if (curr.getTheToken() == "write") {
        curr = vec.nextToken(curr);
        curr = val(curr, vec);
    }else {
        cout << "PARSING ERROR IN IO STATEMENT." << endl;
        exit(-1);
    }
    return curr;
}
token lVal(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "identifier") {
        curr = vec.nextToken(curr);
        curr = lValP(curr, vec);
    }else {
        cout << "PARSING ERROR IN LVALUE." << endl;
        exit(-1);
    }
    return curr;
}
token lValP(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "left-bracket") {
        curr = vec.nextToken(curr);
        curr = exp(curr, vec);
        curr = vec.nextToken(curr);
        if (curr.getTheToken() == "right-bracket") {
        } else{
            cout << "PARSING ERROR IN LVALUE." << endl;
            exit(-1);
        }
    }
    else if (curr.getTheToken() == "comma") {
    }
    else if (curr.getTheToken() == "semicolon") {
    }
    else if (curr.getTheToken() == "end") {
    }
    else if (curr.getTheToken() == "assign") {
    }
    else if (curr.getTheToken() == "else") {
    }
    else{
        cout << "PARSING ERROR IN LVALUE." << endl;
        exit(-1);
    }
    return curr;

}
token val(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "identifier") {
        curr = vec.nextToken(curr);
        curr = valP(curr, vec);
    }
    else if (curr.getTheToken() == "number"){
        curr = vec.nextToken(curr);
    }else{
        cout << "PARSING ERROR IN VALUE." << endl;
        exit(-1);
    }
    return curr;
}
token valP(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "left-bracket") {
        curr = vec.nextToken(curr);
        curr = exp(curr, vec);
        curr = vec.nextToken(curr);
        if (curr.getTheToken() == "right-bracket") {
        } else{
            cout << "PARSING ERROR IN VALUE." << endl;
            exit(-1);
        }
    }
    else if (curr.getTheToken() == "equal") {
    }
    else if (curr.getTheToken() == "semicolon") {
    }
    else if (curr.getTheToken() == "end") {
    }
    else if (curr.getTheToken() == "then") {
    }
    else if (curr.getTheToken() == "else") {
    }
    else if (curr.getTheToken() == "do") {
    }
    else if (curr.getTheToken() == "or") {
    }
    else if (curr.getTheToken() == "and") {
    }
    else if (curr.getTheToken() == "right-parenthesis") {
    }
    else if (curr.getTheToken() == "not-equal") {
    }
    else if (curr.getTheToken() == "less-or-equal") {
    }
    else if (curr.getTheToken() == "less") {
    }
    else if (curr.getTheToken() == "greater") {
    }
    else if (curr.getTheToken() == "greater-or-equal") {
    }
    else{
        cout << "PARSING ERROR IN VALUE." << endl;
        exit(-1);
    }
    return curr;
}
token BE(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "identifier") {
        curr = BT(curr, vec);
        curr = BEP(curr, vec);
    }
    else if (curr.getTheToken() == "number") {
        curr = BT(curr, vec);
        curr = BEP(curr, vec);
    }
    else if (curr.getTheToken() == "left-parenthesis") {
        curr = BT(curr, vec);
        curr = BEP(curr, vec);
    }
    else if (curr.getTheToken() == "not") {
        curr = BT(curr, vec);
        curr = BEP(curr, vec);
    }
    else if (curr.getTheToken() == "odd") {
        curr = BT(curr, vec);
        curr = BEP(curr, vec);
    }
    else{
        cout << "PARSING ERROR IN BOOLEXP." << endl;
        exit(-1);
    }
    return curr;
}
token BEP(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "then") {
    }
    else if (curr.getTheToken() == "do") {
    }
    else if (curr.getTheToken() == "or") {
        curr = vec.nextToken(curr);
        curr = BT(curr, vec);
        curr = BEP(curr, vec);
    }
    else if (curr.getTheToken() == "right-parenthesis") {
    }
    else{
        cout << "PARSING ERROR IN BOOLEXP." << endl;
        exit(-1);
    }
    return curr;
}
token BT(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "identifier") {
        curr = BF(curr, vec);
        curr = BTP(curr, vec);
    }
    else if (curr.getTheToken() == "number") {
        curr = BF(curr, vec);
        curr = BTP(curr, vec);
    }
    else if (curr.getTheToken() == "left-parenthesis") {
        curr = BF(curr, vec);
        curr = BTP(curr, vec);
    }
    else if (curr.getTheToken() == "not") {
        curr = BF(curr, vec);
        curr = BTP(curr, vec);
    }
    else if (curr.getTheToken() == "odd") {
        curr = BF(curr, vec);
        curr = BTP(curr, vec);
    }
    else{
        cout << "PARSING ERROR IN BOOLTERM." << endl;
        exit(-1);
    }
    return curr;
}
token BTP(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "then") {
    }
    else if (curr.getTheToken() == "do") {
    }
    else if (curr.getTheToken() == "or") {
    }
    else if (curr.getTheToken() == "and") {
        curr = vec.nextToken(curr);
        curr = BF(curr, vec);
        curr = BTP(curr, vec);
    }
    else if (curr.getTheToken() == "right-parenthesis") {
    }
    else{
        cout << "PARSING ERROR IN BOOLTERM." << endl;
        exit(-1);
    }
    return curr;
}
token BF(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "identifier") {
        curr = cond(curr, vec);
    }
    else if (curr.getTheToken() == "number") {
        curr = cond(curr, vec);
    }
    else if (curr.getTheToken() == "left-parenthesis") {
        curr = vec.nextToken(curr);
        curr = BE(curr, vec);

        if (curr.getTheToken() == "right-parenthesis"){
        }else{
            cout << "PARSING ERROR IN BOOLFACTOR." << endl;
            exit(-1);
        }
    }
    else if (curr.getTheToken() == "not") {
        curr = vec.nextToken(curr);
        curr = BF(curr, vec);
    }
    else if (curr.getTheToken() == "odd") {
        curr = cond(curr, vec);
    }
    else{
        cout << "PARSING ERROR IN BOOLFACTOR." << endl;
        exit(-1);
    }
    return curr;
}
token cond(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "identifier") {
        curr = val(curr, vec);
        curr = condP(curr, vec);
    }
    else if (curr.getTheToken() == "number") {
        curr = val(curr, vec);
        curr = condP(curr, vec);
    }
    else if (curr.getTheToken() == "odd") {
        curr = vec.nextToken(curr);
        curr = val(curr, vec);
    }
    else{
        cout << "PARSING ERROR IN CONDITION." << endl;
        exit(-1);
    }
    return curr;
}
token condP(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "equal") {
        curr = vec.nextToken(curr);
        curr = val(curr, vec);
    }
    else if (curr.getTheToken() == "not-equal") {
        curr = vec.nextToken(curr);
        curr = val(curr, vec);
    }else if (curr.getTheToken() == "less-or-equal") {
        curr = vec.nextToken(curr);
        curr = val(curr, vec);
    }else if (curr.getTheToken() == "less") {
        curr = vec.nextToken(curr);
        curr = val(curr, vec);
    }else if (curr.getTheToken() == "greater") {
        curr = vec.nextToken(curr);
        curr = val(curr, vec);
    }else if (curr.getTheToken() == "greater-or-equal") {
        curr = vec.nextToken(curr);
        curr = val(curr, vec);
    }else{
        cout << "PARSING ERROR IN CONDITION." << endl;
        exit(-1);
    }
    return curr;
}
token exp(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "identifier") {
        curr = term(curr, vec);
        curr = expP(curr, vec);
    }
    else if (curr.getTheToken() == "number") {
        curr = term(curr, vec);
        curr = expP(curr, vec);
    }
    else if (curr.getTheToken() == "left-parenthesis") {
        curr = term(curr, vec);
        curr = expP(curr, vec);
    }
    else{
        cout << "PARSING ERROR IN EXPRESSION." << endl;
        exit(-1);
    }
    return curr;
}
token expP(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "right-bracket") {
        if ((vec.prevToken(vec.prevToken(curr)).getName() != "*")&&(vec.prevToken(vec.prevToken(curr)).getName() != "/")&&(vec.prevToken(vec.prevToken(curr)).getName() != "%")) {
            if (vec.prevToken(curr).getTheToken() == "number") { curr.setVal(vec.prevToken(curr).getVal()); }
            else { curr.setVal(symbs.findVal(vec.prevToken(curr).getName())); }
        }
    }else if (curr.getTheToken() == "semicolon") {
        if ((vec.prevToken(vec.prevToken(curr)).getName() != "*")&&(vec.prevToken(vec.prevToken(curr)).getName() != "/")&&(vec.prevToken(vec.prevToken(curr)).getName() != "%")) {
            if (vec.prevToken(curr).getTheToken() == "number") { curr.setVal(vec.prevToken(curr).getVal()); }
            else { curr.setVal(symbs.findVal(vec.prevToken(curr).getName())); }
        }
    }else if (curr.getTheToken() == "end") {
        if ((vec.prevToken(vec.prevToken(curr)).getName() != "*")&&(vec.prevToken(vec.prevToken(curr)).getName() != "/")&&(vec.prevToken(vec.prevToken(curr)).getName() != "%")) {
            if (vec.prevToken(curr).getTheToken() == "number") { curr.setVal(vec.prevToken(curr).getVal()); }
            else { curr.setVal(symbs.findVal(vec.prevToken(curr).getName())); }
        }
    }else if (curr.getTheToken() == "do") {
        if ((vec.prevToken(vec.prevToken(curr)).getName() != "*")&&(vec.prevToken(vec.prevToken(curr)).getName() != "/")&&(vec.prevToken(vec.prevToken(curr)).getName() != "%")) {
            if (vec.prevToken(curr).getTheToken() == "number") { curr.setVal(vec.prevToken(curr).getVal()); }
            else { curr.setVal(symbs.findVal(vec.prevToken(curr).getName())); }
        }
    }else if (curr.getTheToken() == "to") {
        if ((vec.prevToken(vec.prevToken(curr)).getName() != "*")&&(vec.prevToken(vec.prevToken(curr)).getName() != "/")&&(vec.prevToken(vec.prevToken(curr)).getName() != "%")) {
            if (vec.prevToken(curr).getTheToken() == "number") { curr.setVal(vec.prevToken(curr).getVal()); }
            else { curr.setVal(symbs.findVal(vec.prevToken(curr).getName())); }
        }
    }else if (curr.getTheToken() == "else") {
        if ((vec.prevToken(vec.prevToken(curr)).getName() != "*")&&(vec.prevToken(vec.prevToken(curr)).getName() != "/")&&(vec.prevToken(vec.prevToken(curr)).getName() != "%")) {
            if (vec.prevToken(curr).getTheToken() == "number") { curr.setVal(vec.prevToken(curr).getVal()); }
            else { curr.setVal(symbs.findVal(vec.prevToken(curr).getName())); }
        }
    }else if (curr.getTheToken() == "right-parenthesis") {
        if ((vec.prevToken(vec.prevToken(curr)).getName() != "*")&&(vec.prevToken(vec.prevToken(curr)).getName() != "/")&&(vec.prevToken(vec.prevToken(curr)).getName() != "%")) {
            if (vec.prevToken(curr).getTheToken() == "number") { curr.setVal(vec.prevToken(curr).getVal()); }
            else { curr.setVal(symbs.findVal(vec.prevToken(curr).getName())); }
        }
    }else if (curr.getTheToken() == "add") {
        int tempI;
        if ((vec.prevToken(vec.prevToken(curr)).getName() != "*")&&(vec.prevToken(vec.prevToken(curr)).getName() != "/")&&(vec.prevToken(vec.prevToken(curr)).getName() != "%")){
            if(vec.prevToken(curr).getTheToken() == "number"){tempI = vec.prevToken(curr).getVal();}
            else{tempI = symbs.findVal(vec.prevToken(curr).getName());}
        }else{
            tempI = curr.getVal();
        }
        curr = vec.nextToken(curr);
        curr = term(curr, vec);
        int tempJ;
        if(vec.prevToken(curr).getTheToken() == "number"){tempJ = vec.prevToken(curr).getVal();}
        else{tempJ = symbs.findVal(vec.prevToken(curr).getName());}
        curr = expP(curr, vec);
        if (curr.getVal() != tempJ){
            tempJ = curr.getVal();
        }
        int tempK = tempI + tempJ;
        curr.setVal(tempK);
    }else if (curr.getTheToken() == "subtract") {
        int tempI;
        if ((vec.prevToken(vec.prevToken(curr)).getName() != "*")&&(vec.prevToken(vec.prevToken(curr)).getName() != "/")&&(vec.prevToken(vec.prevToken(curr)).getName() != "%")){
            if(vec.prevToken(curr).getTheToken() == "number"){tempI = vec.prevToken(curr).getVal();}
            else{tempI = symbs.findVal(vec.prevToken(curr).getName());}
        }else{
            tempI = curr.getVal();
        }
        curr = vec.nextToken(curr);
        curr = term(curr, vec);
        int tempJ;
        if(vec.prevToken(curr).getTheToken() == "number"){tempJ = vec.prevToken(curr).getVal();}
        else{tempJ = symbs.findVal(vec.prevToken(curr).getName());}
        curr = expP(curr, vec);
        if (curr.getVal() != tempJ){
            tempJ = curr.getVal();
        }
        int tempK = tempI - tempJ;
        curr.setVal(tempK);
    }else{
        cout << "PARSING ERROR IN EXPRESSION." << endl;
        exit(-1);
    }
    return curr;
}
token term(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "identifier") {
        curr = fact(curr, vec);
        curr = termP(curr, vec);
    }
    else if (curr.getTheToken() == "number") {
        curr = fact(curr, vec);
        curr = termP(curr, vec);
    }
    else if (curr.getTheToken() == "left-parenthesis") {
        curr = fact(curr, vec);
        curr = termP(curr, vec);
    }
    else{
        cout << "PARSING ERROR IN TERM." << endl;
        exit(-1);
    }
    return curr;
}
token termP(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "right-bracket") {
        curr.setVal(vec.prevToken(curr).getVal());
    }
    else if (curr.getTheToken() == "semicolon") {
        if(vec.prevToken(curr).getTheToken() == "number"){curr.setVal(vec.prevToken(curr).getVal());}
        else{curr.setVal(symbs.findVal(vec.prevToken(curr).getName()));}
    }else if (curr.getTheToken() == "end") {
        if(vec.prevToken(curr).getTheToken() == "number"){curr.setVal(vec.prevToken(curr).getVal());}
        else{curr.setVal(symbs.findVal(vec.prevToken(curr).getName()));}
    }else if (curr.getTheToken() == "do") {
        if(vec.prevToken(curr).getTheToken() == "number"){curr.setVal(vec.prevToken(curr).getVal());}
        else{curr.setVal(symbs.findVal(vec.prevToken(curr).getName()));}
    }else if (curr.getTheToken() == "to") {
        if(vec.prevToken(curr).getTheToken() == "number"){curr.setVal(vec.prevToken(curr).getVal());}
        else{curr.setVal(symbs.findVal(vec.prevToken(curr).getName()));}
    }else if (curr.getTheToken() == "else") {
        if(vec.prevToken(curr).getTheToken() == "number"){curr.setVal(vec.prevToken(curr).getVal());}
        else{curr.setVal(symbs.findVal(vec.prevToken(curr).getName()));}
    } else if (curr.getTheToken() == "right-parenthesis") {
        if(vec.prevToken(curr).getTheToken() == "number"){curr.setVal(vec.prevToken(curr).getVal());}
        else{curr.setVal(symbs.findVal(vec.prevToken(curr).getName()));}
    }else if (curr.getTheToken() == "add") {
        if(vec.prevToken(curr).getTheToken() == "number"){curr.setVal(vec.prevToken(curr).getVal());}
        else{curr.setVal(symbs.findVal(vec.prevToken(curr).getName()));}
    }else if (curr.getTheToken() == "subtract") {
        if(vec.prevToken(curr).getTheToken() == "number"){curr.setVal(vec.prevToken(curr).getVal());}
        else{curr.setVal(symbs.findVal(vec.prevToken(curr).getName()));}
    }else if (curr.getTheToken() == "multiply") {
        int tempI;
        if(vec.prevToken(curr).getTheToken() == "number"){tempI = vec.prevToken(curr).getVal();}
        else{tempI = symbs.findVal(vec.prevToken(curr).getName());}
        curr = vec.nextToken(curr);
        curr = fact(curr, vec);
        int tempJ;
        if(vec.prevToken(curr).getTheToken() == "number"){tempJ = vec.prevToken(curr).getVal();}
        else{tempJ = symbs.findVal(vec.prevToken(curr).getName());}
        curr = termP(curr, vec);
        if (curr.getVal() != tempJ){
            tempJ = curr.getVal();
        }
        int tempK = tempI * tempJ;
        curr.setVal(tempK);
    }else if (curr.getTheToken() == "divide") {
        int tempI;
        if(vec.prevToken(curr).getTheToken() == "number"){tempI = vec.prevToken(curr).getVal();}
        else{tempI = symbs.findVal(vec.prevToken(curr).getName());}
        curr = vec.nextToken(curr);
        curr = fact(curr, vec);
        int tempJ;
        if(vec.prevToken(curr).getTheToken() == "number"){tempJ = vec.prevToken(curr).getVal();}
        else{tempJ = symbs.findVal(vec.prevToken(curr).getName());}
        curr = termP(curr, vec);
        if (curr.getVal() != tempJ){
            tempJ = curr.getVal();
        }
        int tempK = tempI / tempJ;
        curr.setVal(tempK);
    }else if (curr.getTheToken() == "modulo") {
        int tempI;
        if(vec.prevToken(curr).getTheToken() == "number"){tempI = vec.prevToken(curr).getVal();}
        else{tempI = symbs.findVal(vec.prevToken(curr).getName());}
        curr = vec.nextToken(curr);
        curr = fact(curr, vec);
        int tempJ;
        if(vec.prevToken(curr).getTheToken() == "number"){tempJ = vec.prevToken(curr).getVal();}
        else{tempJ = symbs.findVal(vec.prevToken(curr).getName());}
        curr = termP(curr, vec);
        if (curr.getVal() != tempJ){
            tempJ = curr.getVal();
        }
        int tempK = tempI % tempJ;
        curr.setVal(tempK);
    }else{
        cout << "PARSING ERROR IN TERM." << endl;
        exit(-1);
    }
    return curr;
}
token fact(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "identifier") {
        curr = vec.nextToken(curr);
        curr = factP(curr, vec);
    }
    else if (curr.getTheToken() == "number") {
        curr = vec.nextToken(curr);
    }
    else if (curr.getTheToken() == "left-parenthesis") {
        curr = vec.nextToken(curr);
        curr = exp(curr, vec);
        curr = vec.nextToken(curr);
        if (curr.getTheToken() == "right-parenthesis") {
        } else {
            cout << "PARSING ERROR IN FACTOR." << endl;
            exit(-1);
        }
    }
    else{
        cout << "PARSING ERROR IN FACTOR." << endl;
        exit(-1);
    }
    return curr;
}
token factP(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() == "left-bracket") {
        curr = vec.nextToken(curr);
        curr = exp(curr, vec);
        curr = vec.nextToken(curr);
        if (curr.getTheToken() == "right-bracket") {
        } else {
            cout << "PARSING ERROR IN FACTOR." << endl;
            exit(-1);
        }
    }else if (curr.getTheToken() == "semicolon") {
    }else if (curr.getTheToken() == "end") {
    }else if (curr.getTheToken() == "do") {
    }else if (curr.getTheToken() == "to") {
    }else if (curr.getTheToken() == "else") {
    }else if (curr.getTheToken() == "right-parenthesis") {
    }else if (curr.getTheToken() == "add") {
    }else if (curr.getTheToken() == "subtract") {
    }else if (curr.getTheToken() == "multiply") {
    }else if (curr.getTheToken() == "divide") {
    }else if (curr.getTheToken() == "modulo") {
    }else{
        cout << "PARSING ERROR IN FACTOR." << endl;
        exit(-1);
    }
    return curr;
}
token Const(token tok, token_vec vec){
    token curr = std::move(tok);
    if (curr.getTheToken() != "number" || curr.getTheToken() != "identifier"){
        cout << "INVALID CONSTANT INPUT" << endl;
        exit(-1);
    }
}





















