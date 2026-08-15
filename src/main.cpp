#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <unistd.h>
#include <filesystem>
#include <sys/wait.h>
namesapce fs = std::filesystem;

std::string pathWork(const std::string &cmd_name)
{
  std::vector<std::string> paths;
  const char *pathvar_env = std::getenv("PATH");
  if (pathvar_env == nullptr)
  {
    return "";
  }
  std::string pathvar(pathvar_env); // po prostu deklaracja tego jako string bo wczesniej byl char
  std::stringstream ss(pathvar);    // czyta z pathvar strumien, przeksztalca do odczytu dla getline
  std::string token;                // token jest nadpisywany ścieżką do pojawienia sie delimitera, potem od nowa
  char delimiter = ':';

  while (std::getline(ss, token, delimiter))
  {
    paths.push_back(token);
  }

  for (const std::string &dir : paths) // petla for taka jak w pythonie jest np for path in paths
  {
    std::string entirepath = dir + "/" + cmd_name; // zapisuje cala sciezke dodajac na jej koniec argument (podany)
    int res = access(entirepath.c_str(), X_OK);    // funkcja z C, c_str() konwertuje to zeby bylo dla odczytu dla funkcji z C, X_OK - sprawdz czy ma prawo wykonac (wiele takich jest w <unistd.h>)
    if (res == 0)                                  // access daje 0 lub -1, 0 oznacza ze znaelziono dostep
    {
      return entirepath;
    }
  }
  return "";
}

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
        if (pathWork(args) != "")
        {
          std::cout << args << " is " << pathWork(args) << "\n";
        }
        else
        {
          std::cout << args << ": not found\n";
        }
      }
    }
    else if (cmd == "pwd") {
      std::cout<< fs::current_path().string() << "\n";
    }
    else // RUNNING EXTERNAL PROGRAM
    {
      std::vector<std::string> argus; // VECTOR THAT CONTAINS SINGLE ARGUMENTS GIVEN IN INPUT
      std::string arg;
      std::stringstream st(args); // SPLITTING ARGS STRING INTO SINGLE ARG BY WHITESPACES
      while (st >> arg)
      {
        argus.push_back(arg);
      }

      std::string cmd_path = pathWork(cmd);
      if (cmd_path != "")
      {
        // RUNNING GIVEN COMMAND IF ITS ACCESSIBLE

        std::vector<const char *> chary;    // ARRAY/VECTOR FOR METHOD THAT REQUIREST THIS (FROM C)
        chary.push_back(cmd.c_str());       // FIRSTLY WE NEED TO PUSH BACK OUR COMMAND (AS C STRING)
        for (const std::string &ar : argus) // THEN WE ARE PUSHING BACK ARGUMENTS FOR THIS COMMAND
        {
          chary.push_back(ar.c_str());
        }
        chary.push_back(nullptr); // execvp() METHOD REQUIRES nullptr AT THE END OF AN ARRAY/VECTOR

        pid_t pid = fork(); // fork() IS METHOD THAT SPLITS CURRENT PROCESS INTO TWO IDENTICAL COPIES - PARENT AND CHILD
        if (pid == 0)       // IF pid CODE IS 0 CODE INSIDE CHILDS PROCESS RUNS
        {
          // child
          execvp(cmd_path.c_str(), const_cast<char *const *>(chary.data())); // execvp() METHOD THAT RUNS PROGRAM INSIDE RUNNING PROGRAM, WE NEED TO PASS cmd AND ARRAY THAT WE WORKED ON ABOVE
        }
        else if (pid > 0) // IF pid IS GREATER THAN 0 CODE INSIDE PARENTS PROCESS RUNS
        {
          // parent
          int status;
          int result = waitpid(pid, &status, 0); // waitpid() METHOD STOPS PARENTS PROCESS UNTIL CHILD PROCESS ISN'T FINISHED
        }
        else
        {
          perror("fork failed"); // IF pid IS BELOW 0 THEN SOMETHING WENT WRONG
        }
      }
      else
      {
        std::cout << cmd << ": command not found\n";
      }
    }
  }
}
