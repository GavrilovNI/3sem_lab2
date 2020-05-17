#include <iostream>
#include "Var.h"

using namespace std;

void write(...)
{
}

int main()
{
    string s;
    Var* v;
    
    while (1)
    {
        cout << "__:";
        cin >> s;
        v = Var::Assign(s);

        if (v->type == _int)
            cout << (int)*((_Int*)v) << endl;
        if (v->type == _double)
            cout << (double)*((_Double*)v) << endl;
        if (v->type == _bool)
            cout << (bool)*((_Bool*)v) << endl;
        if (v->type == _string)
            cout << (string) * ((_String*)v) << endl;

        cout << endl;
    }
}