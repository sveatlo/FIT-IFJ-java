/**
 * @defgroup IAL
 * @brief Module containing functions for finding substrings, sorting strings and the symbols table
 */

#ifndef IAL_H
#define IAL_H

#include "string.h"

/**
 * Searches for substring in a string
 *
 * @return -1 when not found, position when found
 * @ingroup IAL
 */
int ial_find(string s, string search);

/**
 * Sorts the string in ascending order
 *
 * @return sorted string
 * @ingroup IAL
 */
string *ial_sort(string *s);

void ial_sort_shiftdown(char *s, int last);

#endif
