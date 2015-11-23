#ifndef calcLex_h
#define calcLex_h

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;

class token
{
  private:
    int tokenType;
    string tokenData;
  public:
    void appendChar(char currentChar);
    void clearData();
    int type();
    void type(int);
    string typeToString();
    string data();
    void data(string);
    bool operator== (token);
};

enum tokenTypes {
    endOfFileSym = 0, assignSym, readSym, writeSym, identifier,
    addOp, subOp, multOp, divOp, leftParen, rightParen, numConst, unknownError,
    numConstError};

bool calcLexOpen(const char filename[]);
token calcLex();
bool match(token expectedToken);
int calcParse();
bool followingChars(string target, char currentChar, token curToken);

#endif /* calcLex_h */
