#include <stdio.h>

#include "general.h"
#include "debug.h"

//char* the_string = "CONVERTING TO LC WITH GENERAL.C";

int main ()
{
	char	string_test_buff[10] = "123456789";
	char*	copy_of_string;
	
	char* the_string = "CONVERTING TO LC WITH GENERAL.C";

	printf("OS/f relaunch 0.0.11\n");

	//! @return	a copy of the source string to max_len, or NULL on any error condition
	copy_of_string = General_StrlcpyWithAlloc(the_string, MAX_STRING_COMP_LEN);
  
	printf("orig before lowering: %s\n",the_string);
	printf("copy: %s\n",copy_of_string);
	General_StrToLower(the_string);
	printf("orig after lowering: %s\n",the_string);
	printf("copy: %s\n",copy_of_string);
  
//   printf("%s\n",string_test_buff);
//   General_StrToLower(string_test_buff);
//   printf("%s\n",string_test_buff);
	
	//! @return	Returns the length of the source string, or -1 on any error condition
	General_Strlcpy(string_test_buff, copy_of_string, 5);
	printf("unalloc'ed copy up to 5 chars: %s\n",string_test_buff);

	DEBUG_OUT(("%s %d: testing DEBUG_OUT", __func__, __LINE__));
	
  return 0;
}
