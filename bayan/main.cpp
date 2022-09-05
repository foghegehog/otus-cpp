#include "bayan_searcher.h"
#include "boost_directory_traversal.h"
#include "crc32_hasher.h"
#include "md5_hasher.h"
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

    shared_ptr<Hasher> hasher;
    if (settings.mHashAlgorithm == Settings::CRC32_STR)
    {
        hasher = make_shared<Crc32Hasher>();
    }
    else
    {
        hasher = make_shared<Md5Hasher>();
    }

    unique_ptr<DirectoryTraversal> directoryTraversal;
    if (settings.mScanDepth == ScanDepth::SCAN_CURRENT) 
    {
        directoryTraversal = make_unique<BoostDirectoryTraversal<directory_iterator>>(
            settings, move(hasher));
    }
    else
    {
        directoryTraversal = make_unique<BoostDirectoryTraversal<recursive_directory_iterator>>(
            settings, move(hasher));
    }     
     
    BayanSearcher searcher(move(directoryTraversal));
    searcher.search_bayans();

    bool first = true;
    for(const auto& file: searcher.mComparisonFiles)
    {
        if (!first)
        {
            std::cout << endl;
        }
        else
        { 
            first = false;
        }

        std::cout << file.get_path() << std::endl;
        for (const auto& duplicate: file.get_duplicates())
        {
            std::cout << duplicate << std::endl;
        }
    }
    
    return 0;
}