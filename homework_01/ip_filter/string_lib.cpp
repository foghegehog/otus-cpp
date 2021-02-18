#include "string_lib.h"

#include <iostream>

using namespace std;

vector<string> split(const string &str, char d)
{
    vector<string> r;
    string::size_type start = 0;
    for(auto stop = str.find_first_of(d); stop != string::npos; stop = str.find_first_of(d, start))
    {
        r.push_back(str.substr(start, stop - start));
        start = stop + 1;
    }

    r.push_back(str.substr(start));
    return r;
}
