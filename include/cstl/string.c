#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <assert.h>

#define IMPLEMENTATION
#include "string.h"

/*
 *  "konstruktor"
 */
string* string_create(void)
{
    string* _this = (string*)malloc(sizeof(string));

    _this->size = 0;
    _this->c_str = NULL;

    return _this;
}

/*
 *  "konstruktor" - c-string
 */
string* string_createFromC(const char* str)
{
    string* _this = string_create();

    string_assignC(_this, str);

    return _this;
}

/*
 *  "konstruktor" - z tablicy o okreslonym rozmiarze. moze zawierac nulle.
 */
string* string_createFromArray(const char* arr, unsigned int size)
{
    string* _this = string_create();
    
    _this->size = size;
    _this->c_str = (char*)malloc((size + 1) * sizeof(char));
    memcpy(_this->c_str, arr, size * sizeof(char));
    _this->c_str[_this->size] = '\0';

    return _this;
}

/*
 *  kopiowanie
 */
string* string_copy(const string* _this)
{
    string* ret = string_create();

    if (_this->size)
    {
        ret->size = _this->size;
        ret->c_str = (char*)malloc(ret->size + 1);
        strncpy(ret->c_str, _this->c_str, ret->size + 1);
    }

    return ret;
}

/*
 *  szybka zamiana zawartosci stringow
 */
void string_swap(string* first, string* second)
{
    char* tmpStr = first->c_str;
    unsigned int tmpSize = first->size;

    first->c_str = second->c_str;
    first->size = second->size;
    second->c_str = tmpStr;
    second->size = tmpSize;
}

/*
 *  zwalnianie pamieci
 */
void string_release(string* _this)
{
    if (_this->c_str)
        free(_this->c_str);
}

/* ------------------------------------------------------------------------- */

/*
 *  dodawanie
 */
string* string_add(string* _this, const string* second)
{
    string* ret = string_create();

    if (_this->size || second->size)
    {
        ret->c_str = (char*)malloc(_this->size + second->size + 1);
        if (_this->c_str)
            memcpy(ret->c_str, _this->c_str, _this->size);
        if (second->c_str)
            memcpy(&ret->c_str[_this->size], second->c_str, second->size);
        ret->c_str[_this->size + second->size] = '\0';
        ret->size = _this->size + second->size;
    }

    return ret;
}

/*
 *  dodawanie
 */
string* string_addC(string* _this, const char* second)
{
    string* ret = string_create();
    unsigned int second_size = strlen(second);

    if (_this->size || second_size)
    {
        ret->c_str = (char*)malloc(_this->size + second_size + 1);
        if (_this->c_str)
            memcpy(ret->c_str, _this->c_str, _this->size);
        if (second)
            memcpy(&ret->c_str[_this->size], second, second_size);
        ret->c_str[_this->size + second_size] = '\0';
        ret->size = _this->size + second_size;
    }

    return ret;
}

/*
 *  dodawanie
 */
void string_addTo(string* _this, const string* second)
{
    if (second->size)
    {
        char* tmp = (char*)malloc(_this->size + second->size + 1);
        if (_this->c_str)
            memcpy(tmp, _this->c_str, _this->size);
        if (second->c_str)
            memcpy(&tmp[_this->size], second->c_str, second->size);
        tmp[_this->size + second->size] = '\0';
        
        string_release(_this);

        _this->c_str = tmp;
        _this->size += second->size;
    }
}

/*
 *  dodawanie
 */
void string_addToC(string* _this, const char* second)
{
    unsigned int second_size = strlen(second);

    if (second_size)
    {
        char* tmp = (char*)malloc(_this->size + second_size + 1);
        if (_this->c_str)
            memcpy(tmp, _this->c_str, _this->size);
        if (second)
            memcpy(&tmp[_this->size], second, second_size);
        tmp[_this->size + second_size] = '\0';
        
        string_release(_this);

        _this->c_str = tmp;
        _this->size += second_size;
    }
}

/*
 *  przypisanie
 */
void string_assignC(string* _this, const char* str)
{
    _this->size = strlen(str);
    _this->c_str = (char*)malloc(_this->size + 1);
    strncpy(_this->c_str, str, _this->size + 1);
}

/*
 *  przypisanie
 */
