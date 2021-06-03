#ifndef MIN_PREFIX_FUNCTIONS_H
#define MIN_PREFIX_FUNCTIONS_H

#include "../include/max_summator.h"

#include <functional>
#include <string>
#include <utility>

std::pair<std::string, int> get_prefix_pair(int prefix_len, const std::string& str);

std::function<std::pair<std::string, int>(const std::string&)> get_prefix_pair_function(int prefix_len);

void accumulate_key_sum(max_summator<std::string, int> & accum, const std::pair<std::string, int>& pair);

#endif