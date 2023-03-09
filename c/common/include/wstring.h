/*
 * wstring.h
 *
 *  Created on: 2023年3月4日
 *      Author: hou-lei
 */

#ifndef __WSTRING_H_
#define __WSTRING_H_

#include <wchar.h>

typedef struct wstring {
	int length;
	wchar_t *value;
} wstring;


wstring * make_wstring(wchar_t * value);

void free_wstring(wstring * wstr);


#endif /* __WSTRING_H_ */
