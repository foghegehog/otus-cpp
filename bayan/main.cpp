#include <array>
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

const string DEFAULT_DIR = ".";

const unsigned short SCAN_ALL = 1;
const unsigned short SCAN_CURRENT = 0;

template <typename T>
void check_value_supported(const T& value, initializer_list<T> supported_values, string option_name)
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

int main(int argc, const char *argv[]) 
{
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
                check_value_supported(value, {SCAN_ALL, SCAN_CURRENT}, "depth");
            }),
            "Scan depth, 1 - all directories, 0 - current folder only. Default value is 0.")
        ("min_size,m",
            po::value<unsigned int>()->default_value(1), 
            "Minimum size of the file to be processed, in bytes.")
        ("file_masks,f",
            po::value<vector<string>>()->multitoken()->composing(), 
            "Masks for files participating in comparison.")
        ("block_size,b",
            po::value<unsigned int>()->default_value(256), 
            "The size of the block to read the files with, in bytes. Default value is 256.")
        ("algorithm,a",
            po::value<string>()->default_value("crc32")->notifier([](const string& value)
            {
                check_value_supported(value, {"crc32"s, "md5"s}, "algorithm");
            }),
            "Hash algorithm to hash file blocks. Default value is crc32.")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (vm.count("help"))
    {
        cout << desc << endl;;
        return 0;
    }    

    cout << "Directories to scan: ";
    to_cout(vm["include_dirs"].as<vector<string>>());

    cout << "Directories to exclude: ";
    if (vm.count("exclude_dirs"))
    {
        to_cout(vm["exclude_dirs"].as<vector<string>>());
    }
    else
    {
        cout << endl;
    }

    cout << "Scan depth: " << vm["depth"].as<unsigned short>() << endl;

    cout << "Minimum file size: " << vm["min_size"].as<unsigned int>() << endl;

    cout << "Files masks: ";
    if (vm.count("file_masks"))
    {
        to_cout(vm["file_masks"].as<vector<string>>());
    }
    else
    {
        cout << endl;
    }

    cout << "Block size: " << vm["block_size"].as<unsigned int>() << endl;

    cout << "Algorithm: " << vm["algorithm"].as<string>() << endl;

    return 0;
}