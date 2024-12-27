/*
 * general_test.c
 *
 *  Created on: Dec 27, 2024
 *      Author: micahbly
 */






/*****************************************************************************/
/*                                Includes                                   */
/*****************************************************************************/

// unit testing framework
#include "debug.h"
#include "minunit.h"

// project includes

// class being tested
#include "general.h"

// C includes
#include <stdbool.h>
#include <stdint.h>


// A2560 includes
#include "a2560k.h"



/*****************************************************************************/
/*                               Definitions                                 */
/*****************************************************************************/



/*****************************************************************************/
/*                               Enumerations                                */
/*****************************************************************************/



/*****************************************************************************/
/*                             Global Variables                              */
/*****************************************************************************/

//extern System*			global_system;




/*****************************************************************************/
/*                       Private Function Prototypes                         */
/*****************************************************************************/



/*****************************************************************************/
/*                       Private Function Definitions                        */
/*****************************************************************************/






/*****************************************************************************/
/*                        MinUnit Function Defintions                        */
/*****************************************************************************/



void general_test_setup(void)	// this is called EVERY test
{
// 	foo = 7;
// 	bar = 4;
// 	
}


void general_test_teardown(void)	// this is called EVERY test
{

}



// **** unit tests


MU_TEST(string_len_test)
{
	char	string_test_buff[10] = "123456789";
	char*	copy_of_string_full;
	char*	copy_of_string_5b;
	uint16_t	orig_len;
	uint16_t	copy_len;
	
	char* the_string = "CONVERTING TO LC WITH GENERAL.C";

	orig_len = strlen(the_string);

// 	printf("orig before lowering: %s\n",the_string);
// 	printf("copy: %s\n",copy_of_string);
// 	General_StrToLower(the_string);
// 	printf("orig after lowering: %s\n",the_string);
// 	printf("copy: %s\n",copy_of_string);
  
	//! @return	a copy of the source string to max_len, or NULL on any error condition
	mu_assert( (copy_of_string_full = General_StrlcpyWithAlloc(the_string, MAX_STRING_COMP_LEN) ) != NULL, "could not copy with allocate");
	copy_len = strlen(copy_of_string_full);
	mu_assert(( copy_len == orig_len ), "copied string was different length than original string according to strlen");
	
	// mu_assert_int_eq
	
	
// 	//! @return	Returns the length of the source string, or -1 on any error condition
// 	General_Strlcpy(string_test_buff, copy_of_string, 5);
// 	printf("unalloc'ed copy up to 5 chars: %s\n",string_test_buff);

	// these test pass, but visually, copying from screen A to B or vice versa doesn't work well right now
	// these reason has to do with different # of columns/rows. 
	// TODO: re-write these tests to copy to an off-screen location. have a different offscreen buffer with some preprerated text. copy that to screen. then copy original buffer back to screen. 
	// TODO: if function is necessary, device a smarter, dedicated buffer to screen copy that accounts for width of copied buffer and width of target screen. 
		
// 	mu_assert( (buffer1 = (char*)calloc(global_system->screen_[ID_CHANNEL_A]->text_mem_cols_ * global_system->screen_[ID_CHANNEL_A]->text_mem_rows_, sizeof(char)) ) != NULL, "could not alloc space for screen buffer 1");
//  	mu_assert( (buffer2 = (char*)calloc(global_system->screen_[ID_CHANNEL_A]->text_mem_cols_ * global_system->screen_[ID_CHANNEL_A]->text_mem_rows_, sizeof(char)) ) != NULL, "could not alloc space for screen buffer 2");

	
	free(copy_of_string_full);
	free(copy_of_string_5b);
}


// **** speed tests

MU_TEST(general_test_hline_speed)
{
	long start1;
	long end1;
	long start2;
	long end2;
	
	// test speed of first variant
	start1 = mu_timer_real();
	


	// speed test 1 goes here
	
	
	end1 = mu_timer_real();
	
	// test speed of 2nd variant
	start2 = mu_timer_real();
	


	// speed test 2 goes here
	
	
	end2 = mu_timer_real();
	
	printf("\nSpeed results: first routine completed in %li ticks; second in %li ticks\n", end1 - start1, end2 - start2);
}



	// speed tests
MU_TEST_SUITE(general_test_suite_speed)
{	
	MU_SUITE_CONFIGURE(&general_test_setup, &general_test_teardown);
	
// 	MU_RUN_TEST(general_test_hline_speed);
}


// unit tests
MU_TEST_SUITE(general_test_suite_units)
{	
	MU_SUITE_CONFIGURE(&general_test_setup, &general_test_teardown);
	
	MU_RUN_TEST(string_len_test);
}





/*****************************************************************************/
/*                        Public Function Definitions                        */
/*****************************************************************************/



int main(int argc, char* argv[])
{
	printf("**** general.c Test Suite **** \n");
	
	MU_RUN_SUITE(general_test_suite_units);
// 	MU_RUN_SUITE(general_test_suite_speed);
	MU_REPORT();

	printf("general test complete \n");

	return MU_EXIT_CODE;
}