void string_assign(string* _this, const string* str)
{
    _this->size = str->size;
    _this->c_str = (char*)malloc(_this->size + 1);
    strncpy(_this->c_str, str->c_str, _this->size + 1);
}

/*
 *  podmiana "w miejscu"
 */
unsigned int string_replace(string* _this, const string* str, const string* replacement)
{
    unsigned int foundAt = -1;
    unsigned int prev = 0;
    unsigned int timesReplaced = 0;

    if ((foundAt = string_findSubstr(_this, str, prev)) != -1)
    {
        // string, z ktorego bedziemy pobierac dane
        string* tmpstring = string_create();
        // substring od ostatniej podmiany do nastepnego znalezionego miejsca
        string* substr = NULL;

        // zamiana zawartosci - dane maja wyladowac w _this
        string_swap(tmpstring, _this);

        do {
            substr = string_substr(tmpstring, prev, foundAt - prev);

            // string += substring + podmieniony_fragment
            string_addTo(_this, substr);
            string_addTo(_this, replacement);
            string_release(substr);

            // przesuniecie "wskaznika" - odkad ma kopiowac nastepny nienaruszony fragment
            prev = foundAt + str->size;
            ++timesReplaced;
        } while ((foundAt = string_findSubstr(tmpstring, str, prev)) != -1);

        // doklejanie koncowki
        substr = string_substr(tmpstring, prev, -1);
        string_addTo(_this, substr);

        // to juz niepotrzebne
        string_release(substr);
        string_release(tmpstring);
    }

    return timesReplaced;
}

/*
 *  podmiana
 */
unsigned int string_replaceC(string* _this, const char* str, const char* replacement)
{
    string* s = string_createFromC(str);
    string* r = string_createFromC(replacement);
    unsigned int replacements = string_replace(_this, s, r);

    string_release(s);
    string_release(r);

    return replacements;
}

/*
 *  podmiana pojedynczych znakow
 */
unsigned int string_replaceChar(string* _this, char c, char replacement)
{
    unsigned int foundAt = -1;
    unsigned int timesReplaced = 0;

    while ((foundAt = string_findChar(_this, c, foundAt + 1)) != -1)
    {
        _this->c_str[foundAt] = replacement;
        ++timesReplaced;
    }

    return timesReplaced;
}

/* ------------------------------------------------------------------------- */

/*
 *
 */
unsigned int string_findChar(const string* _this, char c, unsigned int start)
{
    unsigned int at = 0;

    for (at = start; at < _this->size; ++at)
        if (_this->c_str[at] == c)
            return at;

    return -1;
}

/*
 *
 */
unsigned int string_findSubstr(const string* _this, const string* substr, unsigned int start)
{
    unsigned int at = 0, i = 0;

    assert(substr->size > 0 && "Searching for an empty string");
    
    if (_this->size < substr->size)
        return -1;

    for (at = start; at < _this->size - substr->size; ++at)
        if (_this->c_str[at] == substr->c_str[0])
        {
            for (i = 1; i < substr->size; ++i)
                if (_this->c_str[at + i] != substr->c_str[i])
                    break;

            /* znalazlo caly wyraz */
            if (i == substr->size)
                return at;
        }

    return -1;
}

/*
 *
 */
unsigned int string_findSubstrC(const string* _this, const char* substr, unsigned int start)
{
    unsigned int at = 0, i = 0, size = strlen(substr);

    assert(size > 0 && "Searching for an empty string");

    if (_this->size < size)
        return -1;

    for (at = start; at < _this->size - size; ++at)
        if (_this->c_str[at] == substr[0])
        {
            for (i = 1; i < size; ++i)
                if (_this->c_str[at + i] != substr[i])
                    break;

            /* znalazlo caly wyraz */
            if (i == size)
                return at;
        }

    return -1;
}

/*
 *
 */
unsigned int string_findFirstOfArray(const string* _this, const char* array, unsigned int arraySize, unsigned int start)
{
    unsigned int at = 0, i = 0;

    assert(arraySize > 0 && "Searching for nothing.");

    for (at = start; at < _this->size; ++at)
        for (i = 0; i < arraySize; ++i)
            if (_this->c_str[at] == array[i])
                return at;

    return -1;
}

/*
 *
 */
