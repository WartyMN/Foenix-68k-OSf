#include <stdio.h>

#include "general.h"

char* the_string = "CONVERTING TO LC WITH GENERAL.C";

int main ()
{
	char	string_test_buff[10] = "123456789";
// char* the_string = "CONVERTING TO LC WITH GENERAL.C";

  printf("OS/f relaunch 0.0.8c\n");
  
  printf("%s\n",the_string);
  General_StrToLower(the_string);
  printf("%s\n",the_string);
  
//   printf("%s\n",string_test_buff);
//   General_StrToLower(string_test_buff);
//   printf("%s\n",string_test_buff);
  
  return 0;
}
