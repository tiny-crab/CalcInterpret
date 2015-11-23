#include "calcLex.h"

int main(int argc, const char *argv[])
{
    //get the file name from the command line input
    if(argc > 1 && (!calcLexOpen( argv[1]) ) )
    {
        cout << "Sorry, I can't find/open that file!" << argv[1] << endl;
        exit(1);
    }

    // token curToken = calcLex();
    // while( curToken.type() <= numConst )
    // {
    //     cout << "Token(" << curToken.type() << ", " << curToken.typeToString() << "): " << curToken.data() << endl;
    //     curToken = calcLex();
    //     if(curToken.type() == endOfFileSym)
    //     {
    //       break;
    //     }
    // }
    // if(curToken.type() > numConst)
    // {
    //   cout << "Unidentified Token! Sorry, I can't continue interpreting your code. "
    //   << "I was passed a token with value: " << curToken.data() << endl;
    // }

    calcParse();

    return 0;
}