unsigned int string_findFirstOfC(const string* _this, const char* chars, unsigned int start)
{
    unsigned int at = 0, i = 0, size = strlen(chars);

    assert(size > 0 && "Searching for nothing.");

    for (at = start; at < _this->size; ++at)
        for (i = 0; i < size; ++i)
            if (_this->c_str[at] == chars[i])
                return at;

    return -1;
}

/*
 *
 */
unsigned int string_findFirstOfStr(const string* _this, const string* chars, unsigned int start)
{
    unsigned int at = 0, i = 0;

    assert(chars->size > 0 && "Searching for nothing.");

    for (at = start; at < _this->size; ++at)
        for (i = 0; i < chars->size; ++i)
            if (_this->c_str[at] == chars->c_str[i])
                return at;

    return -1;
}

/*
 *
 */
unsigned int string_findFirstNotOfArray(const string* _this, const char* array, unsigned int arraySize, unsigned int start)
{
    unsigned int at = 0, i = 0;

    assert(arraySize > 0 && "Searching for anything.");

    for (at = start; at < _this->size; ++at)
    {
        for (i = 0; i < arraySize; ++i)
            if (_this->c_str[at] == array[i])
                break;

        if (i == arraySize)
            return at;
    }

    return -1;
}

/*
 *
 */
unsigned int string_findFirstNotOfC(const string* _this, const char* chars, unsigned int start)
{
    unsigned int at = 0, i = 0, size = strlen(chars);

    assert(size > 0 && "Searching for anything.");

    for (at = start; at < _this->size; ++at)
    {
        for (i = 0; i < size; ++i)
            if (_this->c_str[at] == chars[i])
                break;

        if (i == size)
            return at;
    }

    return -1;
}

/*
 *
 */
unsigned int string_findFirstNotOfStr(const string* _this, const string* chars, unsigned int start)
{
    unsigned int at = 0, i = 0;

    assert(chars->size > 0 && "Searching for anything.");

    for (at = start; at < _this->size; ++at)
    {
        for (i = 0; i < chars->size; ++i)
            if (_this->c_str[at] == chars->c_str[i])
                break;

        if (i == chars->size)
            return at;
    }

    return -1;
}

/*
 *
 */
unsigned int string_rFindChar(const string* _this, char c, unsigned int start)
{
    unsigned int at = 0;

    for (at = start; (at < _this->size) && (at <= -1); --at)
        if (_this->c_str[at] == c)
            return at;

    return -1;
}

/*
 *
 */
unsigned int string_rFindSubstr(const string* _this, const string* substr, unsigned int start)
{
    unsigned int at = 0, i = 0;

    assert(substr->size > 0 && "Searching for an empty string");
    
    if (_this->size < substr->size)
        return -1;

    for (at = start - substr->size; (at < _this->size) && (at <= -1); --at)
        if (_this->c_str[at] == substr->c_str[0])
        {
            for (i = 1; i < substr->size; ++i)
                if (_this->c_str[at + i] != substr->c_str[i])
                    break;

            /* znalazlo caly wyraz */
            if (i == substr->size)
                return at;
        }

    return -1;
}

/*
 *
 */
unsigned int string_rFindSubstrC(const string* _this, const char* substr, unsigned int start)
{
    unsigned int at = 0, i = 0, size = strlen(substr);

    assert(size > 0 && "Searching for an empty string");

    if (_this->size < size)
        return -1;

    for (at = start - size; (at < _this->size) && (at <= -1); --at)
        if (_this->c_str[at] == substr[0])
        {
            for (i = 1; i < size; ++i)
                if (_this->c_str[at + i] != substr[i])
                    break;

            /* znalazlo caly wyraz */
            if (i == size)
                return at;
        }

    return -1;
}

/*
 *
 */
unsigned int string_rFindFirstOfArray(const string* _this, const char* array, unsigned int arraySize, unsigned int start)
{
    unsigned int at = 0, i = 0;

    assert(arraySize > 0 && "Searching for nothing.");

    for (at = start; (at < _this->size) && (at <= -1); --at)
        for (i = 0; i < arraySize; ++i)
            if (_this->c_str[at] == array[i])
                return at;

    return -1;
}

/*
 *
 */
