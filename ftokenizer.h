#ifndef FTOKENIZER_H
#define FTOKENIZER_H
#include <iostream>
#include "statemachine.h"
#include "stokenizer.h"
#include <fstream>
using namespace std;

class FTokenizer
{
public:
    const int MAX_BLOCK = MAX_BUFFER;

    FTokenizer(char* fname)
    {
        _f.open(fname);
        _more = get_new_block();
    }

    Token next_token()
    {
        Token t;
        if(_stk.done())
        {
            _more = get_new_block();
        }
            _stk>>t;
        return t;
    }

    bool more()        //returns the current value of _more
    {
        return _more;
    }
    int pos()         //returns the value of _pos
    {
        return _pos;
    }
    int block_pos()     //returns the value of _blockPos
    {
        return _blockPos;
    }
    friend FTokenizer& operator >> (FTokenizer& f, Token& t)
    {
        bool debug = false;

        if(debug)
        {
            cout<<"in debug"<<endl;
        }
        t = f.next_token();
        return f;

    }
private:

    bool get_new_block(); //gets the new block from the file

    ifstream _f;   //file being tokenized

    STokenizer _stk;     //The STokenizer object to tokenize current block

    int _pos;           //Current position in the file

    int _blockPos;      //Current position in the current block

    bool _more;         //false if last token of the last block
    //  has been processed and now we are at
    //  the end of the last block.
};


bool FTokenizer::get_new_block()
{
    bool debug = false;
    if(debug)
    {
        cout<<"in debug"<<endl;
    }
    char test[MAX_BLOCK];
    _f.read(test,MAX_BLOCK-1);
    test[_f.gcount()]=NULL;

    if(_f.gcount()> 0)
    {
        _more = true;
        _stk.set_string(test);
        return true;
    }
    else
    {
        _more = false;
        _f.close();
        return false;
    }

}

#endif // FTOKENIZER_H
