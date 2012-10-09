#include <stdlib.h>
#include <assert.h>

#include "../object_macros.h"

#if !defined(TypeString) && defined(Type)
#   define TypeString Type
#endif

#if !defined(Type)
#   pragma message("WARNING: At least Type must be defined!")
#else

#   ifndef list_item_t
#       define  list_item_t(TypeString)  STRUCT_TEMPLATE(list_item, TypeString)
#   endif

#   define  METHOD(name)            METHOD_TEMPLATE(list_item, TypeString, name)

    /*
     *  struktura pomocnicza - element listy
     */

    typedef struct STRUCT_TEMPLATE(list_item, TypeString) {
	    READONLY struct STRUCT_TEMPLATE(list_item, TypeString) * prev, * next;
	    READONLY Type value;
    } list_item_t(TypeString);

    /*
     *  deklaracja zapowiadajaca
     */
    list_item_t(TypeString)* METHOD(create)          (void);
    list_item_t(TypeString)* METHOD(createFromValue) (Type value);
    void                     METHOD(release)         (list_item_t(TypeString)* _this);

#   ifdef IMPLEMENTATION

    /*
     *  konstruktor elementu listy
     */
    list_item_t(TypeString)* METHOD(create)(void)
    {
        list_item_t(TypeString)* _this = (list_item_t(TypeString)*)malloc(sizeof(list_item_t(TypeString)));

        _this->prev = NULL;
        _this->next = NULL;

        return _this;
    }
    
    /*
     *  drugi konstruktor, ustawiajacy wartosc
     */
    list_item_t(TypeString)* METHOD(createFromValue)(Type value)
    {
        list_item_t(TypeString)* _this = METHOD(create)();

        _this->value = value;

        return _this;
    }

    /*
     *  zwalnianie
     */
    void METHOD(release)(list_item_t(TypeString)* _this)
    {
        free(_this);
    }

#   endif

    /*
     *  pomocnicze makra
     */
#   ifndef list_t
#       define  list_t(TypeString)  STRUCT_TEMPLATE(list, TypeString)
#   endif

#   undef METHOD
#   define  METHOD(name)        METHOD_TEMPLATE(list, TypeString, name)

    /*
     *  struktura
     */
    typedef struct STRUCT_TEMPLATE(tag_list, TypeString) {
        READONLY STRUCT_TEMPLATE(list_item, TypeString)* first, * last;
        READONLY unsigned int size;

        Type (*objectCopyConstructor)(Type*);
        void (*objectDestructor)(Type*);
    } list_t(TypeString);

    /*
     *  deklaracje zapowiadajace
     */
    list_t(TypeString)*     METHOD(create)              (void);
    void                    METHOD(release)             (list_t(TypeString)* _this);

    void                    METHOD(pushBack)            (list_t(TypeString)* _this, Type elem);
    void                    METHOD(pushFront)           (list_t(TypeString)* _this, Type elem);
    void                    METHOD(insertAfter)         (list_t(TypeString)* _this, READONLY list_item_t(TypeString)* prev, Type elem);
    void                    METHOD(insertBefore)        (list_t(TypeString)* _this, READONLY list_item_t(TypeString)* next, Type elem);
    void                    METHOD(popBack)             (list_t(TypeString)* _this);
    void                    METHOD(popFront)            (list_t(TypeString)* _this);
    void                    METHOD(erase)               (list_t(TypeString)* _this, READONLY list_item_t(TypeString)* elem);
    void                    METHOD(clear)               (list_t(TypeString)* _this);

