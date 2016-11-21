/**
 *  @defgroup String
 *  @brief Module used for advanced string manipulation
 */


#ifndef STRING_H
#define STRING_H

#define STR_INC_SIZE 8

/**
 *  Structure used for string operations
 *
 *  This structure should be always used for any string operation
 *  @ingroup String
 */
typedef struct {
    char* str;  ///< Actual pointer to the chars
    int length; ///< Real string length
    int mem_size; ///< Memory allocated for the string
} String;


/**
 *  Initializes empty string
 *
 *  Allocates memory for a new string of size <tt>n</tt>*STR_INC_SIZE bytes;
 *
 *  @return 1 for success; 0 for failure
 *  @ingroup String
 */
String* str_init_n(int n);

/**
 *  Wrapper for str_init_n
 *
 *  Calls #str_init_n with second param 1
 *
 *  @return 1 for success; 0 for failure
 *  @ingroup String
 */
String* str_init();

/**
 *  Used for all resizing operations
 *
 *  @ingroup String
 */
void _str_resize_raw(String* s, int n);

/**
 *  Frees the memory allocated for the string structure
 *  @ingroup String
 */
void str_dispose(String* s);

/**
 *  Sets the string to empty string
 *  @ingroup String
 */
void str_clear(String* s);

/**
 *  Appends char to string
 *  @ingroup String
 */
void str_append(String* s1, char c);

/**
 *  Copy the content of <tt>s1</tt> to <tt>s2</tt>
 *  @return 1 for success; 0 for failure
 *  @ingroup String
 */
void str_copy_string(String* s1, String* s2);

/**
 *  Compares two strings
 *
 *  Implementation via strcmp
 *  @return 0 for equal; value < 0 s1 is less than s2 ; value > 0 s2 is less than s1
 *  @ingroup String
 */
int str_cmp(String* s1, String* s2);

/**
 *  Compares string and char*
 *
 *  Implementation via strcmp
 *  @ingroup String
 */
int str_cmp_const(String* s1, char *s2);

/**
 *  Returns the content of string
 *
 *  @return Pointer to the string's c-string
 *  @ingroup String
 */
char* str_get_str(String* s);

/**
 *  Function used to get current length of the string
 *
 *  @return length of the string
 *  @ingroup String
 */
int str_length(String* s);

#endif
