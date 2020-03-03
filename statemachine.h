#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <iostream>
#include <cstring>
#include "constantnumber.h"
#include <iomanip>

using namespace std;

void init_table(int _table[][MAX_COLUMNS]);
void mark_success(int _table[][MAX_COLUMNS], int state);
void mark_fail(int _table[][MAX_COLUMNS], int state);
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state);
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state);
void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state);
void print_table(int _table[][MAX_COLUMNS]);
void show_string(char s[], int _pos);
bool is_success(int _table[][MAX_COLUMNS], int state);

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS])
{
    for(int i =0;i<MAX_ROWS;i++)
    {
        for(int j = 0;j<MAX_COLUMNS;j++)
        {
            _table[i][j]=-1;
        }
    }
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0]=1;
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0]=0;
}

//true if state is a success state search
bool is_success(int _table[][MAX_COLUMNS], int state)
{
    return (_table[state][0] == 1);
}
//Mark a range of cells in the array.
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state)
{
    for(int i = from;i<=to;i++)
    {
        _table[row][i]=state;
    }
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state)
{
    for(unsigned int i = 0;i<strlen(columns);i++)
    {
        _table[row][(int)columns[i]]=state;
    }
}

//Mark this row and column
void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state)
{
    _table[row][column]= state;
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS])
{
    for(int i =0;i<MAX_ROWS;i++)
    {
        cout<<"["<<i<<"]\t";
        for(int j = 0;j<MAX_COLUMNS;j++)
        {
            cout<<setw(3)<<_table[i][j];
            if(j%10==0)
                cout<<endl;

        }
        cout<<endl;
    }
}

//show string s and mark this position on the string:
//hello world   pos: 7
//      ^
void show_string(char s[], int _pos)
{
    cout<<s<<"\t pos: "<<_pos<<endl;
    cout<<setw(_pos-1)<<"^"<<endl;
}

#endif // STATEMACHINE_H
