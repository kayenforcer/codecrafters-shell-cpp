#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

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
      std::string arg = input.substr(5);
      std::cout << arg << "\n";
    }
    else if (input.substr(0, 5) == "type ")
    {
      std::string arg = input.substr(5);

      if (arg == "echo" || arg == "exit" || arg == "type")
      {
        std::cout << arg << " is a shell builtin\n";
      }

      else // tutaj robimy
      {
        std::vector<std::string> paths;
        const char *pathvar_env = std::getenv("PATH");
        if (pathvar_env != nullptr)
        {
          std::string pathvar(pathvar_env);
          std::stringstream ss(pathvar);
          std::string token;
          char delimiter = ':';

          while (std::getline(ss, token, delimiter))
          {
            paths.push_back(token);
          }
        }

        std::cout << arg << ": not found\n";
      }
    }
    else
    {
      std::cout << input << ": command not found\n";
    }
  }
}
