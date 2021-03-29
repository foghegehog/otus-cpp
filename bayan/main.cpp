#include "settings.h"

#include <boost/any.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace po = boost::program_options;

void to_cout(const vector<string> &v)
{
    auto separator = "";
    for(const auto& s: v)
    {
        cout << separator << s;
        separator = ", "; 
    }
  
    cout << endl;
}

int main(int argc, const char *argv[]) 
{
    auto settings = Settings::parse_from_arguments(argc, argv);

    if (settings.mShowHelp)
    {
        cout << settings.mHelpText;
        return 0;
    } 

    cout << "Directories to scan: ";
    to_cout(settings.mIncudeDirs);

    cout << "Directories to exclude: ";
    to_cout(settings.mExcludeDirs);

    cout << "Scan depth: " << (int)settings.mScanDepth << endl;

    cout << "Minimum file size: " << settings.mMinSize << endl;

    cout << "Files masks: ";
    to_cout(settings.mFileMasks);

    cout << "Block size: " << settings.mBlockSize << endl;

    cout << "Algorithm: " << settings.mHashAlgorithm << endl;

    return 0;
}