#ifndef STOKENIZER_H
#define STOKENIZER_H

#include <iostream>
#include <string>
#include "statemachine.h"
#include "constantnumber.h"
#include <cstring>
#include <cassert>

using namespace std;

class Token
{
public:
    Token(){}
    //only way to change the string str, and int type;
    Token(string str, int type)
    {
        _token = str;
        _type = type;
    }
    friend ostream& operator <<(ostream& outs, const Token& t)
    {
        outs<<"|"<<t._token<<"|";
        return outs;
    }
    int type()
    {
        return _type;
    }
    string type_string();

    string token_str()
    {
        return _token;
    }
private:
    string _token ="";
    int _type =0;
};

string Token::type_string()
{
    if(_type == DOUBLE_ROW)
        return "NUMBER";
    else if(_type == ALPHA_ROW)
        return "WORD";
    else if(_type == SYMBOL_ROW)
        return "SYMBOL";
    else if(_type == SPACE_ROW)
        return "SPACE";
    else
        return "UNKNOWN";
}

class STokenizer
{
public:
    STokenizer()
    {
        make_table(_table);
    }
    STokenizer(char str[])
    {
        make_table(_table);
        //        print_table(_table);
        set_string(str);
    }
    STokenizer(string str)
    {
        make_table(_table);

        char a[1024];
        bool debug = false;
        if(debug) cout<<"string: "<<str<<endl;
        if(debug) cout<<"a: "<<a<<endl;
        for(size_t i = 0 ; i < str.size();i++)
        {
            a[i] = str[i];
        }
        a[str.size()] = NULL;
        if(debug) cout<<"a: "<<a<<endl;
        set_string(a);
    }
    //true: there are no more tokens
    bool done()
    {
            return _pos >=strlen(_buffer);
    }
    //true: there are more tokens
    bool more()
    {
        return _pos <= strlen(_buffer);
    }
    //-- big three --

    friend STokenizer& operator >> (STokenizer& s, Token& t);
    void set_string(char str[]);

private:
    void make_table(int _table[][MAX_COLUMNS]);
    bool get_token(int start_state, string& token);
    int last_suc = -1;

    char _buffer[MAX_BUFFER];       //input string
    int _pos = 0;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
};

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

//set a new string as the input string
void STokenizer::set_string(char str[])
{
    bool debug = false;
    strcpy(_buffer,str);
    if(debug) cout<<"buffer: "<<_buffer<<endl;
    last_suc = -1;
    _pos = 0;
}

//create table for all the tokens we will recognize
//                      (e.g. doubles, words, etc.)
void STokenizer::make_table(int _table[][MAX_COLUMNS])
{
    init_table(_table);

    //DIGIT
    mark_fail(_table, DOUBLE_ROW);
    mark_success(_table, DOUBLE_ROW+1);
    mark_fail(_table,DOUBLE_ROW+2);
    mark_success(_table, DOUBLE_ROW+3);

    mark_cells(DOUBLE_ROW, _table,DIGIT,DOUBLE_ROW+1);
    mark_cells(DOUBLE_ROW, _table, '.', '.', DOUBLE_ROW+2);
    //    mark_cells(DOUBLE_ROW, _table, '-', '-', DOUBLE_ROW+1);
    mark_cells(DOUBLE_ROW+1, _table,DIGIT, DOUBLE_ROW+1);
    mark_cells(DOUBLE_ROW+1, _table, '.', '.', DOUBLE_ROW+2);
    mark_cells(DOUBLE_ROW+2, _table,DIGIT, DOUBLE_ROW+3);
    mark_cells(DOUBLE_ROW+3, _table,DIGIT, DOUBLE_ROW+3);


    //    EXCEPT DIGITI SPACE ALPHA
    //UNKNOWN CASE
    mark_cells(SYMBOL_ROW,_table,0,MAX_COLUMNS,SYMBOL_ROW+1);
    mark_cells(SYMBOL_ROW+1,_table,0,MAX_COLUMNS,SYMBOL_ROW+2);

    mark_fail(_table,SYMBOL_ROW);
    mark_success(_table,SYMBOL_ROW+1);
    mark_fail(_table,SYMBOL_ROW+2);

    mark_cells(SYMBOL_ROW,_table,ALPHA,UNKNOWN_TOKEN);
    mark_cells(SYMBOL_ROW,_table,DIGIT,UNKNOWN_TOKEN);
    mark_cells(SYMBOL_ROW,_table,' ', ' ',UNKNOWN_TOKEN);

    mark_cells(SYMBOL_ROW+1,_table,ALPHA,UNKNOWN_TOKEN);
    mark_cells(SYMBOL_ROW+1,_table,DIGIT,UNKNOWN_TOKEN);
    mark_cells(SYMBOL_ROW+1,_table,' ', ' ',UNKNOWN_TOKEN);

    //        mark_success(_table,SYMBOL_ROW);
    //        mark_cells(SYMBOL_ROW,_table,'.','.',SYMBOL_ROW+1);

//    SPACE
    mark_success(_table,SPACE_ROW);
    mark_cells(SPACE_ROW,_table,' ',' ',SPACE_ROW);

    //ALPHA
//    mark_fail(_table,ALPHA_ROW);
//    mark_success(_table,ALPHA_ROW+1);
//    mark_success(_table,ALPHA_ROW+2);
//    mark_fail(_table,ALPHA_ROW+3);
//    mark_fail(_table,ALPHA_ROW+4);
//    mark_success(_table,ALPHA_ROW+5);
//    mark_success(_table,ALPHA_ROW+6);

//    mark_cells(ALPHA_ROW,_table,ALPHA,ALPHA_ROW+1);
//    mark_cells(ALPHA_ROW+1,_table,ALPHA,ALPHA_ROW+1);
//    mark_cells(ALPHA_ROW+1,_table,DIGIT,ALPHA_ROW+2);
//    mark_cells(ALPHA_ROW+2,_table,DIGIT,ALPHA_ROW+3);
//    mark_cells(ALPHA_ROW+3,_table,ALPHA,ALPHA_ROW+3);
//    mark_cells(ALPHA_ROW+3,_table,DIGIT,ALPHA_ROW+2);

//    mark_cells(ALPHA_ROW+1,_table,39,39,ALPHA_ROW+4);
//    mark_cells(ALPHA_ROW+4,_table,ALPHA,ALPHA_ROW+5);
//    mark_cells(ALPHA_ROW+5,_table,ALPHA,ALPHA_ROW+6);

    mark_success(_table,ALPHA_ROW);
    mark_success(_table,ALPHA_ROW+1);
    mark_cells(ALPHA_ROW,_table,ALPHA,ALPHA_ROW+1);
    mark_cells(ALPHA_ROW+1,_table,DIGIT,ALPHA_ROW+1);
    mark_cells(ALPHA_ROW+1,_table,ALPHA,ALPHA_ROW+1);
}