#   ifdef IMPLEMENTATION

        /*
         *  konstruktor
         */
        list_t(TypeString)* METHOD(create)(void)
        {
            list_t(TypeString)* _this = (list_t(TypeString)*)malloc(sizeof(list_t(TypeString)));

	        _this->first = NULL;
	        _this->last = NULL;
	        _this->size = 0;
	        _this->objectCopyConstructor = NULL;
	        _this->objectDestructor = NULL;

            return _this;
        }

        /*
         *  zwalnianie elementow
         */
        void METHOD(release)(list_t(TypeString)* _this)
        {
            assert(_this);

            METHOD(clear)(_this);

            free(_this);
        }

        /*
         *  dodawanie elementu na koniec
         */
        void METHOD(pushBack)(list_t(TypeString)* _this, Type elem)
        {
            assert(_this);

	        list_item_t(TypeString)* item = METHOD_TEMPLATE(list_item, TypeString, create)();

	        item->prev = _this->last;
	        item->next = NULL;

	        if (_this->objectCopyConstructor)
		        item->value = _this->objectCopyConstructor(&elem);
	        else
		        item->value = elem;

	        if (!_this->first)
		        _this->first = item;

	        if (_this->last)
		        _this->last->next = item;

	        _this->last = item;
	        ++_this->size;
        }
        
        /*
         *  dodawanie elementu na poczatek
         */
        void METHOD(pushFront)(list_t(TypeString)* _this, Type elem)
        {
            assert(_this);

	        list_item_t(TypeString)* item = METHOD_TEMPLATE(list_item, TypeString, create)();

	        item->prev = NULL;
	        item->next = _this->first;

	        if (_this->objectCopyConstructor)
		        item->value = _this->objectCopyConstructor(&elem);
	        else
		        item->value = elem;

	        if (!_this->last)
		        _this->last = item;

	        if (_this->first)
		        _this->first->prev = item;

	        _this->first = item;
	        ++_this->size;
        }

        /*
         *  dodawanie elementu w okreslonym miejscu
         */
        void METHOD(insertAfter)(list_t(TypeString)* _this, READONLY list_item_t(TypeString)* prev, Type elem)
        {
            assert(_this);
            assert(prev);

            list_item_t(TypeString)* item = METHOD_TEMPLATE(list_item, TypeString, createFromValue)(elem);

            item->prev = prev;
            if (prev)
            {
                item->next = prev->next;
                prev->next = item;

                if (!item->next) // nie ma nastepnego == wrzucamy na koniec
                    _this->last = item;
                else
                    item->next->prev = item;
            }
            else // nie ma poprzedniego == wrzucamy na poczatek
            {
                if (_this->first)
                    _this->first->prev = item;

                item->next = _this->first;
                _this->first = item;

                if (item->next)
                    item->next->prev = item;
            }

            ++_this->size;
        }

        void METHOD(insertBefore)(list_t(TypeString)* _this, READONLY list_item_t(TypeString)* next, Type elem)
        {
            assert(_this);
            assert(next);

            list_item_t(TypeString)* item = METHOD_TEMPLATE(list_item, TypeString, createFromValue)(elem);

            item->next = next;
            if (next)
            {
                item->prev = next->prev;
                next->prev = item;

                if (!item->prev) // nie ma poprzedniego == wrzucamy na poczatek
                    _this->first = item;
                else
                    item->prev->next = item;
            }
            else // nie ma nastepnego == wrzucamy na koniec
            {
                if (_this->last)
                    _this->last->next = item;

                item->prev = _this->last;
                _this->last = item;

                if (item->prev)
                    item->prev->next = item;
            }

            ++_this->size;
        }


        /*
         *  usuwanie ostatniego elementu
         */
        void METHOD(popBack)(list_t(TypeString)* _this)
        {
            assert(_this);
            assert(_this->last);

	        if (_this->last)
	        {
		        list_item_t(TypeString)* lastItem = _this->last;

		        if (_this->objectDestructor)
			        _this->objectDestructor(&lastItem->value);

		        _this->last = lastItem->prev;
                _this->last->next = NULL;
		        METHOD_TEMPLATE(list_item, TypeString, release)(lastItem);

                --_this->size;
	        }
        }

        /*
         *  usuwanie pierwszego elementu
         */
        void METHOD(popFront)(list_t(TypeString)* _this)
        {
            assert(_this);
            assert(_this->first);
            
	        if (_this->first)
	        {
		        list_item_t(TypeString)* firstItem = _this->first;

		        if (_this->objectDestructor)
			        _this->objectDestructor(&firstItem->value);

		        _this->first = firstItem->next;
                _this->first->prev = NULL;
		        METHOD_TEMPLATE(list_item, TypeString, release)(firstItem);

                --_this->size;
	        }
        }

        /*
         *  usuwanie dowolnego elementu
         *  ZWALNIA elem!
         */
        void METHOD(erase)(list_t(TypeString)* _this, READONLY list_item_t(TypeString)* elem)
        {
            assert(_this);
            assert(elem);

            if (elem->prev)
                elem->prev->next = elem->next;
            else // nie bylo poprzedniego, trzeba przesunac poczatek
                _this->first = elem->next;

            if (elem->next)
                elem->next->prev = elem->prev;
            else // nie bylo nastepnego, trzeba przesunac koniec
                _this->last = elem->prev;

            --_this->size;

            METHOD_TEMPLATE(list_item, TypeString, release)(elem);
        }

        /*
         *  usuwa wszystkie elementy
         */
        void METHOD(clear)(list_t(TypeString)* _this)
        {
            assert(_this);

	        if (_this->size)
	        {
		        list_item_t(TypeString)* current, * next;

		        current = NULL;
		        next = _this->first;

		        while (next)
		        {
			        current = next;
			        next = next->next;

                    if (_this->objectDestructor)
                        _this->objectDestructor(&current->value);

			        METHOD_TEMPLATE(list_item, TypeString, release)(current);
		        }

                _this->first = _this->last = NULL;
		        _this->size = 0;
	        }
        }

#   endif

#   undef METHOD
#   undef TypeString
#   undef Type

#endif
