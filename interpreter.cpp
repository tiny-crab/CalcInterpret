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
bool stmt();
double expr();
double term();
double termTail(double leftSide);
double fctr();
double fctrTail(double leftSide);
//void match();

token currentToken;
map<string, double> symbolTable;

//function to initiate the parsing
void calcParse()
{
  prgm();
}

//function to check token and see if it matches
/*bool match(int expectedToken)
{
  //operator overloaded to check for token type
  if(currentToken.type() == expectedToken)
  {
    //if the current token is not an end of file token, keep on trucking
    if(currentToken.type() != endOfFileSym)
    {
      currentToken = calcLex();
    }
    return true;
  }
  //if its not the expected token, shut down the interpreter!
  else
  {
    cout << "Token " << expectedToken << " expected, but instead found: " << currentToken.data() << endl;
    return false;
  }
}*/

void prgm()
{
  stmtList();
  currentToken = calcLex();
  if(currentToken == endOfFileSym)
  {
    cout << "We made it to the end of the program!" << endl;
  }
}

void stmtList()
{
  if(stmt())
  {
    stmtList();
  }
}

bool stmt()
{
  currentToken = calcLex();
  //if the first token in this statement is an identifier...
  if(currentToken == identifier)
  {
    //token to save the identifier name for the symbol table
    token symbolToken = currentToken;
    //checking for the assignment operator
    currentToken = calcLex();
    if(currentToken == assignSym)
    {
      //calculate the expression on the opposite side of the identifier
      double calculation = expr();
      //this line sets the calculation to an existing key
      //or a nonexistent key is initialized with value = "calculation"
      symbolTable[symbolToken.data()] = calculation;
      return true;
    }
    else
    {
      //in this language, an identifier not followed by an assignment op doesn't make sense
      cout << "The token isn't an assignment symbol. It's a: " << currentToken.typeToString() << endl;
      return false;
    }
  }
  //if the first token in this statement is a read symbol...
  if(currentToken == readSym)
  {
    //get the next token in the statement
    currentToken = calcLex();
    //if it's an identifier (which it should be)
    if(currentToken == identifier)
    {
      //setting up a var so that the user can save their input
      double varValue;
      cout << "Please enter a value for variable " << currentToken.data() << ": ";
      cin >> varValue;

      //this line sets the calculation to an existing key
      //or a nonexistent key is initialized with value = "calculation"
      symbolTable[currentToken.data()] = varValue;
      return true;
    }
    else
    {
      cout << "I found a read symbol that isn't followed by an identifier" << endl;
      return false;
    }
  }
  if(currentToken.type() == writeSym)
  {
    cout << "found a write symbol" << endl;
    currentToken = calcLex();
    if(currentToken == identifier)
    {
      //if the symbol table has the identifier token as a key
      if(symbolTable.count(currentToken.data()))
      {
        cout << currentToken.data() << " is set as: " << symbolTable[currentToken.data()] << endl;
      }
      else
      {
        cout << "You didn't declare " << currentToken.data() << " so I'm initializing it to 1." << endl;
        //setting a new variable up in the symbol table
        symbolTable[currentToken.data()] = 1;
        cout << currentToken.data() << " is set as: " << symbolTable[currentToken.data()] << endl;
      }
      return true;
    }
    else
    {
      return false;
    }
  }
  return false;
}

double expr()
{
  cout << "Tried to calculate something" << endl;
  currentToken = calcLex();
  double leftValue = term();
  double fullValue = termTail(leftValue);
  return fullValue;
}

double term()
{
  currentToken = calcLex();
  double leftValue = fctr();
  double fullValue = fctrTail(leftValue);
  return fullValue;
}

double termTail(double leftSide)
{
  currentToken = calcLex();
  if(currentToken == addOp)
  {
    double firstValue = term();
    double fullValue = termTail(leftSide + firstValue);
    return fullValue;
  }
  if(currentToken == subOp)
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
  currentToken = calcLex();
  if(currentToken == leftParen)
  {
    double expressionVal = expr();
    currentToken = calcLex();
    if(currentToken == rightParen)
    {
      return expressionVal;
    }
    else
    {
      cout << "You forgot a parenthesis so I'm crashing now." << endl;
      return 1;
    }
  }
  else if(currentToken == identifier)
  {
    if(symbolTable.count(currentToken.data()))
    {
      return symbolTable[currentToken.data()];
    }
    else
    {
      return 1;
    }
  }
  else if(currentToken == numConst)
  {
    return stod(currentToken.data());
  }
  else
  {
    return 1;
  }
}

double fctrTail(double leftSide)
{
  currentToken = calcLex();
  if(currentToken == multOp)
  {
    double firstValue = fctr();
    double fullValue = fctrTail(leftSide * firstValue);
    return fullValue;
  }
  if(currentToken == divOp)
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
