/*
 * StringUtils.c
 *
 *  Created on: 21.02.2021
 *      Author: Bayram
 */

#include <utils/App/StringUtils.h>
#include <math.h>

#ifdef STRINGUTILS_H_

void getNumberLength(uint32_t *length, uint32_t number) {
	*length = floor(log10(number)) + 1;

//	if (sz > 99999999999) {
//		char err[] =
//				"Error, message must not exceed the length of 99999 characters (including CR+LF).\r\n";
//		xprintf(err);
//		*len = 12;
//		return;
//	} else if (sz > 9999999999) {
//		*len = 11;
//	} else if (sz > 999999999) {
//		*len = 10;
//	} else if (sz > 99999999) {
//		*len = 9;
//	} else if (sz > 9999999) {
//		*len = 8;
//	} else if (sz > 999999) {
//		*len = 7;
//	} else if (sz > 99999) {
//		*len = 6;
//	} else if (sz > 9999) {
//		*len = 5;
//	} else if (sz > 999) {
//		*len = 4;
//	} else if (sz > 99) {
//		*len = 3;
//	} else if (sz > 9) {
//		*len = 2;
//	} else {
//		*len = 1;
//	}
}

void test_getNumberLength(){
	uint32_t len  = 0;

	for (uint32_t j = 0; j<9; j++){
	    int lower = pow(10, j);
	    int upper = pow(10, j + 1);
//	     printf("index: %d with lower Bound: %d and upper Bound: %d\n", j, lower, upper);
	    for (uint32_t i = lower; i < upper; i++){
	        getNumberLength(&len, i);
	        if (len != j + 1){
//	             printf("Error with method at i = %d\n", i);
	        }
	    }
	}
}

#endif

