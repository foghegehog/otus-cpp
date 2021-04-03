#include "bayan_searcher.h"

void BayanSearcher::search_bayans()
{
    while(!mDirectoryTraversal->is_traversed())
    {
        auto file = mDirectoryTraversal->get_next_file();
        auto is_duplicate = false;
        for(auto& compared_file: mComparisonFiles)
        {
            bool blocks_match = true;
            auto blocks_iterator = file.begin();
            auto compared_iter = compared_file.begin();
            for( ;(blocks_iterator != file.end()) && (compared_iter != compared_file.end()); ++blocks_iterator, ++compared_iter)
            {
                if (*blocks_iterator != *compared_iter)
                {
                    blocks_match = false;
                    break;
                }
            }

            if ((blocks_iterator == file.end()) && (compared_iter == compared_file.end()) && blocks_match)
            {
                is_duplicate = true;
                compared_file.add_duplicate(file.get_path());
            }
        }

        if(!is_duplicate)
        {
            mComparisonFiles.push_back(file);
        }
    }
}