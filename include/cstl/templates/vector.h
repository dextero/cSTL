#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../object_macros.h"

#if !defined(TypeString) && defined(Type)
#   define TypeString Type
#endif

#if !defined(Type)
#   pragma message("WARNING: At least Type must be defined!")
#else

    /*
     *  makro do uproszczenia tworzenia zmiennych
     *  i nastepne, pomocnicze do metod
     */
#   ifndef vector_t
#       define vector_t(TypeString)  STRUCT_TEMPLATE(vector, TypeString)
#   endif

#   define METHOD(name) METHOD_TEMPLATE(vector, TypeString, name)

    /*
     *  struktura
     */
    typedef struct STRUCT_TEMPLATE(tag_vector, TypeString) {
        READONLY unsigned int size;
        READONLY unsigned int capacity;
        READONLY Type* array;

        Type (*objectCopyConstructor)(const Type);
        void (*objectDestructor)(Type);
        int  (*objectCompareFunc)(const Type, const Type);
    } vector_t(TypeString);

    /*
     *  deklaracje zapowiadajace
     */
    vector_t(TypeString)*   METHOD(create)                      (void);
    void                    METHOD(clear)                       (vector_t(TypeString)* _this);
    void                    METHOD(destructObjects)             (vector_t(TypeString)* _this, unsigned int start, unsigned int end);
    void                    METHOD(eraseAt)                     (vector_t(TypeString)* _this, unsigned int at);
    void                    METHOD(eraseRange)                  (vector_t(TypeString)* _this, unsigned int at, unsigned int count);
    unsigned int            METHOD(find)                        (const vector_t(TypeString)* _this, const Type elem);
    void                    METHOD(insert)                      (vector_t(TypeString)* _this, const Type elem, unsigned int where);
    void                    METHOD(pushBack)                    (vector_t(TypeString)* _this, const Type elem);
    void                    METHOD(release)                     (vector_t(TypeString)* _this);
    void                    METHOD(reserve)                     (vector_t(TypeString)* _this, unsigned int newSize);

    /*
     *  implementacja: osobne makro, funkcje moga byc zdefiniowane tylko raz
     */
#   ifdef   IMPLEMENTATION
    
