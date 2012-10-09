#ifndef __OBJECT_MACROS_H__
#define __OBJECT_MACROS_H__

#define STRUCT_TEMPLATE_(name, type)                name ## _ ## type ## _t
#define STRUCT_TEMPLATE(name, type)                 STRUCT_TEMPLATE_(name, type)

#define STRUCT_TEMPLATE2_(name, type1, type2)       name ## _ ## type1 ## _ ## type2 ## _t
#define STRUCT_TEMPLATE2(name, type1, type2)        STRUCT_TEMPLATE2_(name, type1, type2)

#define METHOD_TEMPLATE_(name, type, func)          name ## _ ## type ## _ ## func
#define METHOD_TEMPLATE(name, type, func)           METHOD_TEMPLATE_(name, type, func)

#define METHOD_TEMPLATE2_(name, type1, type2, func) name ## _ ## type1 ## _ ## type2 ## _ ## func
#define METHOD_TEMPLATE2(name, type1, type2, func)  METHOD_TEMPLATE2_(name, type1, type2, func)

#define NEW(type)	                                (type*)malloc(sizeof(type))
#define NEW_ARR(type, size)	                        (type*)malloc(sizeof(type) * size)


#ifdef IMPLEMENTATION
#   define PRIVATE_START
#   define PRIVATE_END
#   define READONLY
#else
#   define PRIVATE_START    const char _private_data[sizeof(struct {
#   define PRIVATE_END      })];
#   define READONLY         const
#endif /* IMPLEMENTATION */

#endif // __OBJECT_MACROS_H__

