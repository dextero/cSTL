#include <stdlib.h>
#include <assert.h>

#include "../object_macros.h"

#if !defined(TypeString) && defined(Type)
#   define TypeString Type
#endif

#if !defined(Type)
#   pragma message("WARNING: At least Type must be defined!")
#else

#   ifdef IMPLEMENTATION
#       ifndef stackItem_t
#           define stackItem_t(TypeString)  STRUCT_TEMPLATE(stackItem, TypeString)
#       endif

        typedef struct STRUCT_TEMPLATE(tag_stackItem, TypeString) {
	        Type value;
            struct STRUCT_TEMPLATE(tag_stackItem, TypeString)* prev;
        } stackItem_t(TypeString);

#   endif


#   ifndef stack_t
#       define stack_t(TypeString)  STRUCT_TEMPLATE(stack, TypeString)
#   endif

#   define METHOD(name) METHOD_TEMPLATE(stack, TypeString, name)

    /*
     *  struktura
     */
    typedef struct STRUCT_TEMPLATE(tag_stack, TypeString) {
#   ifdef IMPLEMENTATION
            stackItem_t(TypeString)* top;
#   else
            Type* top;  /* hack, value jest pierwszym polem stackItem */
#   endif /* IMPLEMENTATION */

        READONLY unsigned int size;

        Type (*objectCopyConstructor)(Type*);
        Type (*objectDestructor)(Type*);
    } stack_t(TypeString);

    /*
     *  deklaracje zapowiadajace
     */
    stack_t(TypeString)*    METHOD(create)  (void);
    void                    METHOD(release) (stack_t(TypeString)* _this);

    void            METHOD(push)    (stack_t(TypeString)* _this, Type elem);
    void            METHOD(pop)     (stack_t(TypeString)* _this);
    void            METHOD(clear)   (stack_t(TypeString)* _this);

#   ifdef IMPLEMENTATION

        /*
         *  konstruktor
         */
        stack_t(TypeString)* METHOD(create)(void)
        {
            stack_t(TypeString)* ret = (stack_t(TypeString)*)malloc(sizeof(stack_t(TypeString)));
	        ret->top = NULL;
	        ret->size = 0;
	        ret->objectCopyConstructor = NULL;
	        ret->objectDestructor = NULL;
            
            return ret;
        }

        /*
         *  zwalnianie stosu
         */
        void METHOD(release)(stack_t(TypeString)* _this)
        {
            METHOD(clear)(_this);
	   
            free(_this);
        }

        /*
         *  dodawanie elementu na stos
         */
        void METHOD(push)(stack_t(TypeString)* _this, Type elem)
        {
	        stackItem_t(TypeString)* item = (stackItem_t(TypeString)*)malloc(sizeof(stackItem_t(TypeString)));
	        item->prev = _this->top;

	        if (_this->objectCopyConstructor)
		        item->value = _this->objectCopyConstructor(&elem);
	        else
		        item->value = elem;

	        _this->top = item;
	        ++_this->size;
        }

        /*
         *  usuwanie elementu ze stosu
         */
        void METHOD(pop)(stack_t(TypeString)* _this)
        {
	        if (_this->top)
	        {
		        stackItem_t(TypeString)* lastItem = _this->top;

		        if (_this->objectDestructor)
			        _this->objectDestructor(&lastItem->value);

		        _this->top = lastItem->prev;
		        free(lastItem);

                --_this->size;
	        }
        }

    void METHOD(clear)(stack_t(TypeString)* _this)
    {
        if (_this->size)
        {
            stackItem_t(TypeString)* current, * prev;

            current = NULL;
            prev = _this->top;

            while (prev)
            {
                current = prev;
                prev = prev->prev;

                if (_this->objectDestructor)
                    _this->objectDestructor(&current->value);

                free(current);
            }

            _this->top = NULL;
            _this->size = 0;
        }
    }

#   endif /* IMPLEMENTATION */

#   undef METHOD
#   undef Type
#   undef TypeString

#endif
