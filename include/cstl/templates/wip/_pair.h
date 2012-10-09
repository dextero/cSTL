#include <stdlib.h>

#include "../object_macros.h"


#if !defined(TypeString1) && defined(Type1)
#   define TypeString1 Type1
#endif
#if !defined(TypeString2) && defined(Type2)
#   define TypeString2 Type2
#endif

#if !defined(Type1) || !defined(Type2)
#   pragma message("WARNING: At least Type1 and Type2 must be defined!")
#else

    /*
     *  deklaracja TypeString
     */
#   define  __TypeString(a, b)  a ## _ ## b
#   define  _TypeString(a, b)   __TypeString(a, b)
#   define  TypeString          _TypeString(TypeString1, TypeString2)

#   ifndef pair_t
#       define  _pair_t(a, b)                       STRUCT_TEMPLATE(pair, a ## _ ## b)
#       define  pair_t(TypeString1, TypeString2)    _pair_t(TypeString1, TypeString2)
#   endif

#   define  METHOD(name)                        METHOD_TEMPLATE(Pair, TypeString, name)

    typedef struct pair_t(TypeString1, TypeString2) pair_t(TypeString1, TypeString2);

    pair_t(TypeString1, TypeString2)*   METHOD(Create)              ();
    pair_t(TypeString1, TypeString2)*   METHOD(CreateFromValues)    (Type1 first, Type2 second);
    void                                METHOD(Release)             (pair_t(TypeString1, TypeString2)* this);

    Type1                               METHOD(First)               (const pair_t(TypeString1, TypeString2)* this);
    Type2                               METHOD(Second)              (const pair_t(TypeString1, TypeString2)* this);

    void                                METHOD(SetFirst)            (pair_t(TypeString1, TypeString2)* this, Type1 first);
    void                                METHOD(SetSecond)           (pair_t(TypeString1, TypeString2)* this, Type2 second);


#   ifdef IMPLEMENTATION

        typedef struct pair_t(TypeString1, TypeString2) {
            Type1 first;
            Type2 second;
        } pair_t(TypeString1, TypeString2);

        
        pair_t(TypeString1, TypeString2)* METHOD(Create)()
        {
            pair_t(TypeString1, TypeString2)* this = (pair_t(TypeString1, TypeString2)*)malloc(sizeof(pair_t(TypeString1, TypeString2)));

            return this;
        }

        pair_t(TypeString1, TypeString2)* METHOD(CreateFromValues)(Type1 first, Type2 second)
        {
            pair_t(TypeString1, TypeString2)* this = METHOD(Create)();

            this->first = first;
            this->second = second;

            return this;
        }

        void METHOD(Release)(pair_t(TypeString1, TypeString2)* this)
        {
            free(this);
        }


        Type1 METHOD(First)(const pair_t(TypeString1, TypeString2)* this)
        {
            return this->first;
        }

        Type2 METHOD(Second)(const pair_t(TypeString1, TypeString2)* this)
        {
            return this->second;
        }


        void METHOD(SetFirst)(pair_t(TypeString1, TypeString2)* this, Type1 first)
        {
            this->first = first;
        }

        void METHOD(SetSecond)(pair_t(TypeString1, TypeString2)* this, Type2 second)
        {
            this->second = second;
        }

#   endif

#   undef METHOD
#   undef  TypeString
#   ifndef SKIP_TYPE_UNDEFS
#       undef  __TypeString
#       undef  _TypeString
#       undef  TypeString1
#       undef  TypeString2
#       undef  Type1
#       undef  Type2
#       undef SKIP_TYPE_UNDEFS
#   endif

#endif