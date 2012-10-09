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

#   ifndef hm_list_t
#       define hm_list_t(key, val)      STRUCT_TEMPLATE2(hash_map_list, key, val)
#   endif /* hm_list_t */
#   define hm_list_type             hm_list_t(KeyTypeString, ValueTypeString)

    /* internal use only */
    typedef struct STRUCT_TEMPLATE2(tag_hash_map_list, KeyTypeString, ValueTypeString) hm_list_type;

#   define TreeKeyType uint64_t
#   define TreeValueType hm_list_type*
#   define TreeValueTypeString hm_list_type
#   include "rbtree.h"

#   ifndef pair_t
#       define pair_t(first, second)    STRUCT_TEMPLATE2(pair, first, second)
#   endif /* pair_t */
#   define pair_type                pair_t(KeyTypeString, ValueTypeString)

#   ifndef hash_map_t
#       define hash_map_t(key, val)     STRUCT_TEMPLATE2(hash_map, key, val)
#   endif /* hash_map_t */
#   define hash_map_type            hash_map_t(KeyTypeString, ValueTypeString)

#   define METHOD(name)             METHOD_TEMPLATE2(hash_map, KeyTypeString, ValueTypeString, name)
#   define RBTREE_METHOD(name)      METHOD_TEMPLATE2(rbtree, uint64_t, hm_list_type, name)

#   define _hash_func_t(key)        hash_ ## key ## _func_t
#   define hash_func_t(key)         _hash_func_t(key)
#   define hash_func_type           hash_func_t(KeyTypeString)

#   define _key_comp_func_t(key)    key_comp_ ## key ## _func_t
#   define key_comp_func_t(key)     _key_comp_func_t(key)
#   define key_comp_func_type       key_comp_func_t(KeyTypeString)

    typedef uint64_t (*hash_func_type)(KeyType);
    typedef int (*key_comp_func_type)(KeyType, KeyType);

    typedef struct STRUCT_TEMPLATE2(tag_hash_map, KeyTypeString, ValueTypeString) hash_map_type;

    typedef struct STRUCT_TEMPLATE2(tag_pair, KeyTypeString, ValueTypeString) {
        KeyType first;
        ValueType second;
    } pair_type;

    /*
     *  hash_func: required
     *  key_comp_func: optional. strcmp-like (0 on equality); if key_comp_func == NULL, operator == is used
     */
    hash_map_type*      METHOD(create)(hash_func_type hash_func, key_comp_func_type key_comp_func);
    void                METHOD(release)(hash_map_type* this);

    /*
     *  NULL if key not in map
     *  if out == NULL, returns NULL (not found) or 0x1 (found)
     */
    pair_type*    METHOD(find)(const hash_map_type* this, KeyType key, pair_type* out);
    ValueType*          METHOD(get)(hash_map_type* this, KeyType key);

    void                METHOD(insert)(hash_map_type* this, KeyType key, ValueType value);
    void                METHOD(erase)(hash_map_type* this, KeyType key);

#   ifdef IMPLEMENTATION

#       include <stdlib.h>
#       include <assert.h>

        struct STRUCT_TEMPLATE2(tag_hash_map_list, KeyTypeString, ValueTypeString) {
            KeyType key;
            ValueType value;
            hm_list_type* next;
        };

        struct STRUCT_TEMPLATE2(tag_hash_map, KeyTypeString, ValueTypeString) {
            rbtree_t(uint64_t, hm_list_type)* tree;
            hash_func_type hash_func;
            key_comp_func_type key_comp_func;
        };


        hash_map_type* METHOD(create)(hash_func_type hash_func, key_comp_func_type key_comp_func)
        {
            assert(hash_func && "hash_func must not be null");

            hash_map_type* map = (hash_map_type*)malloc(sizeof(hash_map_type));

            map->tree = RBTREE_METHOD(create)();
            map->hash_func = hash_func;
            map->key_comp_func = key_comp_func;

            return map;
        }

        void METHOD(release)(hash_map_type* this)
        {
            RBTREE_METHOD(release)(this->tree);
            free(this);
        }


        /* NULL if key not in map */
        pair_type* METHOD(find)(const hash_map_type* this, KeyType key, pair_type* out)
        {
            assert(this->hash_func && "hash_func must not be null");
            uint64_t hash = this->hash_func(key);

            rbtree_node_t(uint64_t, hm_list_type)* node = RBTREE_METHOD(find)(this->tree, hash);
            
            if (!node)
                return NULL;

            hm_list_type* list = node->value;

            if (this->key_comp_func)
            {
                while (list && this->key_comp_func(key, list->key))
                    list = list->next;
            }
            else
            {
                while (list && key != list->key)
                    list = list->next;
            }

            if (list)
            {
                if (out)
                {
                    out->first = key;
                    out->second = list->value;
                    return out;
                }
                else
                    return (pair_type*)1; /* found */
            }
            
            return NULL;
        }

        ValueType* METHOD(get)(hash_map_type* this, KeyType key)
        {
            assert(this->hash_func && "hash_func must not be null");
            uint64_t hash = this->hash_func(key);

            rbtree_node_t(uint64_t, hm_list_type)* node = RBTREE_METHOD(find)(this->tree, hash);

            if (!node)
                return NULL;

            hm_list_type* list = node->value;

            if (this->key_comp_func)
            {
                while (list && this->key_comp_func(key, list->key))
                    list = list->next;
            }
            else
            {
                while (list && key != list->key)
                    list = list->next;
            }

            if (list)
                return &list->value;
            
            return NULL;
        }


        void METHOD(insert)(hash_map_type* this, KeyType key, ValueType value)
        {
            assert(this->hash_func && "hash_func must not be null");
            uint64_t hash = this->hash_func(key);

            rbtree_node_t(uint64_t, hm_list_type)* node = RBTREE_METHOD(find)(this->tree, hash);

            hm_list_type* list = (hm_list_type*)malloc(sizeof(hm_list_type));
            list->key = key;
            list->value = value;
            list->next = NULL;

            if (!node)
                RBTREE_METHOD(insert)(this->tree, hash, list);
            else
            {
                hm_list_type* curr = node->value;
                while (curr->next)
                    curr = curr->next;

                curr->next = list;
            }
        }

        void METHOD(erase)(hash_map_type* this, KeyType key)
        {
            assert(this->hash_func && "hash_func must not be null");
            uint64_t hash = this->hash_func(key);

            rbtree_node_t(uint64_t, hm_list_type)* node = RBTREE_METHOD(find)(this->tree, hash);
            assert(node && "erasing not existing key");

            hm_list_type** list = &node->value;
            if (this->key_comp_func)
            {
                while (*list && this->key_comp_func(key, (*list)->key))
                    list = &(*list)->next;
            }
            else
            {
                while (*list && key != (*list)->key)
                    list = &(*list)->next;
            }

            assert(*list && "erasing not existing key");

            hm_list_type* to_free = *list;
            *list = (*list)->next;
            free(to_free);
        }


#   endif /* IMPLEMENTATION */

#   undef hash_func_type
#   undef hash_func_t
#   undef _hash_func_t

#   undef key_comp_func_type
#   undef key_comp_func_t
#   undef _key_comp_func_t

#   undef RBTREE_METHOD
#   undef METHOD

#   undef hash_map_type
#   undef pair_type
#   undef hash_map_list

#   undef KeyTypeString
#   undef ValueTypeString
#   undef KeyType
#   undef ValueType

#endif /* defined KeyType && defined ValueType */
