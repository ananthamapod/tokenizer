/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
 *This function finds escaped sequences in the command line
 *inputs and converts them into the actual escaped sequence
 *characters.
 *
 *This function takes in 2 char* variables. The first argument
 *is a string and the second argument is an altered copy of the
 *first argument that has its escaped sequences represented as 1
 *character.
 *
 *This function returns a pointer to the copy string.
 */
char *escapeSwitch(char *original, char *copy) {
  int origLen = strlen(original);
  int iter = 0;
  int copyIndex = 0;
  int toggle = 0;
  for(iter = 0;iter < origLen+1;iter++) {
    if(original[iter] == '\\') {
      if(!toggle) {
        toggle = 1;
        continue;
      }
    }
    if(toggle) {
      switch(original[iter]) {
        case 'a':
          copy[copyIndex] = '\a';
          break;
        case 'b':
          copy[copyIndex] = '\b';
          break;
        case 'f':
          copy[copyIndex] = '\f';
          break;
        case 'n':
          copy[copyIndex] = '\n';
          break;
        case 'r':
          copy[copyIndex] = '\r';
          break;
        case 't':
          copy[copyIndex] = '\t';
          break;
        case 'v':
          copy[copyIndex] = '\v';
          break;
        case '\\':
          copy[copyIndex] = '\\';
          break;
        case '\'':
          copy[copyIndex] = '\'';
          break;
        case '\"':
          copy[copyIndex] = '\"';
          break;
        case '\?':
          copy[copyIndex] = '\?';
          break;
        default:
          copy[copyIndex] = original[iter];
          break;
      }
      toggle = 0;
      copyIndex++;
      continue;
    }
    copy[copyIndex] = original[iter];
    copyIndex++;
  }
  return copy;
}

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 *
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 */

TokenizerT *TKCreate(char *separators, char *ts) {
  int sepLen = strlen(separators);
  int tsLen = strlen(ts);
  char *tsCopy = 0;
  char *sepCopy = 0;
  int *delimTable = 0;
  int iter = 0;
  char *tokenBuffer = 0;
  TokenizerT *tokenizer = 0;

  sepCopy = (char *)malloc(sizeof(char)*(sepLen+1));
  if(sepCopy == NULL) {
    free(sepCopy);
    return NULL;
  }
  delimTable = (int *)calloc(128,sizeof(int));
  if(sepCopy == NULL) {
    free(sepCopy);
    return NULL;
  }
  tsCopy = (char *)malloc(sizeof(char)*(tsLen+1));
  if(tsCopy == NULL) {
    free(sepCopy);
    free(tsCopy);
    return NULL;
  }
  tokenBuffer = (char *)malloc(sizeof(char)*(51));
  if(tokenBuffer == NULL) {
    free(sepCopy);
    free(tsCopy);
    free(tokenBuffer);
    return NULL;
  }
  tokenizer = (TokenizerT *)malloc(sizeof(TokenizerT));
  if(tokenizer == NULL) {
    free(sepCopy);
    free(tsCopy);
    free(tokenBuffer);
    free(tokenizer);
    return NULL;
  }

  tsCopy = escapeSwitch(ts,tsCopy);
  sepCopy = escapeSwitch(separators,sepCopy);

  for(iter=0;iter<strlen(sepCopy);iter++) {
    delimTable[(int)sepCopy[iter]] = 1;
  }
  free(sepCopy);

  tokenizer->delimiters = delimTable;
  tokenizer->tokenString = tsCopy;
  tokenizer->currPos = tsCopy;
  tokenizer->token = tokenBuffer;
  return tokenizer;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 */

void TKDestroy(TokenizerT *tk) {
  free(tk->delimiters);
  free(tk->tokenString);
  free(tk->token);
  free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 */

char *TKGetNextToken(TokenizerT *tk) {
  if(tk == 0) {
    return 0;
  }
  char *tokenString = tk->tokenString;
  int *delimiters = tk->delimiters;
  char *first = tk->currPos;
  char *token = NULL;
  char *last = tokenString + strlen(tokenString);
  int offset = 0;

  while((first+offset < last) && (delimiters[(int)first[offset]]==1)) {
    first++;
  }

  if(first < last) {
    while((first+offset < last) && (delimiters[(int)first[offset]]==0)) {
      offset++;
    }
    tk->token = (char *)realloc(tk->token,offset+1);
    token = tk->token;
    int iter = 0;
    for(;iter < offset;iter++) {
      token[iter] = first[iter];
    }
    token[iter] = '\0';
  }
  first = first + offset;
  tk->currPos = first;
  return token;
}

/*
 * TKHasNextToken checks to see if there are any more tokens in the stream
 * beyond the current. Function returns 1 if there are any more tokens, and
 * 0 if there are not. Function accomplishes this by checking if end of input
 * string has been reached.
 *
 * If the TokenizerT argument passed in is empty, function returns 0. If current * token is already empty (as it is for instance when there are not yet any
 * tokens), function returns 0.
 */

int TKHasNextToken(TokenizerT *tk) {
  if(tk == 0) {
    return 0;
  }
  char *curr = tk->currPos;
  char *last = tk->tokenString + strlen(tk->tokenString);
  if(curr == 0) {
    return 0;
  }
  return (curr < last);
}

/*
 * This function prints each token character by character.
 * When it encounters an escape sequence, it instead prints out the
 * hexadecimal representation of the escaped sequence.
 *
 * This function takes in a token. It returns an int.
 * If given a null token, it prints nothing and returns 0.
 *
 */
int escapeToHexMountain(char *token) {
  int printIndex = 0;
  if(token == NULL){
    return 0;
  }
  for(; printIndex < strlen(token); printIndex++) {
    switch(token[printIndex]) {
      case '\a':
        printf("[0x07]");
	break;
      case '\b':
        printf("[0x08]");
	break;
      case '\f':
        printf("[0x0C]");
	break;
      case '\n':
        printf("[0x0A]");
	break;
      case '\r':
        printf("[0x0D]");
	break;
      case '\t':
        printf("[0x09]");
	break;
      case '\v':
        printf("[0x0B]");
	break;
      case '\\':
        printf("[0x5C]");
	break;
      case '\'':
        printf("[0x27]");
	break;
      case '\"':
        printf("[0x22]");
	break;
      case '\?':
        printf("[0x3F]");
	break;
      default:
	printf("%c",token[printIndex]);
	break;
    }
  }
  printf("\n");
  return 0;
}

int help(void) {
  printf("This help file does not yet exist.");
  return 0;
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
  TokenizerT *tokenizer;
  char *numArgsError = "Wrong number of arguments. Use \"tokenizer -h\" for more information.\n";
  char *operationError = "Sorry, the operation could not be completed. Please try again.\n";
  switch(argc) {
    case 1:
      fprintf(stderr, numArgsError);
      return -1;
    case 2:
      if(strcmp(argv[1],"-h") == 0) {
        help();
        return 0;
      }
      fprintf(stderr, numArgsError);
      return -1;
    case 3:
      break;
    default:
      fprintf(stderr, numArgsError);
      return -1;
  }
  tokenizer = TKCreate(argv[1],argv[2]);
  if(tokenizer == 0) {
    printf(operationError);
    return 0;
  }
  while(TKHasNextToken(tokenizer)) {
    char *token = TKGetNextToken(tokenizer);
    escapeToHexMountain(token);
  }
  TKDestroy(tokenizer);

  return 0;
}
