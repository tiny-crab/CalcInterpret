#include "calcLex.h"

//this function opens the "filestream" var and returns true or false for success
ifstream filestream;

bool calcLexOpen(const char filename[])
{
    filestream.open(filename, ios_base::in);
    //returning true or false if the filestream is open
    return filestream.is_open();
}

//this function finds the next token and outputs it
token calcLex()
{
    token curToken;
    char currentChar;

    while(1) //infinite loop, breaks through a return statement
    {
        //get the next char!
        currentChar = filestream.get();

        //used to ignore whitespace as a token
        while( currentChar == ' ' || currentChar == '\t' || currentChar == '\n')
        {
          currentChar = filestream.get();
        };

        //if the current char is the end of a file, it returns the end of file symbol
        if(currentChar == EOF)
        {
            curToken.type(endOfFileSym);
            return curToken;
        }

        //this is the comment logic: if the next chars are /*...
        if(followingChars("/*", currentChar, curToken))
        {
            //then while it isn't ending the line, or finding the end of comment sentinel...
            while(currentChar != '\n' && !followingChars("*/", currentChar, curToken))
            {
              //move onto the next character
              currentChar = filestream.get();
            }
            //move to the next character after the comment or line has ended
            //hop back up to the top of the while loop
            continue;
        }
        //if the chars ":=" are found, return the assignment symbol
        if(followingChars(":=", currentChar, curToken))
        {
            curToken.type(assignSym);
            curToken.data(":=");
            return curToken;
        }
        //if the chars "read" are found, return the read symbol
        if(followingChars("read", currentChar, curToken))
        {
            curToken.type(readSym);
            curToken.data("read");
            return curToken;
        }
        //if the chars "write" are found, return the write symbol
        if(followingChars("write", currentChar, curToken))
        {
            curToken.type(writeSym);
            curToken.data("write");
            return curToken;
        }

        //checks for identifier strings only starting with letters and underscores
        if( isalpha(currentChar) || (currentChar == '_') )
        {
            //messy line, but gets new char and checks for any letters, underscores, and digits
            while( isalnum(currentChar = filestream.get()) || (currentChar == '_'))
            {
                curToken.appendChar(currentChar);
            }

            //once the currentChar falls off the end of the identifier string, it must hop back to read the next one
            filestream.unget();
            //returns the identifier int
            curToken.type(identifier);
            return curToken;
        }

        //if the current char is a number
        if(isdigit(currentChar))
        {
            while ( isdigit(currentChar = filestream.get()) || currentChar == '.')
			      {
				        if (currentChar == '.')
				        {
                  curToken.appendChar(currentChar);
					        currentChar = filestream.get();
					        if (!isdigit(currentChar))
					        {
						        curToken.type(numConstError);
                    return curToken;
					        }
					        while (isdigit(currentChar))
					        {
						        curToken.appendChar(currentChar);
						        currentChar = filestream.get();
					        }
                  filestream.unget();
					        curToken.type(numConst);
                  return curToken;
                }
                curToken.appendChar(currentChar);
            }
            filestream.unget();
            curToken.type(numConst);
            return curToken;
        }
        if(currentChar == '+')
        {
            curToken.type(addOp);
            curToken.data("+");
            return curToken;
        }
        if(currentChar == '-')
        {
            curToken.type(addOp);
            curToken.data("-");
            return curToken;
        }
        if(currentChar == '*')
        {
            curToken.type(multOp);
            curToken.data("*");
            return curToken;
        }
        if(currentChar == '/')
        {
            curToken.type(multOp);
            curToken.data("/");
            return curToken;
        }
        if(currentChar == '(')
        {
            curToken.type(leftParen);
            curToken.data("(");
            return curToken;
        }
        if(currentChar == ')')
        {
            curToken.type(rightParen);
            curToken.data(")");
            return curToken;
        }

        curToken.type(unknownError);
        return curToken;
    }

    curToken.type(unknownError); //this should never get here
    return curToken;
}


//function that finds if the chars following match a target symbol
bool followingChars(string target, char currentChar, token curToken)
{
    for(int x = 0; x < target.length(); x++)
    {
        if(currentChar == target[x])
        {
            curToken.appendChar(currentChar);
            currentChar = filestream.get();
        }
        else
        {
            curToken.clearData();
            while(x != 0)
            {
              filestream.unget();
              x--;
            }
            return false;
        }
    }
    filestream.unget();
    return true;
}
