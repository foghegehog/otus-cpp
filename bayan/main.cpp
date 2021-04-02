#include "boost_directory_traversal.h"
#include "settings.h"

#include <boost/any.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, const char *argv[]) 
{
    auto settings = Settings::parse_from_arguments(argc, argv);

    if (settings.mShowHelp)
    {
        cout << settings.mHelpText;
        return 0;
    } 

    BoostDirectoryTraversal<boost::filesystem::recursive_directory_iterator> directory(settings.mIncudeDirs[0]);
    while (!directory.is_traversed())
    {
        cout << directory.get_next_file().mPath << endl;
    }
    
    return 0;
}