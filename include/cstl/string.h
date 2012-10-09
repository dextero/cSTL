#ifndef _STRING_H
#define _STRING_H

#include "object_macros.h"

/* Struktura opisujaca stringa */
typedef struct tag_string {
    READONLY unsigned int size;
    READONLY char* c_str;
} string;

/* Typ uzyty do tworzenia hasha */
typedef long long int hash_t;


string*         string_create           (void);
string*         string_createFromC      (const char* str);
string*         string_createFromArray  (const char* arr, unsigned int size);
string*         string_copy             (const string* _this);
void            string_swap             (string* first, string* second);
void            string_release          (string* _this);

string*         string_add      (string* _this, const string* second);
string*         string_addC     (string* _this, const char* second);
void            string_addTo    (string* _this, const string* second);
void            string_addToC   (string* _this, const char* second);
void            string_assign   (string* _this, const string* str);
void            string_assignC  (string* _this, const char* str);

unsigned int    string_replace      (string* _this, const string* str, const string* replacement);
unsigned int    string_replaceC     (string* _this, const char* str, const char* replacement);
unsigned int    string_replaceChar  (string* _this, char c, char replacement);

unsigned int    string_findChar             (const string* _this, char c, unsigned int start);
unsigned int    string_findSubstr           (const string* _this, const string* substr, unsigned int start);
unsigned int    string_findSubstrC          (const string* _this, const char* substr, unsigned int start);
unsigned int    string_findFirstOfArray     (const string* _this, const char* array, unsigned int arraySize, unsigned int start);
unsigned int    string_findFirstOfC         (const string* _this, const char* chars, unsigned int start);
unsigned int    string_findFirstOfStr       (const string* _this, const string* chars, unsigned int start);
unsigned int    string_findFirstNotOfArray  (const string* _this, const char* array, unsigned int arraySize, unsigned int start);
unsigned int    string_findFirstNotOfC      (const string* _this, const char* chars, unsigned int start);
unsigned int    string_findFirstNotOfStr    (const string* _this, const string* chars, unsigned int start);

/* TODO: przetestowac RFind'y */
unsigned int    string_rFindChar            (const string* _this, char c, unsigned int start);
unsigned int    string_rFindSubstr          (const string* _this, const string* substr, unsigned int start);
unsigned int    string_rFindSubstrC         (const string* _this, const char* substr, unsigned int start);
unsigned int    string_rFindFirstOfArray    (const string* _this, const char* array, unsigned int arraySize, unsigned int start);
unsigned int    string_rFindFirstOfC        (const string* _this, const char* chars, unsigned int start);
unsigned int    string_rFindFirstOfStr      (const string* _this, const string* chars, unsigned int start);
unsigned int    string_rFindFirstNotOfArray (const string* _this, const char* array, unsigned int arraySize, unsigned int start);
unsigned int    string_rFindFirstNotOfC     (const string* _this, const char* chars, unsigned int start);
unsigned int    string_rFindFirstNotOfStr   (const string* _this, const string* chars, unsigned int start);

/* wyszukiwanie odpowiednio zagniezdzonego odpowiadajacego znaku, np. zamykajacego nawiasu */
unsigned int    string_findMatchingChar     (const string* text, unsigned int leftAt, char left, char right);
unsigned int    string_findMatchingStr      (const string* text, unsigned int leftAt, const string* left, const string* right);
unsigned int    string_findMatchingCStr     (const string* text, unsigned int leftAt, const char* left, const char* right);
unsigned int    string_rFindMatchingChar    (const string* text, unsigned int rightAt, char left, char right);
unsigned int    string_rFindMatchingStr     (const string* text, unsigned int rightAt, const string* left, const string* right);
unsigned int    string_rFindMatchingCStr    (const string* text, unsigned int rightAt, const char* left, const char* right);

string*         string_substr   (const string* _this, unsigned int start, unsigned int count);

string*         string_insert           (const string* _this, unsigned int at, const string* insert);
string*         string_insertC          (const string* _this, unsigned int at, const char* insert);
string*         string_insertArray      (const string* _this, unsigned int at, const char* insert, unsigned int size);
void            string_insertTo         (string* _this, unsigned int at, const string* insert);
void            string_insertToC        (string* _this, unsigned int at, const char* insert);
void            string_insertToArray    (string* _this, unsigned int at, const char* insert, unsigned int size);

string*         string_erase        (const string* _this, unsigned int at, unsigned int chars);
void            string_eraseFrom    (string* _this, unsigned int at, unsigned int chars);

int             string_compare      (const string* str1, const string* str2);
int             string_compareC     (const string* str1, const char* str2);
long long int   string_makeHash     (const string* str);
long long int   string_makeHashC    (const char* str);

void            string_readStdinWord    (string* _this);
void            string_readStdinLine    (string* _this);
void            string_readStdinToChar  (string* _this, char delimiter);

#endif /* _STRING_H */
