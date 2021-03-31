#include "bayan_searcher.h"

void BayanSearcher::search_bayans()
{
    while(!mDirectoryTraversal->is_traversed())
    {
        auto file = mDirectoryTraversal->get_next_file();
        auto is_duplicate = false;
        for(auto& compared_file: mComparisonFiles)
        {
            auto blocks_iterator = file.begin();
            bool blocks_match = true;
            for(const auto& compared_block: compared_file)
            {
                if (blocks_iterator == file.end())
                {
                    blocks_match = false;
                    break;
                }

                if (*blocks_iterator != compared_block)
                {
                    blocks_match = false;
                    break;
                }

                ++blocks_iterator;
            }

            if ((blocks_iterator == file.end()) && blocks_match)
            {
                is_duplicate = true;
                compared_file.mDuplicatePaths.push_back(file.mPath);
            }
        }

        if(!is_duplicate)
        {
            mComparisonFiles.push_back(file);
        }
    }
}