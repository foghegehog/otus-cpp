#ifndef SETTINGS_H
#define SETTINGS_H

#include "scan_depth.h"

#include <array>
#include <string>
#include <vector>

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

    static std::string CRC32_STR;
    static std::string MD5_STR;
    
private:
    Settings(){};

    template <typename T>
    static void check_value_supported(const T& value, const std::initializer_list<T>& supported_values, std::string option_name);

    template <typename T>
    static void check_value_non_negative(const T& value, std::string option_name);
};

#endif