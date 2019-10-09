#include <sys/stat.h>
#include <iostream>
#include <string>
#include <vector>

#define INPUT_LENGTH 1000

using namespace std;

void parser()
{
  char input[INPUT_LENGTH];
  scanf("%s", input);
  
  vector<string> v; 
  char* temp = strtok(input, ";");
  while(temp != NULL)
  {
    v.push_back(temp);
    temp = strtok(NULL, ";"); 
  }

  printf("%d", v.size());
  printf("%c", v[1]);
}

int main()
{
  char *username = getenv("HOME");
  char *directory = strcat(username, "/.myshell/");
  mkdir(directory, 0777);

  parser();
  return 0;
}

