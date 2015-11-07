# tokenizer

Description: String Tokenizer in C built for Rutgers Computer Science Class, Systems Programming 198:214

## Overview
This is a generic string tokenizer generator built in the C Programming Language. The functionality includes the standard tokenizer functionalities: a create function, a destroy function, a function to check for tokens, and a function to return tokens.

In addition, there are a few utility functions to do required tasks such as reformatting of escape characters in I/O stream input.


## Usage
As usual, in order to use the tokenizer, the header file "tokenizer.h" must be included in the respective file with the ```#include``` macro. The "tokenizer.c" file also includes a sample ```main``` method which should be removed before use.

Once included, the recommended use pattern is as follows:

1. To create a tokenizer, call the ```TKCreate(char *, char *)``` function, where the first argument is a string of delimiters to be used and the second is the stream to be tokenized. The return parameter is a pointer to the newly created TokenizerT struct.

2. Loop through the tokens in the stream, checking at each instance whether there are more tokens to be read. The functions to be used for this are ```TKHasNextToken(TokenizerT *)``` and ```TKGetNextToken(TokenizerT *)```.

3. Once finished using the tokenizer, free the appropriate memory in the TokenizerT struct by calling ```TKDestroy(TokenizerT *)```. The TokenizerT object should always be freed after use to prevent memory leaks.

A sample use using a while loop follows. Assuming a stream ```strm```:

    // included print functionality for sake of example
    #include <stdio.h>
    #include "tokenizer.h"

    ...

    // suppose the intention is to find all sentences.
    // for simple sentence cases, it would be sufficient to generate tokens delimited by periods
    char *delims = ".";

    // create the tokenizer
    TokenizerT *tk = TKCreate(delims, strm);

    // loop through tokens
    while(TKHasNextToken(tk)) {
      char *sentence = TKGetNextToken(tk);
      printf("%s\n", sentence);
    }

    // free the tokenizer memory
    TKDestroy(tk);

    ...



## Licensing

This code is free for anyone to use. No restrictions, limits, or liabilities.
