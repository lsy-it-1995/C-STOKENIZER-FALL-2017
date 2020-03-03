#include <iostream>
#include "stokenizer.h"
#include "statemachine.h"
#include "ftokenizer.h"

using namespace std;
void test_f_tokenize_simple();
void s_tokenizer();
int main()
{
    //test_f_tokenize_simple();
    s_tokenizer();
}
void s_tokenizer()
{
    char s[] = "select * from student where lname = 'jones' and age > 23 .";
    //    char s[] = "3.14..";

    cout<<"size is"<<strlen(s)<<endl;
    STokenizer stk(s);
    Token t;
    stk>>t;
    while(!stk.done()){
        cout<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;
        t = Token();
        stk>>t;
    }
    cout<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;

}

void test_f_tokenize_simple(){
    ios_base::sync_with_stdio(false);
    Token t;
    FTokenizer ftk("solitude.txt");
    ftk>>t;
    ofstream ofs("FK.txt");
    int token_count = 0;
    while (ftk.more()){
        if (t.type_string()=="WORD" ){
            {
                token_count++;
                ofs<<setw(10)<<right<<token_count
                   <<setw(3)<<left<<":"<<setw(25)
                  <<left
                 <<t.token_str()
                  <<t.type_string()
                 <<endl;
//                cout<<setw(10)<<right<<token_count
//                   <<setw(3)<<left<<":"<<setw(25)<<left<<t.token_str()
//                  <<t.type_string()<<endl;

            }
        }
        ftk>>t;
    }
    ofs.close();
    cout<<"Tokens Found: "<<token_count<<endl;
    cout<<"=========="<<endl;

}
