#include <stdlib.h>

#include "../object_macros.h"

#if !defined(TypeString) && defined(Type)
#   define TypeString Type
#endif

#if !defined(Type)
#   pragma message("WARNING: At least Type must be defined!")
#else

#   ifndef queueItem_t
#       define queueItem_t(TypeString)  STRUCT_TEMPLATE(queueItem, TypeString)
#   endif

#   ifdef IMPLEMENTATION

        typedef struct STRUCT_TEMPLATE(tag_queueItem, TypeString) {
	        Type value;
	        struct STRUCT_TEMPLATE(tag_queueItem, TypeString)* prev;
        } queueItem_t(TypeString);

#   endif /* IMPLEMENTATION */

#   ifndef queue_t
#       define queue_t(TypeString)  STRUCT_TEMPLATE(queue, TypeString)
#   endif

#   define METHOD(name) METHOD_TEMPLATE(queue, TypeString, name)

    typedef struct STRUCT_TEMPLATE(queue, TypeString) {
#   ifdef IMPLEMENTATION
        queueItem_t(TypeString)* front;
        queueItem_t(TypeString)* back;
#   else
        READONLY Type* front;
        READONLY Type* back;
#   endif /* IMPLEMENTATION */

        READONLY unsigned int size;

        Type (*objectCopyConstructor)(Type*);
        Type (*objectDestructor)(Type*);
    } queue_t(TypeString);


    /*
     *  deklaracje zapowiadajace
     */
    queue_t(TypeString)*    METHOD(create)  ();
    void                    METHOD(release) (queue_t(TypeString)* _this);

    void    METHOD(push)    (queue_t(TypeString)* _this, Type elem);
    void    METHOD(pop)     (queue_t(TypeString)* _this);
    void    METHOD(clear)   (queue_t(TypeString)* _this);


#   ifdef IMPLEMENTATION

        /*
         *  konstruktor
         */
        queue_t(TypeString)* METHOD(create)()
        {
            queue_t(TypeString)* _this = (queue_t(TypeString)*)malloc(sizeof(queue_t(TypeString)));
            
	        _this->front = NULL;
            _this->back = NULL;
	        _this->size = 0;
	        _this->objectCopyConstructor = NULL;
	        _this->objectDestructor = NULL;

            return _this;
        }

        /*
         *  dodawanie elementu na stos
         */
        void METHOD(push)(queue_t(TypeString)* _this, Type elem)
        {
	        queueItem_t(TypeString)* item = (queueItem_t(TypeString)*)malloc(sizeof(queueItem_t(TypeString)));

            item->prev = NULL;

            if (!_this->front)
                _this->front = item;

            if (_this->back)
                _this->back->prev = item;

            _this->back = item;

	        if (_this->objectCopyConstructor)
		        item->value = _this->objectCopyConstructor(&elem);
	        else
		        item->value = elem;

	        ++_this->size;
        }

        /*
         *  usuwanie elementu ze stosu
         */
        void METHOD(pop)(queue_t(TypeString)* _this)
        {
	        if (_this->front)
	        {
		        queueItem_t(TypeString)* lastItem = _this->front;

		        if (_this->objectDestructor)
			        _this->objectDestructor(&lastItem->value);

                if (_this->front == _this->back)
                    _this->back = NULL;

		        _this->front = _this->front->prev;
		        free(lastItem);

                --_this->size;
	        }
        }

        /*
         *  zwalnianie elementow
         */
        void METHOD(release)(queue_t(TypeString)* _this)
        {
            METHOD(clear)(_this);

            free(_this);
        }

        void METHOD(clear)(queue_t(TypeString)* _this)
        {
	        if (_this->size)
	        {
		        queueItem_t(TypeString)* current, * prev;

		        current = NULL;
		        prev = _this->front;

		        while (prev)
		        {
			        current = prev;
			        prev = prev->prev;

                    if (_this->objectDestructor)
                        _this->objectDestructor(&current->value);

			        free(current);
		        }

                _this->front = _this->back = NULL;
		        _this->size = 0;
	        }
        }

#   endif // IMPLEMENTATION

#undef METHOD
#undef Type
#undef TypeString

#endif
