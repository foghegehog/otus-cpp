#include <iostream>
#include <string>

int main()
{
  std::string first_weights_str;
  std::string second_weights_str;
  std::string third_weights_str;
  getline (std::cin, first_weights_str);
  getline (std::cin, second_weights_str);
  getline (std::cin, third_weights_str);
  
  auto space_indx = first_weights_str.find(' ');
  auto a1 = stoi(first_weights_str.substr(0, space_indx));
  auto b1 = stoi(first_weights_str.substr(space_indx+1));
  
  space_indx = second_weights_str.find(' ');
  auto b2 = stoi(second_weights_str.substr(space_indx+1));
  
  space_indx = third_weights_str.find(' ');
  auto a3 = stoi(third_weights_str.substr(0, space_indx));

  std::cout << a1 - a3 << " " << b1 - b2;   
}
