#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<stdbool.h>

typedef char *TableName;  /* Kluc pre binarne vyhladavanie */

typedef enum /* interne typy symbolov */
{
  T_NIL,
  T_BOOL,
  T_DOUBLE,
  T_STRING,
} TableTypSymbolu;

typedef union TableSymbolValue // union pre data symbolu
{
	int int;
	double d;
	char *s;
	bool b;
} TableSymbolValue;

typedef struct tBTSS
{
	bool defined;
	TableName name; //nazov symbolu
	TableTypSymbolu typ; //datovy typ symbolu
  int counter;
  void * nextnode; // dalsi argument funkcie
  TableSymbolValue value;
  int jmp;
} tBTSS, *TableSymbol;

typedef struct tBTSuzol {
	TableName key;			                                      /* klíč */
	tBTSS data;                                           /* užitečný obsah uzlu */
	struct tBTSuzol *lptr;                                /* levý podstrom */
	struct tBTSuzol *rptr;                                /* pravý podstrom */
} *TableUzol;                                         /* uzol binarneho stromu */

extern TableUzol tabsym;                              /* tabulka symbolov */
/**
* Inicializing table of variables
*
* @return Inicialized table of variables
* @ingroup IAL
*/
void TSinit(void);

/**
* Cancel table of variables
*
* @return Canceled table of variables
* @ingroup IAL
*/
void TSdispose(void);

/**
* Inicializing variable in decleration
*
* @ingroup IAL
*/
void TSinitSymbol(tBTSS *symbol);

/**
* Insert variable into table
*
* @return Inserted variable
* @ingroup IAL
*/
tBTSS *TSvlozSymbol(tBTSS symbol);

/**
* Insert variable into table type of Bool
*
* @return Inserted variable
* @ingroup IAL
*/
tBTSS *TSvlozBool(TableName name, bool data);

/**
* Insert variable into table type of Double
*
* @return Inserted variable
* @ingroup IAL
*/
tBTSS *TSvlozDouble(TableName name, double data);

/**
* Insert variable into table type of string
*
* @return Inserted variable
* @ingroup IAL
*/
tBTSS *TSvlozString(TableName name, char *string);

/**
* Read variable from table
*
* @return Loaded variable from table
* @ingroup IAL
*/
TableUzol Readsymbol(TableName name);

/**
* Inicializing Binary Tree
*
* @ingroup IAL
*/
void BTS_Init (TableUzol * node);

/**
* Function for searching in tree ussing key
*
* @return node When finding word is equal to key
* @ingroup IAL
*/
TableUzol BTS_Search (TableUzol node, TableName key);

/**
* Function for insert  into tree ussing key and content for insert
*
* @return node with inserted
* @ingroup IAL
*/
TableUzol BTS_Insert (TableUzol *node, TableName key, tBTSS symbol);

/**
* Function for Deleting everything in tree
*
* @ingroup IAL
*/
void BTS_Dispose (TableUzol *node) ;
