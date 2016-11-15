#include "bvs.h"


TableUzol tabsym;


void tabsyminit(void) {
    BTS_Init(&tabsym);
}


void tabsymdispose(void) {
    BTS_Dispose(&tabsym);
}


tBTSS *TSvlozSymbol(tBTSS symbol) {
  TableUzol uzol;
  uzol = BTS_Insert(&tabsym, symbol.name, symbol);

  if (uzol) {
    return &(uzol->data);
  }

  return NULL;
}

tBTSS *TSvlozBool(TableName name, bool data) {
    tBTSS symbol;

    symbol.name = name;
    symbol.typ = T_BOOL;
    symbol.value.b = data;

    return TSvlozSymbol(symbol);
}


tBTSS *TSvlozDouble(TableName name, double data) {
    tBTSS symbol;

    symbol.name = name;
    symbol.typ = T_DOUBLE;
    symbol.value.d = data;

    return TSvlozSymbol(symbol);
}

tBTSS *TSvlozString(TableName name, char *string) {
    tBTSS symbol;

    symbol.name = name;
    symbol.typ = T_STRING;
    symbol.value.s = string;

    return TSvlozSymbol(symbol);
}

void TSinitSymbol(tBTSS *symbol) {
  if (symbol != NULL) {
    symbol->name = NULL;
    symbol->typ = T_NIL;
  }
}

TableUzol Readsymbol(TableName name) {
  TableUzol uzol;

  if (tabsym != NULL) {
    uzol = BTS_Search(tabsym, name);

    if (uzol != NULL) {
          return uzol;
    }
  }

  return NULL;
}

void BTS_Init (TableUzol * node) {

*node = NULL;

}

TableUzol BTS_Search (TableUzol node, TableName key) {
  if ( node != NULL ) {
  	int cmp = strcmp(node->key, key);

    if (cmp == 0) {
      return node;
    }

    else if ( cmp > 0 ) { // je mensi hladame v lavo
  		return BTS_Search(node->rptr, key);
  	}
  	else { // hladame v pravo
  		return BTS_Search(node->lptr, key);
  	}
  }
  return NULL;
}

TableUzol BTS_Insert (TableUzol *node, TableName key, tBTSS symbol) {
  if (*node == NULL) {

		if ((*node = malloc(sizeof(struct tBTSuzol))) != NULL ) {
			(*node)->key = key; // novy kluc
      (*node)->data = symbol; // vlozenie obsahu
			(*node)->lptr = NULL;
			(*node)->rptr = NULL;
		}
  }
  else { // ak je neprazdni
    int cmp = strcmp(key, (*node)->key);

    if(cmp > 0) {  // je vacsi nez aktualny , vkladame do lava
      return BTS_Insert(&((*node)->rptr), key, symbol);
    }
    else if (cmp < 0) { // je mensi nez aktualny , vkladame do lava
      return BTS_Insert(&((*node)->lptr), key, symbol);
    }
    else {
      (*node)->data = symbol;
    }
  }
  return *node;
}


void BTS_Dispose (TableUzol *node) {

  if ( *node != NULL ) {
		BTS_Dispose(&((*node)->rptr)); //dispose pravych uzlov
		BTS_Dispose(&((*node)->lptr)); //dispose lavych uzlov
		free (*node);
		*node = NULL; // povodny stav
	}

}
