/*
 * StringUtils.c
 *
 *  Created on: 21.02.2021
 *      Author: Bayram
 */

#include <utils/App/StringUtils.h>

#ifdef STRINGUTILS_H_

void getNumberLength(uint16_t *len, uint16_t sz) {
	// TODO: Implement universal algorithm with math.h; floor(log10(number)) + 1) or so...
	if (sz > 99999) {
		char err[] =
				"Error, message must not exceed the length of 99999 characters (including CR+LF).\r\n";
		xprintf(err);
		*len = 6;
		return;
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

