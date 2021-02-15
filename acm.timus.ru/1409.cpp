#include <iostream>
#include <string>

int main()
{
  std::string numbers;
  getline (std::cin, numbers);
  
  auto space_indx = numbers.find(' ');
  auto garry_s = numbers.substr(0, space_indx);
  auto larry_s = numbers.substr(space_indx+1); 
  auto garry_n = std::stoi(garry_s);
  auto larry_n = std::stoi(larry_s);
  auto total = garry_n + larry_n - 1;
  std::cout << total - garry_n << " " << total - larry_n;
}