unsigned int string_rFindFirstOfC(const string* _this, const char* chars, unsigned int start)
{
    unsigned int at = 0, i = 0, size = strlen(chars);

    assert(size > 0 && "Searching for nothing.");

    for (at = start; (at < _this->size) && (at <= -1); --at)
        for (i = 0; i < size; ++i)
            if (_this->c_str[at] == chars[i])
                return at;

    return -1;
}

/*
 *
 */
unsigned int string_rFindFirstOfStr(const string* _this, const string* chars, unsigned int start)
{
    unsigned int at = 0, i = 0;

    assert(chars->size > 0 && "Searching for nothing.");

    for (at = start; (at < _this->size) && (at <= -1); --at)
        for (i = 0; i < chars->size; ++i)
            if (_this->c_str[at] == chars->c_str[i])
                return at;

    return -1;
}

/*
 *
 */
unsigned int string_rFindFirstNotOfArray(const string* _this, const char* array, unsigned int arraySize, unsigned int start)
{
    unsigned int at = 0, i = 0;

    assert(arraySize > 0 && "Searching for anything.");

    for (at = start; (at < _this->size) && (at <= -1); --at)
    {
        for (i = 0; i < arraySize; ++i)
            if (_this->c_str[at] == array[i])
                break;

        if (i == arraySize)
            return at;
    }

    return -1;
}

/*
 *
 */
unsigned int string_rFindFirstNotOfC(const string* _this, const char* chars, unsigned int start)
{
    unsigned int at = 0, i = 0, size = strlen(chars);

    assert(size > 0 && "Searching for anything.");

    for (at = start; (at < _this->size) && (at <= -1); --at)
    {
        for (i = 0; i < size; ++i)
            if (_this->c_str[at] == chars[i])
                break;

        if (i == size)
            return at;
    }

    return -1;
}

/*
 *
 */
unsigned int string_rFindFirstNotOfStr(const string* _this, const string* chars, unsigned int start)
{
    unsigned int at = 0, i = 0;

    assert(chars->size > 0 && "Searching for anything.");

    for (at = start; (at < _this->size) && (at <= -1); --at)
    {
        for (i = 0; i < chars->size; ++i)
            if (_this->c_str[at] == chars->c_str[i])
                break;

        if (i == chars->size)
            return at;
    }

    return -1;
}


unsigned int string_findMatchingChar(const string* text, unsigned int leftAt, char left, char right)
{
    // zakladamy, ze '<left>' jest przed leftAt, a '<right>' za
    unsigned int open, close, count = 1;
    while (count)
    {
        open = string_findChar(text, left, leftAt + 1);
        close = string_findChar(text, right, leftAt + 1);

        // jesli znajdujemy <left> przed <right>, to wchodzimy stopien glebiej
        if (close > open)
        {
            ++count;
            leftAt = open;
        }
        else // open >= close, czyli wychodzimy
        {
            if (close == -1) // koniec stringa
                return -1;

            --count;
            leftAt = close;
        }
    }

    return close;
}

unsigned int string_findMatchingStr(const string* text, unsigned int leftAt, const string* left, const string* right)
{
    // zakladamy, ze '<left>' jest przed leftAt, a '<right>' za
    unsigned int open, close, count = 1;

    assert(left->size && right->size && "Searching for matching null string.");

    while (count)
    {
        open = string_findSubstr(text, left, leftAt + left->size);
        close = string_findSubstr(text, right, leftAt + right->size);

        // jesli znajdujemy <left> przed <right>, to wchodzimy stopien glebiej
        if (close > open)
        {
            ++count;
            leftAt = open;
        }
        else // open >= close, czyli wychodzimy
        {
            if (close >= text->size) // koniec stringa
                return -1;

            --count;
            leftAt = close;
        }
    }

    return close;
}

unsigned int string_findMatchingCStr(const string* text, unsigned int leftAt, const char* left, const char* right)
{
    string* begin = string_createFromC(left), * end = string_createFromC(right);
    unsigned int ret = string_findMatchingStr(text, leftAt, begin, end);

    string_release(begin);
    string_release(end);
    return ret;
}

