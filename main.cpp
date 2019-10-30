/**
 *Name: Yash Lekhwani
 *Date: October 11, 2019
 *My Shell written for COMS 352
 */

#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <sstream>
#include <fstream>

#define INPUT_LENGTH 1000

using namespace std;

//used to keep track of current directory
char *directory;

vector<string> parser(string input, char delim);
int commandhandle(vector<string> commands);
int inputfile_check(string input);

/**
 *Main function: initializes the program
 *@param argc = argument count of the binary
 *@param argv = each argument is in the parameter
 */
int main(int argc, char **argv)
{
  char *username = getenv("HOME");
  directory = strcat(username, "/.myshell/");
  mkdir(directory, 0777);
  chdir(directory);

  printf("Welcome to MyShell! \n");
  vector<string> commands; 
 
  if(argv[1] != NULL){ 
    if(inputfile_check(argv[1]) == 0)
    {
      ifstream infile(argv[1]);
    
      string line;
      while(getline(infile, line))
      { 
        char delim = ';';
        commands = parser(line, delim);
        int lol = commandhandle(commands);
      }
    }
  }
  else
  {
    string input;
    int alive = 0;
    char delim;
    while(alive != -1)
    {
      printf("myshell>");
      getline(cin, input);

      if(input.find('&') != string::npos)
      {
        delim = '&';
        vector<string> final_input = parser(input, delim);
        delim = ';';
        commands = parser(final_input[0], delim);
        alive = commandhandle(commands);
        alive = -1;
      }
      else
      {
        delim = ';';
        commands = parser(input, delim);
        alive = commandhandle(commands);
      }
    }
  }

  exit(0);
  return 0;
}

/**
 *Checks if the input string has a file as an argument
 *@param input = input string to check
 *return : returns 1 if not found, and return 0 if found
 */
int inputfile_check(string input)
{
  int found = 1;
  if(input.find(".txt") != string::npos)
  {
    found = 0;
  }
  return found;
}

/**
 *Parses function based on delim and returns string tokens in the vector
 *@param input = input string to parse
 *@param delim = delimter based on which string has to be tokenized
 *return : returns individual strings in the vector
 */
vector<string> parser(string input, char delim)
{
  vector<string> v;
  string temp_word, command;

  int first = input.find_first_not_of(' ');
  int right = input.find_last_not_of(' ');
  command = input.substr(first, right - first + 1);
 
  istringstream input_iss(command);
  while(getline(input_iss, temp_word, delim))
  {
    v.push_back(temp_word);
  } 

  return v;
}

/**
 *Handles all commands from the parameter. Also handles piping
 *@param commands = vector of commands to be executed
 *returns -1 if quit was called
 */
