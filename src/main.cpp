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
    else if (input.substr(0, 5) == "echo ")
    {
      std::cout << input.substr(5) << "\n";
    }
    else if (input.substr(0, 5) == "type ")
    {
      if (input.substr(5) == "echo" || input.substr(5) == "exit")
      {
        std::cout << input.substr(5) << " is a shell builtin\n";
      }
      else
      {
        std::cout << input.substr(5) << ": not found\n";
      }
    }
    else
    {
      std::cout << input << ": command not found\n";
    }
  }
}
