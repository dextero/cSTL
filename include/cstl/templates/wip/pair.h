#include "../object_macros.h"

#include <stdint.h>

#if !defined(KeyType) || !defined(ValueType)
#   pragma message("WARNING: at least KeyType and ValueType must be defined!")
#else

#   if !defined(KeyTypeString) && defined(KeyType)
#       define KeyTypeString KeyType
#   endif
#   if !defined(ValueTypeString) && defined(ValueType)
#       define ValueTypeString ValueType
#   endif

#   ifndef pair_t
#       define pair_t(first, second)    STRUCT_TEMPLATE2(pair, first, second)
#   endif /* pair_t */

    typedef struct STRUCT_TEMPLATE2(tag_pair, KeyTypeString, ValueTypeString) {
        KeyType first;
        ValueType second;
    } pair_type;

#   ifndef SKIP_TYPE_UNDEFS
#       undef KeyTypeString
#       undef ValueTypeString
#       undef KeyType
#       undef ValueType
#   else
#       undef SKIP_TYPE_UNDEFS
#   endif /* SKIP_TYPE_UNDEFS */

#endif /* defined KeyType && defined ValueType */
