#ifndef SETTINGS_H
#define SETTINGS_H

#include <array>
#include <string>
#include <vector>

enum class ScanDepth
{
    SCAN_CURRENT = 0,
    SCAN_ALL = 1
};

class Settings
{
public:
    static Settings parse_from_arguments(int argc, const char *argv[]);
    
    bool mShowHelp;
    std::string mHelpText;
    std::vector<std::string> mIncudeDirs;
    std::vector<std::string> mExcludeDirs;
    ScanDepth mScanDepth;
    unsigned int mMinSize;
    std::vector<std::string> mFileMasks;
    unsigned int mBlockSize;
    std::string mHashAlgorithm;

private:
    Settings(){};

    template <typename T>
    static void check_value_supported(const T& value, const std::initializer_list<T>& supported_values, std::string option_name);
};

#endif