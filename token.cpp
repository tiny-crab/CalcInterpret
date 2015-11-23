#include "calcLex.h"

//this function resets the token's string data
void token::clearData()
{
    tokenData = "";
}

//this function pushes chars onto the token's data string
void token::appendChar(char currentChar)
{
    tokenData += currentChar;
}

int token::type()
{
    return tokenType;
}

void token::type(int change)
{
    tokenType = change;
}

string token::typeToString()
{
  switch (tokenType) {
    case 0: return "End of File Symbol";
    case 1: return "Assignment Symbol";
    case 2: return "Read Symbol";
    case 3: return "Write Symbol";
    case 4: return "Identifier";
    case 5: return "Add Operation";
    case 6: return "Subtract Operation";
    case 7: return "Multiplication Operation";
    case 8: return "Division Operation";
    case 9: return "Left Parenthesis";
    case 10: return "Right Parenthesis";
    case 11: return "Numeric Constant";
    case 12: return "Unknown Token Error";
    case 13: return "Issue with numConsts";
    default: return "This error isn't known.";
  }
}

string token::data()
{
  return tokenData;
}

void token::data(string change)
{
  tokenData = change;
}

bool token::operator==(token compareToken)
{
  if(tokenType == compareToken.type())
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool token::operator==(int compareType)
{
  if(tokenType == compareType)
  {
    return true;
  }
  else
  {
    return false;
  }
}
