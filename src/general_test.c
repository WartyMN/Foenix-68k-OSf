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



void test_setup(void)	// this is called EVERY test
{
// 	foo = 7;
// 	bar = 4;
// 	
}


void test_teardown(void)	// this is called EVERY test
{

}



// **** unit tests

MU_TEST(misc_test)
{
//MB: 2024-12-27: this call appears to be deadly. 
// 	// "test" get char
// 	uint8_t	the_char;
// 	uint8_t	i;
// 	char	user_string[10] = "0123456789";
// 	char*	the_user_string = user_string;
// 	
// 	for (i=0; i < 10; i++)
// 	{
// 		the_char = General_GetChar();
// 		user_string[i] = the_char;
// 	}
// 	
// 	printf("get_char: user entered string '%s' \n", the_user_string);


	printf("delaying %u seconds... \n", 1);
	General_DelaySeconds(1);
	printf("delay over. \n");
	
	
	// big/little endian swap
	uint32_t	before_swap;
	uint32_t	after_swap;
	uint32_t	re_swapped;
	
	before_swap = 0x12345678;	// 305419896
	after_swap = 0;
	re_swapped = 0;
	
	after_swap = General_ByteSwapLong(before_swap);
	re_swapped = General_ByteSwapLong(after_swap);
	
	mu_assert_int_eq(0x12345678, before_swap);
	mu_assert_int_eq(0x78563412, after_swap);	// 2018915346
	mu_assert_int_eq(0x12345678, re_swapped);


	// random number generator
	uint16_t	the_random_value;
	uint16_t	the_range;

	printf("the_random_value: testing 10 numbers \n");
	
	for (the_range = 0; the_range < 50; the_range += 5)
	{
		the_random_value = General_GetRandom(the_range);
		printf("%u ", the_random_value);
	}
	
	printf("done \n");
}


MU_TEST(number_string_test)
{

}


MU_TEST(rect_test)
{
}


MU_TEST(filepath_manipulation_test)
{
	char	string_test_buff1[32] = "big deal.txt";
	char	string_test_buff2[32] = "no extension filename";
	char	string_test_buff3[32] = "a";
	char	string_test_buff4[32];
	char	string_test_buff5[4];	
	char	string_test_buff6[32] = "/cd/_sys/config/sound.ini";
	char	string_test_buff7[32] = "_sys/config/sound.ini";
	char	string_test_buff8[32] = "sound.ini";
	char	string_test_buff9[32];
	char*	norm_filename = string_test_buff1;
	char*	no_ext_filename = string_test_buff2;
	char*	tiny_filename = string_test_buff3;
	char*	the_name_part = string_test_buff4;
	char*	the_extension = string_test_buff5;
	char*	pathtype1 = string_test_buff6;
	char*	pathtype2 = string_test_buff7;
	char*	filenameonlypath = string_test_buff8;
	char*	the_path_part = string_test_buff9;
	bool	success;
	
	//! Extract core part of the file name, not including the extension
	//! @param	the_file_name - the file name to extract an extension from
	//! @param	the_core_part - a pre-allocated buffer that will contain the pre-extension part of the file name, if any is detected. Must be large enough to hold the entire file name! No bounds checking is done. If no extension is found, this will contain an empty string.
	//! @return	Returns false if no file extension found (because what is "core" part without an extension)
	success = General_ExtractCoreFilename(norm_filename, the_name_part);
	mu_check( success == true);
	mu_assert_string_eq( "big deal", the_name_part );

	success = General_ExtractCoreFilename(no_ext_filename, the_name_part);
	mu_check( success == false);
	mu_assert_string_eq( "", the_name_part );

	success = General_ExtractCoreFilename(tiny_filename, the_name_part);
	mu_check( success == false);
	mu_assert_string_eq( "", the_name_part );
	

	//! Extract file extension into the passed char pointer, as new lowercased string pointer, if any found.
	//! @param	the_file_name - the file name to extract an extension from
	//! @param	the_extension - a pre-allocated buffer that will contain the extension, if any is detected. Must be large enough to hold the extension! No bounds checking is done. 
	//! @return	Returns false if no file extension found.
	success = General_ExtractFileExtensionFromFilename(norm_filename, the_extension);
	mu_check( success == true);
	mu_assert_string_eq( "txt", the_extension );

	success = General_ExtractFileExtensionFromFilename(no_ext_filename, the_extension);
	mu_check( success == false);
	mu_assert_string_eq( "", the_extension );

	success = General_ExtractFileExtensionFromFilename(tiny_filename, the_extension);
	mu_check( success == false);
	mu_assert_string_eq( "", the_extension );
	

	// return everything to the left of the filename in a path. 
	// amigaDOS compatibility function
	the_path_part = General_PathPart(pathtype1);
	mu_assert_string_eq( "/sound.ini", the_path_part );
	printf("path part test 1: '%s' \n", the_path_part);

	the_path_part = General_PathPart(pathtype2);
	mu_assert_string_eq( "/sound.ini", the_path_part );
	printf("path part test 2: '%s' \n", the_path_part);

	the_path_part = General_PathPart(filenameonlypath);
	mu_assert_string_eq( "sound.ini", the_path_part );
	printf("path part test 3: '%s' \n", the_path_part);
	
}


