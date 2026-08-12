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
    std::string arg = input.substr(5);

    if (input == "exit")
    {
      break;
    }
    else if (input.substr(0, 5) == "echo ")
    {
      std::cout << arg << "\n";
    }
    else if (input.starts_with("type "))
    {
      if (input.substr(0, 5) == "echo" || input.substr(0, 5) == "exit" || input.substr(0, 5) == "type")
      {
        std::cout << arg << " is a shell builtin\n";
      }
      else
      {
        std::cout << arg << ": not found\n";
      }
    }
    else
    {
      std::cout << input << ": command not found\n";
    }
  }
}
