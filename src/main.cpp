#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <unistd.h>

int main()
{
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::vector<std::string> builtins = {
      "echo",
      "cd",
      "pwd",
      "export",
      "unset",
      "type",
      "exit",
      "env",
      "alias",
      "unalias",
      "history",
      "help"};

  while (true)
  {
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);

    std::string cmd;
    std::string args;
    size_t space_pos = input.find(' ');
    if (space_pos != std::string::npos)
    {
      cmd = input.substr(0, space_pos);
      args = input.substr(space_pos + 1);
    }
    else
    {
      cmd = input;
      args = "";
    }

    if (cmd == "exit")
    {
      break;
    }
    else if (cmd == "echo")
    {
      std::cout << args << "\n";
    }
    else if (cmd == "type")
    {
      if (std::find(builtins.begin(), builtins.end(), args) != builtins.end()) // niepewna linijka sprawdz potem
      {
        std::cout << args << " is a shell builtin\n";
      }

      else // TYPE arg is not shell builtin, check if is avaiable in path
      {
        std::vector<std::string> paths;
        const char *pathvar_env = std::getenv("PATH");
        if (pathvar_env != nullptr)
        {
          std::string pathvar(pathvar_env); // po prostu deklaracja tego jako string bo wczesniej byl char
          std::stringstream ss(pathvar);    // czyta z pathvar strumien, przeksztalca do odczytu dla getline
          std::string token;                // token jest nadpisywany ścieżką do pojawienia sie delimitera, potem od nowa
          char delimiter = ':';
          bool found = false;

          while (std::getline(ss, token, delimiter))
          {
            paths.push_back(token);
          }

          for (const std::string &dir : paths) // petla for taka jak w pythonie jest np for path in paths
          {
            std::string entirepath = dir + "/" + args;  // zapisuje cala sciezke dodajac na jej koniec argument (podany)
            int res = access(entirepath.c_str(), X_OK); // funkcja z C, c_str() konwertuje to zeby bylo dla odczytu dla funkcji z C, X_OK - sprawdz czy ma prawo wykonac (wiele takich jest w <unistd.h>)
            if (res == 0)                               // access daje 0 lub -1, 0 oznacza ze znaelziono dostep
            {
              std::cout << args << " is " << entirepath << "\n";
              found = true;
              break;
            }
          }
          if (!found)
          {
            std::cout << args << ": not found\n";
          }
        }
      }
    }
    else // RUNNING EXTERNAL PROGRAM
    {
      std::cout << cmd << ": command not found\n";
    }
  }
}