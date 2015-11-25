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

//global token var to hold things properly
token currentToken;
//map that holds the dynamically allocated variables
map<string, double> symbolTable;

//function to initiate the parsing
void calcParse()
{
  //start program off by getting the first token
  currentToken = calcLex();
  prgm();
}

//prgm -> stmtList
void prgm()
{
  //initiate the stmtList terminal symbol
  stmtList();
  //can I do something to eliminate this variable?
  token lastToken = currentToken;
  //once done with stmt list, iterate to the end of file symbol
  currentToken = calcLex();

  if(currentToken == endOfFileSym)
  {
    return;
  }
  else
  {
    //basic exception message
    cout << "Received unexpected token of type: \'" << lastToken.typeToString()
    << "\' and data: \'" << lastToken.data() << "\'" << endl;
  }
}

// stmtList -> stmt stmtList
//          -> nothing
void stmtList()
{
  //if the first statement is valid, keep moving through the program
  if(stmt())
  {
    stmtList();
  }
}

// stmt -> identifier := expr
//      -> read identifier
//      -> write expr
bool stmt()
{
  //if the first token in this statement is an identifier...
  if(currentToken == identifier)
  {
    //token to save the identifier name for the symbol table
    token symbolToken = currentToken;
    //checking for the assignment operator
    currentToken = calcLex();
    if(currentToken == assignSym)
    {
      currentToken = calcLex();
      //calculate the expression on the opposite side of the identifier
      double calculation = expr();
      //this line sets the calculation to an existing key
      //or a nonexistent key is initialized with value = "calculation"
      symbolTable[symbolToken.data()] = calculation;
      return true;
    }
    else
    {
      return false;
    }
  }
  //if the first token in this statement is a read symbol...
  else if(currentToken == readSym)
  {
    //get the next token in the statement
    currentToken = calcLex();
    //if it's an identifier (which it should be)
    if(currentToken == identifier)
    {
      //setting up a var so that the user can save their input
      double varValue;
      cout << "Please enter a numeric value for variable " << currentToken.data() << ": ";
      cin >> varValue;
      if(cin.fail())
      {
        cout << "This isn't a double. Try again." << endl;
        return false;
      }

      //this line sets the calculation to an existing key
      //or a nonexistent key is initialized with value = "calculation"
      symbolTable[currentToken.data()] = varValue;
      currentToken = calcLex();
      return true;
    }
    else
    {
      return false;
    }
  }
  else if(currentToken.type() == writeSym)
  {
    currentToken = calcLex();
    if(currentToken == identifier)
    {
      //if the symbol table has the identifier token as a key
      if(!symbolTable.count(currentToken.data()))
      {
        cout << "You didn't declare " << currentToken.data() << " so I'm initializing it to 1." << endl;
        //setting a new variable up in the symbol table
        symbolTable[currentToken.data()] = 1;
      }
      cout << currentToken.data() << " is set as: " << symbolTable[currentToken.data()] << endl;
      currentToken = calcLex();
      return true;
    }
    else
    {
      return false;
    }
  }
  return false;
}

// expr -> term termTail
double expr()
{
  double leftValue = term();
  double fullValue = termTail(leftValue);
  return fullValue;
}

// term -> fctr fctrTail
double term()
{
  double leftValue = fctr();
  double fullValue = fctrTail(leftValue);
  return fullValue;
}

// termTail -> + term termTail
//          -> - term termTail
//          -> nothing
double termTail(double leftSide)
{
  if(currentToken == addOp)
  {
    currentToken = calcLex();
    double firstValue = term();
    double fullValue = termTail(leftSide + firstValue);
    return fullValue;
  }
  else if(currentToken == subOp)
  {
    currentToken = calcLex();
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
  if(currentToken == leftParen)
  {
    currentToken = calcLex();
    double expressionVal = expr();
    if(currentToken == rightParen)
    {
      currentToken = calcLex();
      return expressionVal;
    }
    else
    {
      cout << "I couldn't find a matching right parenthesis." << endl;
      return 1;
    }
  }
  else if(currentToken == identifier)
  {
    double returnVar;
    if(symbolTable.count(currentToken.data()))
    {
      returnVar = symbolTable[currentToken.data()];
    }
    else
    {
      symbolTable[currentToken.data()] = 1;
      returnVar = 1;
    }
    currentToken = calcLex();
    return returnVar;
  }
  else if(currentToken == numConst)
  {
    double returnVar = stod(currentToken.data());
    currentToken = calcLex();
    return returnVar;
  }
  else
  {
    return 1;
  }
}

double fctrTail(double leftSide)
{
  if(currentToken == multOp)
  {
    currentToken = calcLex();
    double firstValue = fctr();
    double fullValue = fctrTail(leftSide * firstValue);
    return fullValue;
  }
  else if(currentToken == divOp)
  {
    currentToken = calcLex();
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