unsigned int string_rFindMatchingChar(const string* text, unsigned int rightAt, char left, char right)
{
    // zakladamy, ze '<left>' jest przed leftAt, a '<right>' pod text[rightAt]
    unsigned int open, close, count = 1;
    while (count)
    {
        open = string_rFindChar(text, left, rightAt - 1);
        close = string_rFindChar(text, right, rightAt - 1);

        // jesli znajdujemy <right> przed <left>, to wchodzimy stopien glebiej
        if (close > open)
        {
            ++count;
            rightAt = close;
        }
        else // open >= close, czyli wychodzimy
        {
            if (close == -1) // koniec stringa
                return -1;

            --count;
            rightAt = close;
        }
    }

    return close;
}

unsigned int string_rFindMatchingStr(const string* text, unsigned int rightAt, const string* left, const string* right)
{
    // zakladamy, ze '<left>' jest przed leftAt, a '<right>' pod text[rightAt]
    unsigned int open, close, count = 1;

    assert(left->size && right->size && "Searching for matching null string.");
    
    while (count)
    {
        open = string_rFindSubstr(text, left, rightAt - left->size);
        close = string_rFindSubstr(text, right, rightAt - right->size);

        // jesli znajdujemy <right> przed <left>, to wchodzimy stopien glebiej
        if (close > open)
        {
            ++count;
            rightAt = close;
        }
        else // open >= close, czyli wychodzimy
        {
            if (close >= text->size) // koniec stringa
                return -1;

            --count;
            rightAt = close;
        }
    }

    return close;
}

unsigned int string_rFindMatchingCStr(const string* text, unsigned int rightAt, const char* left, const char* right)
{
    string* begin = string_createFromC(left), * end = string_createFromC(right);
    unsigned int ret = string_rFindMatchingStr(text, rightAt, begin, end);

    string_release(begin);
    string_release(end);
    return ret;
}


/* ------------------------------------------------------------------------- */

/*
 *  substring
 */
string* string_substr(const string* _this, unsigned int start, unsigned int count)
{
    string* ret = string_create();

    if ((_this->size > start) && count)
    {
        if (count == -1 || _this->size < start + count)
            count = _this->size - start;

        ret->c_str = (char*)malloc(count + 1);
        memcpy(ret->c_str, &_this->c_str[start], count);
        ret->c_str[count] = '\0';

        ret->size = count;
    }

    return ret;
}

// ----------------------------------------------------

string* string_insert(const string* _this, unsigned int at, const string* insert)
{
    string* ret = string_substr(_this, 0, at), * end = string_substr(_this, at, -1);
    string_addTo(ret, insert);
    string_addTo(ret, end);

    string_release(end);
    return ret;
}

string* string_insertC(const string* _this, unsigned int at, const char* insert)
{
    string* str = string_createFromC(insert);
    string* ret = string_insert(_this, at, str);

    string_release(str);
    return ret;
}

string* string_insertArray(const string* _this, unsigned int at, const char* insert, unsigned int size)
{
    string* str = string_createFromArray(insert, size);
    string* ret = string_insert(_this, at, str);

    string_release(str);
    return ret;
}

void string_insertTo(string* _this, unsigned int at, const string* insert)
{
    string* tmp = string_substr(_this, at, -1);

    string_eraseFrom(_this, at, -1);
    string_addTo(_this, insert);
    string_addTo(_this, tmp);

    string_release(tmp);
}

void string_insertToC(string* _this, unsigned int at, const char* insert)
{
    string* str = string_createFromC(insert);
    string_insertTo(_this, at, str);
    string_release(str);
}

void string_insertToArray(string* _this, unsigned int at, const char* insert, unsigned int size)
{
    string* str = string_createFromArray(insert, size);
    string_insertTo(_this, at, str);
    string_release(str);
}


string* string_erase(const string* _this, unsigned int at, unsigned int chars)
{
    string* ret, * end;

    if (at >= _this->size)
        return string_copy(_this);
    
    ret = string_substr(_this, 0, at);

    if (at + chars < _this->size)
    {
        end = string_substr(_this, at + chars, -1);
        string_addTo(ret, end);
        string_release(end);
    }

    return ret;
}

void string_eraseFrom(string* _this, unsigned int at, unsigned int chars)
{
    string* ret, * end;

    if (at >= _this->size)
        return;

    ret = string_substr(_this, 0, at);

    if (at + chars < _this->size)
    {
        end = string_substr(_this, at + chars, -1);
        string_addTo(ret, end);
        string_release(end);
    }

    string_swap(_this, ret);
    string_release(ret);
}


