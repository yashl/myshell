#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>

#define INPUT_LENGTH 1000

using namespace std;

vector<string> parser();
int commandhandle(vector<string> commands);

int main()
{
  char *username = getenv("HOME");
  char *directory = strcat(username, "/.myshell/");
  mkdir(directory, 0777);

  printf("Welcome to MyShell! \n");
  
  int alive = 0;
  while(alive != -1)
  {
    printf("Pwd: %s >", directory); 
    vector<string> commands = parser();
    alive = commandhandle(commands);
  }
  
  exit(0);
  return 0;
}

vector<string> parser()
{
  char input[INPUT_LENGTH];
  scanf("%s", input);
  
  vector<string> v;
  char *temp;
  temp  = strtok(input, ";");
  while(temp != NULL)
  {
    v.push_back(temp);
    temp = strtok(NULL, ";"); 
  }

  int size = v.size();
  //printf("%d\n", size);
  for(int i =0; i<size; i++)
  {
    //cout << v[i] << endl;
  }
  return v;
}

int commandhandle(vector<string> commands)
{
  int total_commands = commands.size();

  int i;
  for(i = 0; i < total_commands; i++)
  {
    //Case: "cd"
    if(commands[i].compare("cd") == 0)
    {
      
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

    //Case: dir<directory>
    string str = "dir";
    if(commands[i].find(str) != string::npos)
    {
      pid_t status = fork();
      switch(status)
      {
        case -1:
          break;
        case 0:
          char *argv[2];
          if(commands[i].compare("dir") == 0)
          {
            argv[0] = "ls";
            argv[1] = NULL;
          }
          else
          {
            char *par;
            char temp[commands[i].size() + 1];
            strcpy(temp, commands[i].c_str());
            strtok(temp, " ");
            par = strtok(NULL, " ");
            argv[0] = "ls";
            argv[1] = par;
          }

          if(execvp("ls", argv) == -1)
          {
            printf("Error listing contents of the directory");
          }
        default:
          wait(&status);
          break;
      }
    }

    //Case:
  }
  return 0;
}

