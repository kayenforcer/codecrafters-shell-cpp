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
      if (arg == "echo" || arg == "exit" || arg == "type")
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
