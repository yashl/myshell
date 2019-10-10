#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#define INPUT_LENGTH 1000

using namespace std;

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

int main()
{
  char *username = getenv("HOME");
  char *directory = strcat(username, "/.myshell/");
  mkdir(directory, 0777);

  bool alive = 1;

  while(alive)
  {
    printf("Welcome to MyShell! \n");
    
    vector<string> commands = parser();
  }
  
  exit(0);
  return 0;
}

