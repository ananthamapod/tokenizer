/*
 * Tokenizer type.
 */

struct TokenizerT_ {
  int *delimiters;
  char *tokenString;
  char *currPos;
  char *token;
};

typedef struct TokenizerT_ TokenizerT;


/*
 * Function Headers
 */

TokenizerT *TKCreate(char *, char *);

int TKHasNextToken(TokenizerT *);

char *TKGetNextToken(TokenizerT *);

void TKDestroy(TokenizerT *);