//extract the longest string that match
//     one of the acceptable token types
bool STokenizer::get_token(int start_state, string &token)
{
    if (_pos>strlen(_buffer))
        return false;
    string temp;
    bool debug = false;
    bool test = false;
    if(debug) cout<<"startstat1e"<<start_state<<endl;
    if(debug) cout<<"GET_TOKEN GET_TOKEN"<<endl;
    start_state = _table[start_state][(unsigned char)_buffer[_pos]];
    if(debug)    cout<<"_buffer"<<_buffer<<endl;
    if(debug)    cout<<"pos"<<_pos<<endl;
    if(debug)    cout<<"buffer pos "<<(int)_buffer[_pos]<<endl;
    if(debug)   cout<<"startsta2te"<<start_state<<endl;

    while(start_state!=-1&&!done())
    {
        //        cout<<"startst3ate"<<start_state<<endl;

        temp += _buffer[_pos];
        if(is_success(_table,start_state))
        {
            test = true;
            token = temp;
            last_suc = _pos;
        }
        ++_pos;
        start_state = _table[start_state][_buffer[_pos]];
    }
    if(debug) cout<<"strlen(_buffer)"<<strlen(_buffer)<<endl;
    if(debug) cout<<"pos"<<_pos<<endl;
//    if(strlen(_buffer)==_pos-1)
//    {
//        if(debug) cout<<"same"<<endl;
//        return true;
//    }
    if(debug) cout<<"+1+1+1"<<endl;
    _pos = last_suc +1;
    if(test)
    {
        return true;
    }
    else
    {
        return false;
    }
}
STokenizer& operator >> (STokenizer& s, Token& t)
{
    string temp;

    bool debug = false;

    if(debug) cout<<"the following is alpha"<<endl;
    if(s.get_token(ALPHA_ROW, temp)){
        if(debug)
        {
            cout<<"in alpha"<<endl;
            cout<<"temp"<<temp<<"|"<<endl;
        }
        t= Token(temp, ALPHA_ROW);

        return s;
    }
    if(debug) cout<<"the following is space"<<endl;
    if(s.get_token(SPACE_ROW, temp)){
        if(debug)
        {
            cout<<"in space"<<endl;
            cout<<"temp"<<temp<<"|"<<endl;
        }
        t= Token(temp, SPACE_ROW);

        return s;
    }
    if(debug) cout<<"the following is double"<<endl;
    if(s.get_token(DOUBLE_ROW, temp)){
        if(debug)
        {
            cout<<"in double"<<endl;
            cout<<"temp"<<temp<<"|"<<endl;
            cout<<"s.pos"<<s._pos<<endl;
        }
        t = Token(temp, DOUBLE_ROW);

        return s;
    }
    if(debug) cout<<"the following is symbol"<<endl;
    if(s.get_token(SYMBOL_ROW, temp))
    {
        if(debug)
        {
            cout<<"in symbol"<<endl;
            cout<<"temp"<<temp<<"|"<<endl;
            cout<<"s.pos"<<s._pos<<endl;
        }
        t= Token(temp, SYMBOL_ROW);

        return s;
    }

    if(debug) cout<<"the following is unknown"<<endl;
    t = Token({temp,'\0'},-1);
    ++s._pos;
    ++s.last_suc;

    return s;
}
#endif // STOKENIZER_H
