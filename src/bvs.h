#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<stdbool.h>

typedef char *tName;

typedef enum
{
  tNil,
  tBool,
  tDouble,
  tString,
} tTypSymbolu;

typedef union tSymbolValue // union pre data symbolu
{
	int intv;
	double d;
	char *s;
	bool b;
} tSymbolValue;

typedef struct tBVSS
{
	bool defined;
	tName name;
	tTypSymbolu typ; //datovy typ
  int counter;
  void * nextnode;
  tSymbolValue value;
  int jmp;
} tBVSS, *tSymbolPTR;

typedef struct tBVSuzol {
	tName key;			                                                      /* klíč */
	tBVSS data;                                           /* užitečný obsah uzlu */
	struct tBVSuzol *LPtr;                                    /* levý podstrom */
	struct tBVSuzol *RPtr;                                   /* pravý podstrom */
} *tBVSuzolptr;

extern tBVSuzolptr tabsym;

void TSinit(void);
void TSdispose(void);
tBVSS *InsertSymbol(tBVSS symbol);
tBVSS *TSvlozBool(tName name, bool data);
tBVSS *TSvlozDouble(tName name, double data);
tBVSS *TSvlozString(tName name, char *string);
tBVSuzolptr Readsymbol(tName name);
void BVSFunction_Init (tBVSuzolptr * node);
tBVSuzolptr BVSFunction_Search (tBVSuzolptr node, tName key);
tBVSuzolptr BVSFunktion_Insert (tBVSuzolptr *node, tName key, tBVSS symbol);
void BVSFunction_Dispose (tBVSuzolptr *node) ;
