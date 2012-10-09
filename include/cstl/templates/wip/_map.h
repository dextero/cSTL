#include <stdlib.h>

#include "../object_macros.h"


#if !defined(TypeString1) && defined(Type1)
#   define TypeString1 Type1
#endif
#if !defined(TypeString2) && defined(Type2)
#   define TypeString2 Type2
#endif

#define SKIP_TYPE_UNDEFS
#include "pair.h"


#if !defined(Type1) || !defined(Type2)
#   pragma message("WARNING: At least Type1 and Type2 must be defined!")
#else
#   ifdef IMPLEMENTATION

#       define __PairTypeString(a, b)   pair_ ## a ## _ ## b
#       define _PairTypeString(a, b)    __PairTypeString(a, b)
#       define PairTypeString           _PairTypeString(TypeString1, TypeString2)

#       define Type pair_t(TypeString1, TypeString2)*
#       define TypeString PairTypeString

#       include "vector.h"

#       undef TypeString

#   endif
    

    /*
     *  ponowna deklaracja TypeString
     */
#   define  TypeString _TypeString(TypeString1, TypeString2)

#   ifndef map_t
#       define  _map_t(a, b)                        STRUCT_TEMPLATE(map, a ## _ ## b)
#       define  map_t(TypeString1, TypeString2)     _map_t(TypeString1, TypeString2)
#   endif

#   define  METHOD(name)                        METHOD_TEMPLATE(Map, TypeString, name)

    typedef struct map_t(TypeString1, TypeString2) map_t(TypeString1, TypeString2);


    map_t(TypeString1, TypeString2)*    METHOD(Create)          ();
    void                                METHOD(Release)         (map_t(TypeString1, TypeString2)* _this);

    Type2                               METHOD(At)              (const map_t(TypeString1, TypeString2)* _this, const Type1 key);
    unsigned int                        METHOD(FindInVector)    (const map_t(TypeString1, TypeString2)* _this, const Type1 key);
    pair_t(TypeString1, TypeString2)*   METHOD(Find)            (const map_t(TypeString1, TypeString2)* _this, const Type1 key);

    void                                METHOD(InsertPair)      (map_t(TypeString1, TypeString2)* _this, const pair_t(TypeString1, TypeString2)* pair);
    void                                METHOD(InsertValues)    (map_t(TypeString1, TypeString2)* _this, const Type1 first, const Type2 second);
    void                                METHOD(Erase)           (map_t(TypeString1, TypeString2)* _this, const Type1 key);

    unsigned int                        METHOD(Size)            (const map_t(TypeString1, TypeString2)* _this);


#   ifdef IMPLEMENTATION

        typedef struct map_t(TypeString1, TypeString2) {
            vector_t(PairTypeString)* vector;
        } map_t(TypeString1, TypeString2);
        

        map_t(TypeString1, TypeString2)* METHOD(Create)()
        {
            map_t(TypeString1, TypeString2)* _this = (map_t(TypeString1, TypeString2)*)malloc(sizeof(map_t(TypeString1, TypeString2)));

            _this->vector = METHOD_TEMPLATE(Vector, pair_t(TypeString1, TypeString2), Create)();

            return _this;
        }

        void METHOD(Release)(map_t(TypeString1, TypeString2)* _this)
        {
            METHOD_TEMPLATE(Vector, pair_t(TypeString1, TypeString2), Release)(_this->vector);
        }


        Type2 METHOD(At)(const map_t(TypeString1, TypeString2)* _this, const Type1 key)
        {
            return _this->vector->array[METHOD(FindInVector)(_this, key)]->second;
        }

        unsigned int METHOD(FindInVector)(const map_t(TypeString1, TypeString2)* _this, const Type1 key)
        {
            int begin = 0, end = _this->vector->size;
            int at = (begin + end) / 2;

            while (begin < end)
            {
                int keyInt = *(int*)&key;
                int valInt = *(int*)&_this->vector->array[at]->first;

                if (valInt == keyInt)
                    return at;

                if (valInt > keyInt)
                    end = at;
                else
                    begin = at + 1;

                at = (begin + end) / 2;
            }

            return -1;
        }

        pair_t(TypeString1, TypeString2)* METHOD(Find)(const map_t(TypeString1, TypeString2)* _this, const Type1 key)
        {
            unsigned int i = METHOD(FindInVector)(_this, key);
            if (i == -1)
                return NULL;

            return _this->vector->array[i];
        }

        /* DEBUG *
        void METHOD(PrintContents)(map_t(TypeString1, TypeString2)* _this)
        {
            unsigned int i;
            for (i = 0; i < _this->vector->size; ++i)
            {
                const pair_t(int, int)* pair = METHOD_TEMPLATE(Vector, PairTypeString, At)(_this->vector, i);
                if (pair)
                    printf("%d: %d => %d\n", i, pair->first, pair->second);
                else
                    printf("%d: error\n", i);
            }
        }/**/

        void METHOD(InsertPair)(map_t(TypeString1, TypeString2)* _this, const pair_t(TypeString1, TypeString2)* pair)
        {
            int begin = 0, end = _this->vector->size;
            int at = (begin + end) / 2;

            while (begin < end)
            {
                int keyInt = *(int*)&pair->first;
                int valInt = *(int*)&_this->vector->array[at]->first;

                if (valInt == keyInt)
                    break;

                if (valInt > keyInt)
                    end = at;
                else
                    begin = at + 1;

                at = (begin + end) / 2;
            }

            METHOD_TEMPLATE(Vector, PairTypeString, Insert)(_this->vector, pair, at);
        }

        void METHOD(InsertValues)(map_t(TypeString1, TypeString2)* _this, const Type1 first, const Type2 second)
        {
            pair_t(TypeString1, TypeString2)* elem = METHOD_TEMPLATE(Pair, TypeString, CreateFromValues)(first, second);

            METHOD(InsertPair)(_this, elem);
        }

        void METHOD(Erase)(map_t(TypeString1, TypeString2)* _this, const Type1 key)
        {
            unsigned int i = METHOD(FindInVector)(_this, key);
            if (i != -1)
                METHOD_TEMPLATE(Vector, PairTypeString, EraseAt)(_this->vector, i);
        }


        unsigned int METHOD(Size)(const map_t(TypeString1, TypeString2)* _this)
        {
            return METHOD_TEMPLATE(Vector, PairTypeString, Size)(_this->vector);
        }


#   endif

#   undef __TypeString
#   undef _TypeString
#   undef TypeString
#   undef __PairTypeString
#   undef _PairTypeString
#   undef PairTypeString
#   undef TypeString1
#   undef TypeString2
#   undef Type1
#   undef Type2
#   undef METHOD

#endif
