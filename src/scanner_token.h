/**
 *  @defgroup ScannerToken
 *  @brief Module defining ScannerToken and functions for its manipulation
 */


#ifndef SCANNER_TOKEN_H
#define SCANNER_TOKEN_H

#include <stdbool.h>
#include "string.h"

/**
 *  Enumeration of all token types used in ScannerToken
 *
 * @ingroup ScannerToken
 */
typedef enum {
    STT_EMPTY, ///< empty token

    STT_AND, ///< &&
    STT_COMMA, ///< ,
    STT_COMMENT, ///< comment (inline or block)
    STT_DIVIDE, ///< /
    STT_DOUBLE, ///< double number
    STT_EOF, /// end of file
    STT_EQUALS, ///< =
    STT_GREATER_EQUALS, ///< >=
    STT_GREATER, ///< >
    STT_IDENT, ///< identificator
    STT_INT,  ///< integer number
    STT_KEYWORD_TYPE, ///< var type keywords
    STT_KEYWORD, ///< one of keywords
    STT_LEFT_BRACE, ///< {
    STT_LEFT_PARENTHESE, ///< (
    STT_LESS_EQUALS, ///< <=
    STT_LESS, ///< <
    STT_LOGIC_EQUAL, ///< ==
    STT_LOGIC_NOT_EQUAL, ///< !=
    STT_MINUS_EQUAL, /// -=
    STT_MINUS, ///< `-`
    STT_MULTIPLY, ///< `*`
    STT_NEGATE, /// !
    STT_OR, ///< ||
    STT_PLUS_EQUAL, /// +=
    STT_PLUS,   ///< `+`
    STT_RIGHT_BRACE, ///< }
    STT_RIGHT_PARENTHESE, ///< )
    STT_SEMICOLON, ///< ;
    STT_STRING, ///< string
    // STT_LEFT_CHEVRON, ///< <
    // STT_RIGHT_CHEVRON, ///< >
} ScannerTokenType;

/**
 *  Enumeration of all keywords
 *
 * @ingroup ScannerToken
 */
typedef enum {
    KW_BOOLEAN, ///< boolean
    KW_BREAK, ///< break
    KW_CLASS, ///< class
    KW_CONTINUE, ///< continue
    KW_DO, ///< do
    KW_DOUBLE, ///< double
    KW_ELSE, ///< else
    KW_FALSE, ///< false
    KW_FOR, ///< for
    KW_IF, ///< if
    KW_INT, ///< int
    KW_RETURN, ///< return
    KW_STATIC, ///< static
    KW_STRING, ///< string
    KW_TRUE, ///< true
    KW_VOID, ///< void
    KW_WHILE ///< while
} KeywordType;


/**
*  Struct used to save class and member
*
*  @ingroup ScannerToken
*/
typedef struct {
    String* class;
    String* name;
} Ident;


/**
*  Union used to save various data for the ScannerToken
*
*  @ingroup ScannerToken
*/
typedef union  {
    //TODO: add some data fields
    int i;
    double d;
    String* str;
    Ident* id;
    KeywordType keyword_type;
} ScannerTokenData;


/**
 *  Structure defining token returned from scanner
 *
 *  @ingroup ScannerToken
 */
typedef struct {
    ScannerTokenType type; ///< type of the ScannerToken defined in #ScannerTokenType
    ScannerTokenData* data; ///< data for the ScannerToken
} ScannerToken;

/**
 *  Initializes ScannerToken by allocating memory and settings default values
 *
 *  @ingroup ScannerToken
 */
ScannerToken* token_init();

/**
 *  Frees token memory
 *
 *  @ingroup ScannerToken
 */
void token_delete (ScannerToken *);

/**
 *  Returns string which corresponds to the token type
 *
 *  Useful for debugging
 *
 *  @ingroup ScannerToken
 */
char* token_to_string(ScannerToken *);


#endif
