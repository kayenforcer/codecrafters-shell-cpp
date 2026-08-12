#include <iostream>
#include <string>

int main()
{
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true)
  {
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);

    if (input == "exit")
    {
      break;
    }
    else if (input.substr(0,5) == "echo")
    {
      std::cout << input.substr(5) << "\n";
    }
    else
    {
      std::cout << input << ": command not found\n";
    }
  }
}
