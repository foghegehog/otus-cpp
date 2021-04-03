#include <boost/test/unit_test.hpp>
#include <memory>
#include <vector>

#include "bayan_searcher.h"

using namespace std;

class FileReaderMock: public FileReader
{
public:
    FileReaderMock(const vector<vector<uint8_t>>& blocks)
        :mBlocks(blocks)
    {
        mCurrentBlock = mBlocks.begin();
    }

    size_t max_block_size()
    {
        return mBlocks[0].size();
    }

    bool is_eof()
    {
        return mCurrentBlock == mBlocks.end();
    }

    size_t read_block(std::vector<uint8_t>& buffer)
    {
        mReadsCount++;
        copy(mCurrentBlock->begin(), mCurrentBlock->end(), back_inserter(buffer));
        mCurrentBlock++;
        return mBlocks[0].size(); 
    }

    int reads_count()
    {
        return mReadsCount;
    }

private:
    vector<vector<uint8_t>> mBlocks;
    vector<vector<uint8_t>>::const_iterator mCurrentBlock; 
    int mReadsCount = 0;
};

class HasherMock: public Hasher
{
    vector<uint8_t> hash_block(vector<uint8_t> block)
    {
        return block;
    }
};

class DirectoryTraversalMock : public DirectoryTraversal
{
public:
    DirectoryTraversalMock()
    {
    }

    void AddFile(const string& path, size_t size, shared_ptr<FileReaderMock> fileReader)
    {
        FileComparison fileAComparison(path, size, fileReader, mHasher);
        mFiles.push_back(fileAComparison);
    }

    void initialize()
    {
        mCurrentFile = mFiles.begin();
    }

    bool is_traversed() override
    {
        return mCurrentFile == mFiles.end();
    }

    FileComparison get_next_file() override
    {
        auto file = *mCurrentFile;
        mCurrentFile++;
        return file;
    } 
private:
    vector<FileComparison> mFiles;
    vector<FileComparison>::const_iterator mCurrentFile;
    shared_ptr<HasherMock> mHasher = make_shared<HasherMock>();
};

BOOST_AUTO_TEST_SUITE(Search)

BOOST_AUTO_TEST_CASE(test_two_equal_files)
{
    auto traversal = new DirectoryTraversalMock();
    auto fileABlocks = vector<vector<uint8_t>>
    {
        vector<uint8_t>{1, 3, 5},
        vector<uint8_t>{2, 4, 6}, 
    };
    auto fileAReader = make_shared<FileReaderMock>(fileABlocks);
    traversal->AddFile("A", 2*3, fileAReader);


    auto fileBBlocks = vector<vector<uint8_t>>
    {
        vector<uint8_t>{1, 3, 5},
        vector<uint8_t>{2, 4, 6}, 
    };
    auto fileBReader = make_shared<FileReaderMock>(fileBBlocks);
    traversal->AddFile("B", 2*3, fileBReader);

    traversal->initialize();

    BayanSearcher searcher(move(unique_ptr<DirectoryTraversal>(traversal)));
    searcher.search_bayans();

    BOOST_TEST(searcher.mComparisonFiles.size()==1);
    BOOST_TEST(searcher.mComparisonFiles[0].get_duplicates().size() == 1);
    BOOST_TEST(fileAReader->reads_count() == 2);
    BOOST_TEST(fileBReader->reads_count() == 2);
}

BOOST_AUTO_TEST_CASE(test_two_different_files)
{
    auto traversal = new DirectoryTraversalMock();
    auto fileABlocks = vector<vector<uint8_t>>
    {
        vector<uint8_t>{1, 3, 5},
        vector<uint8_t>{2, 4, 6}, 
    };
    auto fileAReader = make_shared<FileReaderMock>(fileABlocks);
    traversal->AddFile("A", 2*3, fileAReader);


    auto fileBBlocks = vector<vector<uint8_t>>
    {
        vector<uint8_t>{2, 4, 6}, 
        vector<uint8_t>{1, 3, 5}
    };

    auto fileBReader = make_shared<FileReaderMock>(fileBBlocks);
    traversal->AddFile("B", 2*3, fileBReader);

    traversal->initialize();

    BayanSearcher searcher(move(unique_ptr<DirectoryTraversal>(traversal)));
    searcher.search_bayans();

    BOOST_TEST(searcher.mComparisonFiles.size()==2);
    BOOST_TEST(searcher.mComparisonFiles[0].get_duplicates().size() == 0);
    BOOST_TEST(searcher.mComparisonFiles[1].get_duplicates().size() == 0);
    BOOST_TEST(fileAReader->reads_count() == 1);
    BOOST_TEST(fileBReader->reads_count() == 1);
}

