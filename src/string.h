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
typedef struct StringStruct {
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
 *  Initializes String from char*
 *
 *  @ingroup String
 */
String* str_init_const(char* src);

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
 *  Concatenates 2 Strings
 *
 *  @param[in,out] dest = dest + src
 *  @param[in] string to be appended to src
 *
 *  @ingroup String
 */
void str_concat(String* dest, String* src);

/**
 *  Concatenates String and char*
 *
 *  @param[in,out] dest = dest + src
 *  @param[in] c-string to be appended to src
 *
 *  @ingroup String
 */
void str_concat_const(String* dest, char* src);

/**
 *  Copy the content of `src` to `dest`
 *
 *  @param[in,out] dest = src
 *  @param[in] string to copied to dest
 *
 *  @ingroup String
 */
void str_copy_string(String* dest, String* src);

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

/**
 *  Function get string from integer
 *
 *  @return string
 *  @ingroup String
 */
void int_to_string(String* s, int i);

/**
 *  Function get string from double
 *
 *  @return string
 *  @ingroup String
 */
void double_to_string(String* s, double i);


/**
 *  Function search substring in string
 *
 *  @return substring
 *  @ingroup String
 */
String* substr(String* s, int i, int n);

#endif
