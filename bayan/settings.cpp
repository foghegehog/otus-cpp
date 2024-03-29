#include "settings.h"

#include <boost/any.hpp>
#include <boost/program_options.hpp>
#include <sstream>

using namespace std;
namespace po = boost::program_options;

std::string Settings::CRC32_STR = std::string("crc32");
std::string Settings::MD5_STR = std::string("md5");

Settings Settings::parse_from_arguments(int argc, const char *argv[])
{
    Settings settings;

    po::options_description desc("An util to search duplicates among files. Command line options");
    desc.add_options()
        ("help,h", "produces help message")
        ("include_dirs,i", 
            po::value<vector<string>>()->default_value(vector<string>{"."}, ".")->multitoken()->composing(),
            "Directories to be scanned, can be multiple. Default is current folder.")
        ("exclude_dirs,e",
            po::value<vector<string>>()->multitoken()->composing(),
            "Directories to be excluded, can be multiple.")
        ("depth,d", 
            po::value<unsigned short>()->default_value(0)->notifier([](const unsigned short& value)
            {
                auto supported_depths = 
                {
                    static_cast<unsigned short>(ScanDepth::SCAN_CURRENT),
                    static_cast<unsigned short>(ScanDepth::SCAN_ALL)
                };
                check_value_supported(value, supported_depths, "depth");
            }),
            "Scan depth, 1 - all directories, 0 - current folder only. Default value is 0.")
        ("min_size,m",
            po::value<int>()->default_value(1)->notifier([](const int& value)
            {
                check_value_non_negative(value, "min_size");
            }), 
            "Minimum size of the file to be processed, in bytes. Default value is 1.")
        ("file_masks,f",
            po::value<vector<string>>()->multitoken()->composing(), 
            "Masks for files participating in comparison, case-insensitive.")
        ("block_size,b",
            po::value<int>()->default_value(256)->notifier([](const int& value)
            {
                check_value_non_negative(value, "block_size");
            }), 
            "The size of the block to read the files with, in bytes. Default value is 256.")
        ("algorithm,a",
            po::value<string>()->default_value("crc32")->notifier([](const string& value)
            {
                check_value_supported(value, {CRC32_STR, MD5_STR}, "algorithm");
            }),
            "Hash algorithm to hash file blocks. Default value is crc32.")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    notify(vm);

    stringstream help;
    help << desc << endl;
    settings.mHelpText = help.str();
    settings.mShowHelp = vm.count("help");
    
    settings.mIncudeDirs = vm["include_dirs"].as<vector<string>>();
    if (vm.count("exclude_dirs"))
    {
        settings.mExcludeDirs = vm["exclude_dirs"].as<vector<string>>();
    }
    settings.mScanDepth = static_cast<ScanDepth>(vm["depth"].as<unsigned short>());
    settings.mMinSize = static_cast<unsigned int>(vm["min_size"].as<int>());
    if (vm.count("file_masks"))
    {
        settings.mFileMasks = vm["file_masks"].as<vector<string>>();
    }
    settings.mBlockSize = static_cast<unsigned int>(vm["block_size"].as<int>());
    settings.mHashAlgorithm = vm["algorithm"].as<string>();

    return settings;
}

template <typename T>
void Settings::check_value_supported(const T& value, const std::initializer_list<T>& supported_values, std::string option_name)
{
    for (const auto& supported : supported_values)
    {
        if (value == supported)
        {
            return;
        }
    }

    throw po::validation_error(po::validation_error::invalid_option_value, option_name);
}

template <typename T>
void Settings::check_value_non_negative(const T& value, std::string option_name)
{
    if (value < 0)
    {
        throw po::validation_error(po::validation_error::invalid_option_value, option_name);
    }
}

