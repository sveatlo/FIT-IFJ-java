#include "bvs.h"


tBVSuzolptr tabsym;


void tabsyminit(void) {
    BVSFunction_Init(&tabsym);
}


void tabsymdispose(void) {
    BVSFunction_Dispose(&tabsym);
}


tBVSS *InsertSymbol(tBVSS symbol) {
  tBVSuzolptr uzol;
  uzol = BVSFunktion_Insert(&tabsym, symbol.name, symbol);

  if (uzol) {
    return &(uzol->data);
  }

  return NULL;
}

tBVSS *TSvlozBool(tName name, bool data) {
    tBVSS symbol;

    symbol.name = name;
    symbol.typ = tBool;
    symbol.value.b = data;

    return InsertSymbol(symbol);
}


tBVSS *TSvlozDouble(tName name, double data) {
    tBVSS symbol;

    symbol.name = name;
    symbol.typ = tDouble;
    symbol.value.d = data;

    return InsertSymbol(symbol);
}

tBVSS *TSvlozString(tName name, char *string) {
    tBVSS symbol;

    symbol.name = name;
    symbol.typ = tString;
    symbol.value.s = string;

    return InsertSymbol(symbol);
}

tBVSuzolptr Readsymbol(tName name) {
  tBVSuzolptr uzol;

  if (tabsym != NULL) {
    uzol = BVSFunction_Search(tabsym, name);

    if (uzol != NULL) {
          return uzol;
    }
  }

  return NULL;
}

void BVSFunction_Init (tBVSuzolptr * node) {

*node = NULL;

}

tBVSuzolptr BVSFunction_Search (tBVSuzolptr node, tName key) {
  if ( node != NULL ) {
  	int cmp = strcmp(node->key, key);

    if (cmp == 0) {
      return node;
    }

    else if ( cmp > 0 ) { // je mensi hladame v lavo
  		return BVSFunction_Search(node->RPtr, key);
  	}
  	else { // hladame v pravo
  		return BVSFunction_Search(node->LPtr, key);
  	}
  }
  return NULL;
}

tBVSuzolptr BVSFunktion_Insert (tBVSuzolptr *node, tName key, tBVSS symbol) {
  if (*node == NULL) {

		if ((*node = malloc(sizeof(struct tBVSuzol))) != NULL ) {
			(*node)->key = key;
      (*node)->data = symbol;
			(*node)->LPtr = NULL;
			(*node)->RPtr = NULL;
		}
  }
  else { // ak je neprazdni
    int cmp = strcmp(key, (*node)->key);

    if(cmp > 0) {
      return BVSFunktion_Insert(&((*node)->RPtr), key, symbol);
    }
    else if (cmp < 0) {
      return BVSFunktion_Insert(&((*node)->LPtr), key, symbol);
    }
    else {
      (*node)->data = symbol;
    }
  }
  return *node;
}


void BVSFunction_Dispose (tBVSuzolptr *node) {

  if ( *node != NULL ) {
		BVSFunction_Dispose(&((*node)->RPtr)); //dispose pravych uzlov
		BVSFunction_Dispose(&((*node)->LPtr)); //dispose lavych uzlov
		free (*node);
		*node = NULL; // povodny stav
	}

}
