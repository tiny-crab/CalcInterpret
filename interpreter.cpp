#include "calcLex.h"

//CalcLex Grammar!
// prgm -> stmtList
//
// stmtList -> stmt stmtList
//          -> nothing
//
// stmt -> identifier := expr
//      -> read identifier
//      -> write expr
//
// expr -> term termTail
//
// termTail -> + term termTail
//          -> - term termTail
//          -> nothing
//
// term -> fctr fctrTail
//
// fctrTail -> * fctr fctrTail
//          -> / fctr fctrTail
//          -> nothing
//
// fctr -> ( expr )
//      -> identifier
//      -> numConst

//function prototyping
void prgm();
void stmtList();
void stmt();
void expr();
void term();
void termTail();
void fctr();
void fctrTail();
void match();

token curToken;
map<string, double> symbolTable;

//function to initiate the parsing
void calcParse()
{
  curToken = calcLex();
  prgm();
}

//function to check token and see if it matches
bool match(int expectedToken)
{
  //operator overloaded to check for token type
  if(curToken.type() == expectedToken)
  {
    //if the current token is not an end of file token, keep on trucking
    if(curToken.type() != endOfFileSym)
    {
      curToken = calcLex();
    }
    return true;
  }
  //if its not the expected token, shut down the interpreter!
  else
  {
    cout << "Token " << expectedToken << " expected, but instead found: " << curToken.data() << endl;
    return false;
  }
}

void prgm()
{
  stmtList();
  match(endOfFileSym);
}

void stmtList()
{
  cout << "Found a statement list." << endl;
  stmt();
  stmtList();
}

void stmt()
{
  cout << "Found a statement" << endl;
  if(curToken == identifier)
  {
    //token to save the identifier name for the table
    token symbolToken = curToken;
    //getting the assignment operator out of the way for curToken
    token nextToken = calcLex();
    if(nextToken == assignSym)
    {
      double calculation = expr();
      //this line sets the calculation to an existing key
      //or a nonexistent key is initialized with value = "calculation"
      symbolTable[symbolToken.data()] = calculation;
    }
    else
    {
      return;
    }
  }
  if(curToken == readSym)
  {
    token nextToken = calcLex();
    if(nextToken == identifier)
    {
      double varValue;
      cout << "Please enter a value for variable " << nextToken.data() << ": ";
      //this next line might be syntactically incorrect
      cin >> varValue;

      symbolTable[nextToken.data()] = varValue;
    }
    else
    {
      return;
    }
  }
  if(curToken.type() == writeSym)
  {
    token nextToken = calcLex();
    if(nextToken == identifier)
    {
      //if the symbol table has the identifier token as a key
      if(symbolTable.count(nextToken.data()))
      {
        cout << nextToken.data() << " is set as: " << symbolTable[nextToken.data()] << endl;
      }
      else
      {
        cout << "You didn't declare " << nextToken.data() << " so initializing it to 1." << endl;
        //setting a new variable up in the symbol table
        symbolTable[nextToken.data()] = 1;
        cout << nextToken.data() << " is set as: " << symbolTable[nextToken.data()] << endl;
      }
    }
    else
    {
      return;
    }
  }
}

double expr()
{
  curToken = calcLex();
  double leftValue = term();
  double fullValue = termTail(leftValue);
  return fullValue;
}

double term()
{
  curToken = calcLex();
  double leftValue = fctr();
  double fullValue = fctrTail(leftValue);
  return fullValue;
}

double termTail(double leftSide)
{
  curToken = calcLex();
  if(curToken == addOp)
  {
    double firstValue = term();
    double fullValue = termTail(leftSide + firstValue);
    return fullValue;
  }
  if(curToken == subOp)
  {
    double firstValue = term();
    double fullValue = termTail(leftSide - firstValue);
    return fullValue;
  }
  else
  {
    //there is no tail!
    return leftSide;
  }
}

double fctr()
{
  curToken = calcLex();
  if(curToken == leftParen)
  {
    double expressionVal = expr();
    curToken = calcLex();
    if(curToken == rightParen)
    {
      return expressionVal;
    }
    else
    {
      cout << "You forgot a parenthesis so I'm crashing now." << endl;
    }
  }
  else if(curToken == identifier)
  {
    if(symbolTable.count(curToken.data()))
    {
      return symbolTable[curToken.data()]
    }
    else
    {
      return 1;
    }
  }
  else if(curToken == numConst)
  {
    return curToken.data();
  }
  else
  {
    return 1;
  }
}

double fctrTail(double leftSide)
{
  curToken = calcLex();
  if(curToken == multOp)
  {
    double firstValue = fctr();
    double fullValue = fctrTail(leftSide * firstValue);
    return fullValue;
  }
  if(curToken == divOp)
  {
    double firstValue = fctr();
    double fullValue = fctrTail(leftSide / firstValue);
    return fullValue;
  }
  else
  {
    //there is no tail!
    return leftSide;
  }
}
