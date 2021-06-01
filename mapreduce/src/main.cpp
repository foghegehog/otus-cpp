#include <cstdlib>
#include <iostream>
#include <string>


int main(int argc, char* argv[])
{
    using namespace std;

    std::string src;
    int mnum, rnum; 
    if (argc == 1)
    {
        src = "mailing_list.txt";
        mnum = 10;
        rnum = 5;
    }
    else if (argc != 4)
    {
        cout << "Usage:  mapreduce <src> <mnum> <rnum>" << endl;
        return 0;
    }
    else
    {
        src = argv[0];
        mnum = atoi(argv[1]);
        rnum = atoi(argv[2]);
    }

    cout << mnum << rnum;

    return 0;
}