BOOST_AUTO_TEST_CASE(test_longer_file)
{
    auto traversal = new DirectoryTraversalMock();
    auto fileABlocks = vector<vector<uint8_t>>
    {
        vector<uint8_t>{1, 3, 5},
        vector<uint8_t>{2, 4, 6},
        vector<uint8_t>{2, 4, 6} 
    };
    auto fileAReader = make_shared<FileReaderMock>(fileABlocks);
    traversal->AddFile("A", 3*3, fileAReader);


    auto fileBBlocks = vector<vector<uint8_t>>
    {
        vector<uint8_t>{1, 3, 5},
        vector<uint8_t>{2, 4, 6}, 
    };
    auto fileBReader = make_shared<FileReaderMock>(fileBBlocks);
    traversal->AddFile("B", 3*2, fileBReader);

    traversal->initialize();

    BayanSearcher searcher(move(unique_ptr<DirectoryTraversal>(traversal)));
    searcher.search_bayans();

    BOOST_TEST(searcher.mComparisonFiles.size()==2);
    BOOST_TEST(searcher.mComparisonFiles[0].get_duplicates().size() == 0);
    BOOST_TEST(searcher.mComparisonFiles[1].get_duplicates().size() == 0);
    BOOST_TEST(fileAReader->reads_count() == 2);
    BOOST_TEST(fileBReader->reads_count() == 2);
}

BOOST_AUTO_TEST_CASE(test_shorter_file)
{
    auto traversal = new DirectoryTraversalMock();
    auto fileABlocks = vector<vector<uint8_t>>
    {
        vector<uint8_t>{1, 3, 5},
    };
    auto fileAReader = make_shared<FileReaderMock>(fileABlocks);
    traversal->AddFile("A", 3, fileAReader);


    auto fileBBlocks = vector<vector<uint8_t>>
    {
        vector<uint8_t>{1, 3, 5},
        vector<uint8_t>{2, 4, 6}, 
    };
    auto fileBReader = make_shared<FileReaderMock>(fileBBlocks);
    traversal->AddFile("B", 2*3, fileBReader);

    traversal->initialize();

    BayanSearcher searcher(move(unique_ptr<DirectoryTraversal>(traversal)));
    searcher.search_bayans();

    BOOST_TEST(searcher.mComparisonFiles.size()==2);
    BOOST_TEST(searcher.mComparisonFiles[0].get_duplicates().size() == 0);
    BOOST_TEST(searcher.mComparisonFiles[1].get_duplicates().size() == 0);
    BOOST_TEST(fileAReader->reads_count() == 1);
    BOOST_TEST(fileBReader->reads_count() == 1);
}

BOOST_AUTO_TEST_CASE(test_single_file)
{
    auto traversal = new DirectoryTraversalMock();
    auto fileABlocks = vector<vector<uint8_t>>
    {
        vector<uint8_t>{1, 3, 5},
        vector<uint8_t>{2, 4, 6}, 
        vector<uint8_t>{2, 4, 6}, 
    };
    auto fileAReader = make_shared<FileReaderMock>(fileABlocks);
    traversal->AddFile("A", 3*3, fileAReader);

    traversal->initialize();

    BayanSearcher searcher(move(unique_ptr<DirectoryTraversal>(traversal)));
    searcher.search_bayans();

    BOOST_TEST(searcher.mComparisonFiles.size()==1);
    BOOST_TEST(fileAReader->reads_count() == 0);
}

BOOST_AUTO_TEST_SUITE_END()
