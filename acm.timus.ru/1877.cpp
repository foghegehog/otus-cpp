#include <iostream>
#include <string>

int main()
{
  std::string first_str;
  std::string second_str;
  getline (std::cin, first_str);
  getline (std::cin, second_str);
  
  auto first_code = std::stoi(first_str);
  auto second_code = std::stoi(second_str);

  if ((first_code % 2 == 0) || (second_code % 2 == 1))
  {
      std::cout << "yes";   
  }
  else
  {
      std::cout << "no";
  }
}

