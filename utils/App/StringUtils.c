/*
 * StringUtils.c
 *
 *  Created on: 21.02.2021
 *      Author: Bayram
 */

#include <utils/App/StringUtils.h>

#ifdef STRINGUTILS_H_

void getNumberLength(uint32_t *len, uint32_t sz) {
	// TODO: Implement universal algorithm with math.h; floor(log10(number)) + 1) or so...
	if (sz > 99999999999) {
		char err[] =
				"Error, message must not exceed the length of 99999 characters (including CR+LF).\r\n";
		xprintf(err);
		*len = 12;
		return;
	} else if (sz > 9999999999) {
		*len = 11;
	} else if (sz > 999999999) {
		*len = 10;
	} else if (sz > 99999999) {
		*len = 9;
	} else if (sz > 9999999) {
		*len = 8;
	} else if (sz > 999999) {
		*len = 7;
	} else if (sz > 99999) {
		*len = 6;
	} else if (sz > 9999) {
		*len = 5;
	} else if (sz > 999) {
		*len = 4;
	} else if (sz > 99) {
		*len = 3;
	} else if (sz > 9) {
		*len = 2;
	} else {
		*len = 1;
	}
}

#endif

