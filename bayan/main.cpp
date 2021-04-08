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

    unique_ptr<DirectoryTraversal> directory;
    if (settings.mScanDepth == ScanDepth::SCAN_CURRENT) 
    {
        directory = make_unique<BoostDirectoryTraversal<directory_iterator>>(
            settings.mIncudeDirs, settings.mExcludeDirs, settings.mFileMasks, move(hasher), settings.mBlockSize);
    }
    else
    {
        directory = make_unique<BoostDirectoryTraversal<recursive_directory_iterator>>(
            settings.mIncudeDirs, settings.mExcludeDirs, settings.mFileMasks, move(hasher), settings.mBlockSize);
    }     
     
    BayanSearcher searcher(move(directory));
    searcher.search_bayans();

    for(const auto& file: searcher.mComparisonFiles)
    {
        std::cout << file.get_path() << std::endl;
        for (const auto& duplicate: file.get_duplicates())
        {
            std::cout << duplicate << std::endl;
        }
        std::cout << std::endl;
        
    }
    
    return 0;
}