int commandhandle(vector<string> commands)
{
  int total_commands = commands.size();

  int i;
  string str;

  for(i = 0; i < total_commands; i++)
  {
    //Case: "date"
    if(commands[i].compare("date") == 0)
    {
      pid_t status = fork();
      switch(status)
      {
        case -1:
          break;
        case 0:
          if(execvp("date", (char *const[]){"date", NULL}) == -1)
          {
            printf("Error showing date!");
          }
        default:
          wait(&status);
          break;
      } 
    }
    
    //Case: "cal"
    if(commands[i].compare("cal") == 0)
    {
      pid_t status = fork();
      switch(status)
      {
        case -1:
          break;
        case 0:
          if(execvp("cal", (char *const[]){"cal", NULL}) == -1)
          {
            printf("Error showing calendar!");
          }
        default:
          wait(&status);
          break;
      } 
    }

    //Case: "who"
    if(commands[i].compare("who") == 0)
    {
      pid_t status = fork();
      switch(status)
      {
        case -1:
          break;
        case 0:
          if(execvp("who", (char *const[]){"who", NULL}) == -1)
          {
            printf("Error showing who!");
          }
        default:
          wait(&status);
          break;
      } 
    }
    
    //Case: "ps"
    if(commands[i].compare("ps") == 0)
    {
      pid_t status = fork();
      switch(status)
      {
        case -1:
          break;
        case 0:
          if(execvp("ps", (char *const[]){"ps", NULL}) == -1)
          {
            printf("Error showing ps!");
          }
        default:
          wait(&status);
          break;
      } 
    }

    //Case: "cd"
    if(commands[i].find("cd") == 0)
    {
      //if command has no argument
      if(commands[i].compare("cd") == 0)
      {
        printf("Enter directory please! Current directory is: %s \n", directory);
      }
      else
      {
        vector<string> temp = parser(commands[i], ' ');
        //Error check
        if(chdir(temp[1].c_str()) < 0)
        {
          printf("Error chanding directory");
        }
        //Directory change
        else
        {
          printf("Chanding directory to %s\n", temp[1].c_str());
          directory = strcat(directory, temp[1].c_str());
          directory = strcat(directory, "/");
          printf("Current directory is %s\n", directory);
        }
      }      
    }

    //Case: "clr"
    if(commands[i].compare("clr") == 0)
    {
      pid_t status = fork();
      switch(status)
      {
        case -1:
          break;
        case 0:
          if(execvp("clear", (char *const[]){"clear", NULL}) == -1)
          {
            printf("Error clearing the screen!");
          }
        default:
          wait(&status);
          break;
      }
    }

    //Case: dir<directory , ls>
    str = "dir";
    string str1 = "ls";
    if(commands[i].find(str) == 0 || commands[i].find(str1) == 0)
    {
      pid_t status = fork();
      switch(status)
      {
        case -1:
          break;
        case 0:
          char *argv[64];
          //if no argument is is given
          if(commands[i].compare("dir") == 0 || commands[i].compare("ls") == 0)
          {
            argv[0] = "ls";
            argv[1] = NULL;
          }
          else
          {
            char *par;
            argv[0] = "ls";
            vector<string> temp = parser(commands[i], ' ');
            for(int i = 1; i<temp.size(); i++)
            {
              //change string to char pointer
              char *c = new char [temp[i].size() + 1];
              std::copy(temp[i].begin(), temp[i].end(), c);
              c[temp[i].size()] = '\0';
              //populate argv for execvp
              argv[i] = c;
            } 
          }
          
          //error check
          if(execvp("ls", argv) == -1)
          {
            printf("Error listing contents of the directory \n");
          }
        default:
          //wait until child process dies
          wait(&status);
          break;
      }
    }

    //Case: environ
    if(commands[i].compare("environ") == 0)
    {
      pid_t status = fork();
      switch(status)
      {
        case -1:
          break;
        case 0:
          if(execvp("env", (char *const[]){"env", NULL}) == -1)
          {
            printf("Error displaying environment variables");
          }
        default:
          wait(&status);
          break;
      } 
    }

    //Case: pipes
    if(commands[i].compare("ls -l|more") == 0)
    {
      char temp[(commands[i].size() +1)];
      pid_t status = fork();
      switch(status)
      {
        case -1:
          break;
        case 0:
          strcpy(temp, commands[i].c_str());
          system(temp);
        default:
          wait(&status);
          break;
      } 
    }

    //Case: help
    if(commands[i].compare("help") == 0)
    {
      printf("This shell has a number of commands that you can call to the operating system! \n");
      printf("For more information for the use of this program, please refer to the README! \n");
    }

    //Case: time
    if(commands[i].compare("time") == 0)
    {
      pid_t status = fork();
      switch(status)
      {
        case -1:
          break;
        case 0:
          if(execvp("date", (char *const[]){"date", NULL}) == -1)
          {
            printf("Error displaying time!");
          }
        default:
          wait(&status);
          break;
      } 
    }

    //Case: pause
    if(commands[i].compare("pause") == 0)
    {
      printf("Press enter to continue.");
      while(cin.get() != '\n') {}
    }

    //Case: echo
    str = "echo";
    if(commands[i].find(str) == 0)
    {
      vector<string> temp = parser(commands[i], ' ');
      for(int i = 1; i<temp.size(); i++)
      {
        cout << temp[i] << '\n';
      } 
    }

    //Case: quit
    if(commands[i].compare("quit") == 0)
    {
      return -1;
    }
  }

  return 0;
}

