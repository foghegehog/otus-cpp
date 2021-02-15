#include <iostream>
#include <string>

int main()
{
  std::string n_m_str;

  getline (std::cin, n_m_str);
  
  auto space_indx = n_m_str.find(' ');
  
  auto N = stoi(n_m_str.substr(0, space_indx));
  auto M = stoi(n_m_str.substr(space_indx+1));

  std::cout << N * (M+1);   
}

