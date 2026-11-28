#ifndef DELIMITERS_COMMENTS_H
#define DELIMITERS_COMMENTS_H

#include <string>
#include "token.h"
using namespace std;

// ---------------------------------------------------------------------
// Delimiters, punctuation and comment identification
//
// Delimiters / punctuation recognized:   (  )  {  }  ;  :  ,
// Comments:  the word  lowkey  starts a single-line comment that runs
//            to the end of the line (AERIS has no multi-line comments).
// ---------------------------------------------------------------------

// F01: Checks whether a comment starts at position pos.
// True only when the complete word "lowkey" starts there, so an
// identifier such as lowkeyfoo is NOT treated as a comment.
bool isCommentStart(const string& code, int pos);

// F02: Skips the comment up to (but not including) the newline.
// Updates pos and col. The main loop handles the newline itself.
void skipComment(const string& code, int &pos, int &line, int &col);

// F03: Checks whether the character at pos is a delimiter or punctuation.
// A ':' that is part of the := operator is NOT punctuation.
bool isDelimiterAt(const string& code, int pos);

// F04: Reads one delimiter or punctuation token and advances pos and col.
Token readDelimiter(const string& code, int &pos, int &line, int &col);

#endif
