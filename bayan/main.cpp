#include "boost_directory_traversal.h"
#include "settings.h"

#include <boost/any.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;
using namespace boost::filesystem;

int main(int argc, const char *argv[]) 
{
    auto settings = Settings::parse_from_arguments(argc, argv);

    if (settings.mShowHelp)
    {
        cout << settings.mHelpText;
        return 0;
    } 

    shared_ptr<DirectoryTraversal> directory;
    if (settings.mScanDepth == ScanDepth::SCAN_CURRENT) 
    {
        directory = make_shared<BoostDirectoryTraversal<directory_iterator>>(settings.mIncudeDirs, settings.mExcludeDirs);
    }
    else
    {
        directory = make_shared<BoostDirectoryTraversal<recursive_directory_iterator>>(settings.mIncudeDirs, settings.mExcludeDirs);
    }     
     
    while (!directory->is_traversed())
    {
        cout << directory->get_next_file().get_path() << endl;
    }
    
    return 0;
}