#include "calcLex.h"

//CalcLex Grammar!
// prgm -> stmtList
//
// stmtList -> stmt stmtList
//          -> nothing
//
// stmt -> ID := expr
//      -> read ID
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
//      -> ID
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

int token;

int calcParse()
{
  token = calcLex();
  prgm();
  return 0;
}

void match(int expectedToken)
{
  if(token == expectedToken)
  {
    if(token != endOfFileSym)
    {
      token == calcLex();
    }
    else
    {
      cout << "Token " << expectedToken << " expected, but instead found: " << calcText << endl;
    }
  }
}

int prgm()
{
  int val = stmtList();
  match(endOfFileSym);

  cout << "The grammar says: " << val << endl;
  return val;
}

int stmtList()
{
  int val1 = stmt();
  int val2 = stmtList(val1);
  return val2;
}

int stmt()
{
  if(token == identifier)
  {

  }
  else if(token == readSym)
  {

  }
  else if(token = writeSym)
  {

  }
  else
  {

  }
}