MU_TEST(string_manipulation_test)
{
	char	string_test_buff1[32];
	char	string_test_buff2[32];
	char	string_test_buff3[9];	
	char*	pstring_test_buff3 = string_test_buff3;
	char*	copy_of_string_full;
	uint16_t	orig_len;
	uint16_t	copy_len;
	uint16_t	copy_len2;
	
	char* the_string = "CONVERTING TO LC 123$%^";

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
	
	// test string copy without allocation
	//! @return	Returns the length of the source string, or -1 on any error condition
	General_Strlcpy(string_test_buff1, copy_of_string_full, 32);
	mu_assert_string_eq(string_test_buff1, copy_of_string_full);
	
	// test string copy without allocation - limited # of chars
	General_Strlcpy(string_test_buff2, copy_of_string_full, 5);
	copy_len2 = strlen(string_test_buff2);
	mu_assert(( copy_len2 == 4 ), "copied string was expected to be 4 chars long, but is not");
	
	// test measurement of string len
	//! @return	Returns strlen(the_string), if that is less than max_len, or max_len if there is no null terminating ('\0') among the first max_len characters pointed to by the_string.
	int16_t	len_full = General_Strnlen(the_string, MAX_STRING_COMP_LEN);
	int16_t	len_short = General_Strnlen(the_string, 3);
	mu_assert_int_eq(orig_len, len_full);
	mu_assert_int_eq(len_short, 3);
	
	// test strlcat
	// should append x to y, but only for as many chars as passed, - 1 for a terminator
	//! @return	Returns the length of the attempted concatenated string: initial length of dst plus the length of src.
	string_test_buff1[0] = 0;
	uint16_t cat_len = General_Strlcat(string_test_buff3, "abc", MAX_STRING_COMP_LEN);
	printf("after 1st cat: '%s' \n", string_test_buff3);
	mu_assert_string_eq("abc", pstring_test_buff3);
	uint16_t cat_len2 = General_Strlcat(string_test_buff3, "def", 5); // should now hold abcd\0
	printf("after 2nd cat: '%s' \n", pstring_test_buff3);
	mu_assert_string_eq("abcd", string_test_buff3);
	mu_assert_int_eq(3, cat_len);
	mu_assert_int_eq(6, cat_len2);
	mu_assert_int_eq(4, strlen(string_test_buff3));
	
	// test string to lower (also tests char to lower)
	General_StrToLower(the_string);
	mu_assert_string_eq(the_string, "converting to lc 123$%^");

	
	// test case sensitive string comparison
	//! Makes a case sensitive comparison of the specified number of characters of the two passed strings
	//! @return	Returns 0 if the strings are equivalent (at least up to max_len). Returns a negative or positive if the strings are different.
	uint16_t strncomp1 = General_Strncmp("ABC123", "abc123", MAX_STRING_COMP_LEN);
	uint16_t strncomp2 = General_Strncmp("ABCdef", "ABCdef", MAX_STRING_COMP_LEN);
	uint16_t strncomp3 = General_Strncmp("ABC123", "ABCdef", 2); 	// only comparing first 2 chars, should be equal
	mu_assert(( strncomp1 != 0 ), "expected strings to be rated non-equal, but were equal");
	mu_assert_int_eq(0, strncomp2);
	mu_assert_int_eq(0, strncomp3);

	
	// test case INsensitive string comparison
	//! Makes a case insensitive comparison of the specified number of characters of the two passed strings
	//! @return	Returns 0 if the strings are equivalent (at least up to max_len). Returns a negative or positive if the strings are different.
	uint16_t strncasecomp1 = General_Strncasecmp("ABC123", "abc123", MAX_STRING_COMP_LEN);
	uint16_t strncasecomp2 = General_Strncasecmp("ABCdef", "ABCdef", MAX_STRING_COMP_LEN);
	uint16_t strncasecomp3 = General_Strncasecmp("ABC123", "ABCdef", 2); 	// only comparing first 2 chars, should be equal
	mu_assert(( strncasecomp1 == 0 ), "expected strings to be rated equal, but were non-equal");
	mu_assert_int_eq(0, strncasecomp2);
	mu_assert_int_eq(0, strncasecomp3);
	
	free(copy_of_string_full);
}


// **** speed tests

MU_TEST(test_speed_1)
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
MU_TEST_SUITE(test_suite_speed)
{	
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(test_speed_1);
}


// unit tests
MU_TEST_SUITE(test_suite_units)
{	
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	
	MU_RUN_TEST(string_manipulation_test);
	MU_RUN_TEST(misc_test);
	MU_RUN_TEST(number_string_test);
	MU_RUN_TEST(rect_test);
	MU_RUN_TEST(filepath_manipulation_test);
}





/*****************************************************************************/
/*                        Public Function Definitions                        */
/*****************************************************************************/



int main(int argc, char* argv[])
{
	printf("**** general.c Test Suite **** \n");
	
	MU_RUN_SUITE(test_suite_units);
// 	MU_RUN_SUITE(test_suite_speed);
	MU_REPORT();

	printf("general test complete \n");

	return MU_EXIT_CODE;
}
