#include <iostream>
#include <string>

int main()
{
  std::string k_n_str;

  getline (std::cin, k_n_str);
  
  auto space_indx = k_n_str.find(' ');
  
  auto k = stoi(k_n_str.substr(0, space_indx));
  auto n = stoi(k_n_str.substr(space_indx+1));
  
  std::string ai_str;
  getline (std::cin, ai_str);
  long jam = 0;
  int start = 0;
  
  for(auto i=0; i<n; i++)
  {
      auto space_indx = ai_str.find(' ', start);
      auto ai = stoi(ai_str.substr(start, space_indx));
      jam += ai - k;
      jam = jam > 0 ? jam : 0;
      start = space_indx + 1;
  }
  
  std::cout << jam;
  
}

