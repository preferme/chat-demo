/*
 * wstring.c
 *
 *  Created on: 2023年3月4日
 *      Author: hou-lei
 */
#include "../include/wstring.h"
#include "../include/memory.h"


wstring * make_wstring(wchar_t * wcs) {
	if(wcs == NULL) {
		return NULL;
	}
	int length = wcslen(wcs);
	wstring * str = (wstring *)chat_alloc(sizeof(wstring));
	if(str == NULL) {
		return NULL;
	}
	str->length = length;
	str->value = (wchar_t*)chat_alloc(sizeof(wchar_t)*length);
	if(str->value) {
		return NULL;
	}
	wcscpy(str->value, wcs);
	return str;
}

void free_wstring(wstring * str) {
	if(str != NULL) {
		if(str->value != NULL) {
			chat_free(str->value);
		}
		chat_free(str);
	}
}