// ----------------------------------------------------

int string_compare(const string* str1, const string* str2)
{
    return strcmp(str1->c_str, str2->c_str);
}

int string_compareC(const string* str1, const char* str2)
{
    return strcmp(str1->c_str, str2);
}


long long int string_makeHash(const string* str)
{
    const char* data = str->c_str;
    unsigned int len = str->size;

    long long int hash = len, tmp;
    int rem;

    rem = len & 3;
    len >>= 2;

    for (; len > 0; len--)
    {
        hash += ((unsigned int)(data[1]) << 8) + (unsigned int)(data[0]);
        tmp = ((((unsigned int)(data[3]) << 8) + (unsigned int)(data[2])) << 11) ^ hash;
        hash = (hash << 16) ^ tmp;
        data += 4;
        hash += hash >> 11;
    }

    switch (rem)
    {
    case 3:
        hash += ((unsigned int)(data[1]) << 8) + (unsigned int)(data[0]);
		hash ^= hash << 16;
		hash ^= data[2] << 18;
		hash += hash >> 11;
        break;
    case 2:
        hash += ((unsigned int)(data[1]) << 8) + (unsigned int)(data[0]);
		hash ^= hash << 11;
		hash += hash >> 17;
        break;
    case 1:
        hash += *data;
		hash ^= hash << 10;
		hash += hash >> 1;
        break;
    default:
        break;
    }

	hash ^= hash << 3;
	hash += hash >> 5;
	hash ^= hash << 4;
	hash += hash >> 17;
	hash ^= hash << 25;
	hash += hash >> 6;

	return hash + 0x10000000;
}

long long int string_makeHashC(const char* str)
{
    string* tmp = string_createFromC(str);
    long long int ret = string_makeHash(tmp);
    string_release(tmp);
    return ret;
}


/*
 *  czytanie stdin-a
 */
#define BUFFER_SIZE 128

/*
 *  slowo
 */
void string_readStdinWord(string* _this)
{
    char buffer[BUFFER_SIZE];
    unsigned int chars = -1;

    buffer[BUFFER_SIZE - 1] = '\0';
    string_assignC(_this, "");

    while (!feof(stdin))
    {
        chars = 0;

        while (chars < BUFFER_SIZE - 1 && !feof(stdin))
        {
            buffer[chars] = (char)fgetc(stdin);
            if (buffer[chars] == ' ' || buffer[chars] == '\n' || buffer[chars] == '\t')
            {
                buffer[chars] = '\0';
                string_addToC(_this, buffer);
                return;
            }

            ++chars;
        }

        if (feof(stdin))
            buffer[chars] = '\0';

        string_addToC(_this, buffer);
    }
}

/*
 *  linia
 */
void string_readStdinLine(string* _this)
{
    char buffer[BUFFER_SIZE];
    unsigned int chars = -1;

    buffer[BUFFER_SIZE - 1] = '\0';
    string_assignC(_this, "");

    while (!feof(stdin))
    {
        chars = 0;

        while (chars < BUFFER_SIZE - 1 && !feof(stdin))
        {
            buffer[chars] = (char)fgetc(stdin);
            if (buffer[chars] == '\n')
            {
                buffer[chars] = '\0';
                string_addToC(_this, buffer);
                return;
            }

            ++chars;
        }

        if (feof(stdin))
            buffer[chars] = '\0';

        string_addToC(_this, buffer);
    }
}

/*
 *  do napotkania okreslonego znaku
 */
void string_readStdinToChar(string* _this, char delimiter)
{
    char buffer[BUFFER_SIZE];
    unsigned int chars = -1;

    buffer[BUFFER_SIZE - 1] = '\0';
    string_assignC(_this, "");

    while (!feof(stdin))
    {
        chars = 0;

        while (chars < BUFFER_SIZE - 1 && !feof(stdin))
        {
            buffer[chars] = (char)fgetc(stdin);
            if (buffer[chars] == delimiter)
            {
                buffer[chars] = '\0';
                string_addToC(_this, buffer);
                return;
            }

            ++chars;
        }

        if (feof(stdin))
            buffer[chars] = '\0';

        string_addToC(_this, buffer);
    }
}