#       define VECTOR_MIN_SIZE  256
#       ifndef VECTOR_CAP_INCREASE
#           define VECTOR_CAP_INCREASE(currCap) ((currCap) * 2)
#       endif /* VECTOR_CAP_INCREASE */

        /*
         *  "konstruktor"
         */
        vector_t(TypeString)* METHOD(create)(void)
        {
            vector_t(TypeString)* _this = (vector_t(TypeString)*)malloc(sizeof(vector_t(TypeString)));

            _this->size = 0;
            _this->array = (Type*)malloc(VECTOR_MIN_SIZE * sizeof(Type));
            _this->capacity = VECTOR_MIN_SIZE;

            _this->objectCopyConstructor = NULL;
            _this->objectDestructor = NULL;

            return _this;
        }

        /*
         * usuwanie zawartosci wektora
         */
        void METHOD(clear)(vector_t(TypeString)* _this)
        {
            assert(_this);

            METHOD(destructObjects)(_this, 0, _this->size - 1);

            _this->size = 0;
        }

        /*
         *  wywolywanie "destruktorow" obiektow, jesli takie posiadaja, "prywatne"
         *  zakres: [start, end)
         */
        void METHOD(destructObjects)(vector_t(TypeString)* _this, unsigned int start, unsigned int end)
        {
            assert(_this);
            assert(start == 0 || start < _this->size);
            assert(start <= end);

            if (_this->objectDestructor)
            {
                unsigned int i;

                if (end > _this->size)

                    end = _this->size;

                for (i = start; i < end; ++i)
                    _this->objectDestructor(_this->array[i]);
            }
        }

        /*
         *  usuwanie jednego elementu
         */
        void METHOD(eraseAt)(vector_t(TypeString)* _this, unsigned int at)
        {
            assert(_this);
            assert(at < _this->size);

            if (_this->size > at)
            {
                METHOD(destructObjects)(_this, at, at + 1);

                // taki hack, moze zadziala
                memcpy(&_this->array[at], &_this->array[at + 1], (_this->size - at - 1) * sizeof(Type));

                --_this->size;
            }
        }

        /*
         *  usuwanie zakresu elementow
         */
        void METHOD(eraseRange)(vector_t(TypeString)* _this, unsigned int at, unsigned int count)
        {
            assert(_this);
            assert(at < _this->size);
            assert(count < _this->size - at);

            if (_this->size > at)
            {
                METHOD(destructObjects)(_this, at, at + count);

                // taki hack, moze zadziala
                memcpy(&_this->array[at], &_this->array[at + count], (_this->size - at - count) * sizeof(Type));

                _this->size -= count;
            }
        }

        /*
         *  wyszukiwanie
         */
        unsigned int METHOD(find)(const vector_t(TypeString)* _this, const Type elem)
        {
            assert(_this);

            if (_this->objectCompareFunc)
            {
                unsigned int i;
                for (i = 0; i < _this->size; ++i)
                {
                    if (_this->objectCompareFunc(_this->array[i], elem) == 0)
                        return i;
                }
            }
            else
            {
                unsigned int i;
                for (i = 0; i < _this->size; ++i)
                {
                    if (*(int*)&_this->array[i] == *(int*)&elem)
                        return i;
                }
            }

            return -1;
        }

        /*
         *  wstawianie elementu w okreslone miejsce
         */
        void METHOD(insert)(vector_t(TypeString)* _this, const Type elem, unsigned int where)
        {
            assert(_this);
            assert(where <= _this->size);

            unsigned int i;

            if (_this->size >= _this->capacity)
                METHOD(reserve)(_this, VECTOR_CAP_INCREASE(_this->size));

            // nie pozwalamy na dodawanie za koncem
            if (where > _this->size)
                where = _this->size;

            for (i = _this->size; i > where; --i)
                _this->array[i] = _this->array[i - 1];

            if (_this->objectCopyConstructor)
                _this->array[where] = _this->objectCopyConstructor(elem);
            else
                _this->array[where] = (Type)elem;
            
            _this->size++;
        }

        /*
         *  dodanie elementu na koniec listy
         */
        void METHOD(pushBack)(vector_t(TypeString)* _this, const Type elem)
        {
            assert(_this);

            if (_this->size >= _this->capacity)
                METHOD(reserve)(_this, VECTOR_CAP_INCREASE(_this->size));

            if (_this->objectCopyConstructor)
                _this->array[_this->size] = _this->objectCopyConstructor(elem);
            else
                _this->array[_this->size] = (Type)elem;

            _this->size++;
        }

        /*
         *  "destruktor"  
         */
        void METHOD(release)(vector_t(TypeString)* _this)
        {
            assert(_this);

            METHOD(destructObjects)(_this, 0, _this->size);

            free(_this->array);
            _this->array = NULL;
            _this->size = 0;
            _this->capacity = 0;
        }

        /*
         *  zmiana rozmiaru wektora
         */
        void METHOD(reserve)(vector_t(TypeString)* _this, unsigned int newSize)
        {
            assert(_this);

            if (_this->capacity < newSize)
            {
                Type* tmp = (Type*)malloc((newSize) * sizeof(Type));
                memcpy(tmp, _this->array, _this->capacity * sizeof(Type));
                free(_this->array);
                _this->array = tmp;
                _this->capacity = newSize;
            }
            else if (_this->capacity > newSize)
            {
                METHOD(destructObjects)(_this, newSize + 1, _this->size);

                _this->size = newSize;
            }
        }

#       undef VECTOR_MIN_SIZE
#       undef VECTOR_CAP_INCREASE

#   endif // IMPLEMENTATION

#undef METHOD
#undef Type
#undef TypeString

#endif
