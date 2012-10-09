/*
 *  modified red-black tree by Emin Martinian
 *  from http://www.mit.edu/~emin/source_code/red_black_tree/index.html
 */

#include "../object_macros.h"

#if !defined(TreeKeyType) || !defined(TreeValueType)
#   pragma message("WARNING: at least TreeKeyType and TreeValueType must be defined!")
#else

#   if !defined(TreeKeyTypeString) && defined(TreeKeyType)
#       define TreeKeyTypeString TreeKeyType
#   endif
#   if !defined(TreeValueTypeString) && defined(TreeValueType)
#       define TreeValueTypeString TreeValueType
#   endif

#   define rbtree_node_t(key, val)  STRUCT_TEMPLATE2(rbtree_node, key, val)
#   define rbtree_t(key, val)       STRUCT_TEMPLATE2(rbtree, key, val)

/*  pomonicze */
#   define rbtree_node_type         rbtree_node_t(TreeKeyTypeString, TreeValueTypeString)
#   define rbtree_type              rbtree_t(TreeKeyTypeString, TreeValueTypeString)

#   define METHOD(name)             METHOD_TEMPLATE2(rbtree, TreeKeyTypeString, TreeValueTypeString, name)

    typedef struct STRUCT_TEMPLATE2(tag_rbtree_node, TreeKeyTypeString, TreeValueTypeString) rbtree_node_t(TreeKeyTypeString, TreeValueTypeString);
    typedef struct STRUCT_TEMPLATE2(tag_rbtree, TreeKeyTypeString, TreeValueTypeString) rbtree_t(TreeKeyTypeString, TreeValueTypeString);

    struct STRUCT_TEMPLATE2(tag_rbtree_node, TreeKeyTypeString, TreeValueTypeString) {
        TreeKeyType key;
        TreeValueType value;

        PRIVATE_START
            int is_red;
            rbtree_node_type* left;
            rbtree_node_type* right;
            rbtree_node_type* parent;
        PRIVATE_END
    };


    rbtree_type*        METHOD(create)              (void);
    void                METHOD(release_node)        (rbtree_type* tree, rbtree_node_type* node);
    void                METHOD(release)             (rbtree_type* tree);

    void                METHOD(insert_node)         (rbtree_type* tree, rbtree_node_type* node);
    rbtree_node_type*   METHOD(insert)              (rbtree_type* tree, TreeKeyType key, TreeValueType value);
    void                METHOD(erase_restore_rb)    (rbtree_type* tree, rbtree_node_type* node);
    void                METHOD(erase_node)          (rbtree_type* tree, rbtree_node_type* node);

    rbtree_node_type*   METHOD(predecessor)         (rbtree_type* tree, rbtree_node_type* node);
    rbtree_node_type*   METHOD(successor)           (rbtree_type* tree, rbtree_node_type* node);
    /* returns tree->null when node not found */
    rbtree_node_type*   METHOD(find_node)           (rbtree_type* tree, TreeKeyType key);
    /* returns NULL when node not found */
    rbtree_node_type*   METHOD(find)                (rbtree_type* tree, TreeKeyType key);

    void                METHOD(rotate_left)         (rbtree_type* tree, rbtree_node_type* node);
    void                METHOD(rotate_right)        (rbtree_type* tree, rbtree_node_type* node);


#   ifdef IMPLEMENTATION

#       include <stdlib.h>
#       include <assert.h>


        struct STRUCT_TEMPLATE2(tag_rbtree, TreeKeyTypeString, TreeValueTypeString) {
            rbtree_node_type* root;
            rbtree_node_type* null;
        };


        rbtree_type* METHOD(create)(void)
        {
            rbtree_type* tree = (rbtree_type*)malloc(sizeof(rbtree_type));
            tree->null = (rbtree_node_type*)malloc(sizeof(rbtree_node_type));

            tree->null->parent = tree->null->left = tree->null->right = tree->null;
            tree->null->key = 0;
            tree->null->is_red = 0;

            tree->root = (rbtree_node_type*)malloc(sizeof(rbtree_node_type));
            tree->root->parent = tree->root->left = tree->root->right = tree->null;
            tree->root->is_red = 0;

            return tree;
        }

        void METHOD(release_node)(rbtree_type* tree, rbtree_node_type* node)
        {
            if (node != tree->null)
            {
                METHOD(release_node)(tree, node->left);
                METHOD(release_node)(tree, node->right);
                free(node);
            }
        }

        void METHOD(release)(rbtree_type* tree)
        {
            METHOD(release_node)(tree, tree->root->left);
            free(tree->null);
            free(tree->root);
            free(tree);
        }


        void METHOD(insert_node)(rbtree_type* tree, rbtree_node_type* node)
        {
            rbtree_node_type* curr;
            rbtree_node_type* next;

            node->left = node->right = tree->null;
            curr = tree->root;
            next = tree->root->left;

            while (next != tree->null)
            {
                curr = next;
                if (next->key > node->key)
                    next = next->left;
                else if (next->key < node->key)
                    next = next->right;
                else
                    assert(!"node already in tree!");
        #if 0
                    return; /* node already in tree */
        #endif 
            }

            node->parent = curr;

            if ((curr == tree->root) || (curr->key > node->key))
                curr->left = node;
            else
                curr->right = node;

            assert(!tree->null->is_red && "null not black in rbtree_insert_node");
        }

        rbtree_node_type* METHOD(insert)(rbtree_type* tree, TreeKeyType key, TreeValueType value)
        {
            rbtree_node_type* x;
            rbtree_node_type* y;
            rbtree_node_type* node;

            node = (rbtree_node_type*)malloc(sizeof(rbtree_node_type));
            node->key = key;
            node->value = value;

            METHOD(insert_node)(tree, node);
            node->is_red = 1;

            x = node;

            while (x->parent->is_red)
            {
                if (x->parent == x->parent->parent->left)
                {
                    y = x->parent->parent->right;
                    if (y->is_red)
                    {
                        x->parent->is_red = 0;
                        y->is_red = 0;
                        x->parent->parent->is_red = 1;
                        x = x->parent->parent;
                    }
                    else
                    {
                        if (x == x->parent->right)
                        {
                            x = x->parent;
                            METHOD(rotate_left)(tree, x);
                        }

                        x->parent->is_red = 0;
                        x->parent->parent->is_red = 1;
                        METHOD(rotate_right)(tree, x->parent->parent);
                    }
                }
                else /* x->parent == x->parent->parent->right */
                {
                    y = x->parent->parent->left;

                    if (y->is_red)
                    {
                        x->parent->is_red = 0;
                        y->is_red = 0;
                        x->parent->parent->is_red = 1;
                        x = x->parent->parent;
                    }
                    else
                    {
                        if (x == x->parent->left)
                        {
                            x = x->parent;
                            METHOD(rotate_right)(tree, x);
                        }

                        x->parent->is_red = 0;
                        x->parent->parent->is_red = 1;
                        METHOD(rotate_left)(tree, x->parent->parent);
                    }
                }
            }

            tree->root->left->is_red = 0;

            assert(!tree->null->is_red && "null not black in rbtree_insert");
            assert(!tree->root->is_red && "root not black in rbtree_insert");
            return node;
        }

        void METHOD(erase_restore_rb)(rbtree_type* tree, rbtree_node_type* node)
        {
            rbtree_node_type* root = tree->root->left;
            rbtree_node_type* w;

            while ((!node->is_red) && (root != node))
            {
                if (node == node->parent->left)
                {
                    w = node->parent->right;
                    if (w->is_red)
                    {
                        w->is_red = 0;
                        node->parent->is_red = 1;
                        METHOD(rotate_left)(tree, node->parent);
                        w = node->parent->right;
                    }

                    if ((!w->right->is_red) && (!w->left->is_red))
                    {
                        w->is_red = 1;
                        node = node->parent;
                    }
                    else
                    {
                        if (!w->right->is_red)
                        {
                            w->left->is_red = 0;
                            w->is_red = 1;
                            METHOD(rotate_right)(tree, w);
                            w = node->parent->right;
                        }

                        w->is_red = node->parent->is_red;
                        node->parent->is_red = 0;
                        w->right->is_red = 0;
                        METHOD(rotate_left)(tree, node->parent);
                        node = root;
                    }
                }
                else
                {
                    w = node->parent->left;
                    if (w->is_red)
                    {
                        w->is_red = 0;
                        node->parent->is_red = 1;
                        METHOD(rotate_right)(tree, node->parent);
                        w = node->parent->left;
                    }

                    if ((!w->right->is_red) && (!w->left->is_red))
                    {
                        w->is_red = 1;
                        node = node->parent;
                    }
                    else
                    {
                        if (!w->left->is_red)
                        {
                            w->right->is_red = 0;
                            w->is_red = 1;
                            METHOD(rotate_left)(tree, w);
                            w = node->parent->left;
                        }

                        w->is_red = node->parent->is_red;
                        node->parent->is_red = 0;
                        w->left->is_red = 0;
                        METHOD(rotate_right)(tree, node->parent);
                        node = root;
                    }
                }
            }

            node->is_red = 0;

            assert(!tree->null->is_red && "null not black @ rbtree_erase_restore_rb");
        }

        void METHOD(erase)(rbtree_type* tree, rbtree_node_type* node)
        {
            rbtree_node_type* y;
            rbtree_node_type* x;
            rbtree_node_type* root = tree->root;

            y = ((node->left == tree->null) || (node->right == tree->null)) ? node : METHOD(successor)(tree, node);
            x = (y->left == tree->null) ? y->right : y->left;

            if (root == (x->parent = y->parent))
                root->left = x;
            else
            {
                if (y == y->parent->left)
                    y->parent->left = x;
                else
                    y->parent->right = x;
            }

            if (y != node)
            {
                assert(y != tree->null && "y is null @ rbtree_erase");

                if (!y->is_red)
                    METHOD(erase_restore_rb)(tree, x);

                y->left = node->left;
                y->right = node->right;
                y->parent = node->parent;
                y->is_red = node->is_red;
                node->left->parent = node->right->parent = y;

                if (node == node->parent->left)
                    node->parent->left = y;
                else
                    node->parent->right = y;

                free(node);
            }
            else
            {
                if (!y->is_red)
                    METHOD(erase_restore_rb)(tree, x);
                free(y);
            }

            assert(!tree->null->is_red && "null not black @ rbtree_erase");
        }


        rbtree_node_type* METHOD(predecessor)(rbtree_type* tree, rbtree_node_type* node)
        {
            rbtree_node_type* y;
            rbtree_node_type* root = tree->root;

            if (tree->null != (y = node->left))
            {
                while (y->right != tree->null)
                    y = y->right;
                return y;
            }
            else
            {
                y = node->parent;
                while (node == node->left)
                {
                    if (y == root)
                        return tree->null;
                    node = y;
                    y = y->parent;
                }

                return y;
            }
        }

        rbtree_node_type* METHOD(successor)(rbtree_type* tree, rbtree_node_type* node)
        {
            rbtree_node_type* y;
            rbtree_node_type* root = tree->root;

            if (tree->null != (y = node->right))
            {
                while (y->left != tree->null)
                    y = y->left;
                return y;
            }
            else
            {
                y = node->parent;
                while (node == y->right)
                {
                    node = y;
                    y = y->parent;
                }

                if (y == root)
                    return tree->null;
                return y;
            }
        }

        rbtree_node_type* METHOD(find_node)(rbtree_type* tree, TreeKeyType key)
        {
            rbtree_node_type* curr = tree->root->left;

            while (curr != tree->null)
            {
                if (curr->key < key)
                    curr = curr->right;
                else if (curr->key > key)
                    curr = curr->left;
                else
                    return curr;
            }

            return tree->null;
        }

        rbtree_node_type* METHOD(find)(rbtree_type* tree, TreeKeyType key)
        {
            rbtree_node_type* curr = tree->root->left;
            
            while (curr != tree->null)
            {
                if (curr->key < key)
                    curr = curr->right;
                else if (curr->key > key)
                    curr = curr->left;
                else
                    return curr;
            }

            return NULL;
        }

        void METHOD(rotate_left)(rbtree_type* tree, rbtree_node_type* node)
        {
            rbtree_node_type* tmp;

            tmp = node->right;
            node->right = tmp->left;

            if (tmp->left != tree->null)
                tmp->left->parent = node;

            tmp->parent = node->parent;

            if (node == node->parent->left)
                node->parent->left = tmp;
            else
                node->parent->right = tmp;

            tmp->left = node;
            node->parent = tmp;

            assert(!tree->null->is_red && "null not black @ rbtree_rotate_left");
        }

        void METHOD(rotate_right)(rbtree_type* tree, rbtree_node_type* node)
        {
            rbtree_node_type* tmp;

            tmp = node->left;
            node->left = tmp->right;

            if (tree->null != tmp->right)
                tmp->right->parent = node;

            tmp->parent = node->parent;

            if (node == node->parent->left)
                node->parent->left = tmp;
            else
                node->parent->right = tmp;

            tmp->right = node;
            node->parent = tmp;

            assert(!tree->null->is_red && "null not black @ rbtree_rotate_right");

        }

#   endif /* IMPLEMENTATION */

#   undef METHOD
#   undef rbtree_type
#   undef rbtree_node_type
#   undef TreeValueTypeString
#   undef TreeKeyTypeString

#   ifndef SKIP_TYPE_UNDEFS
#       undef TreeValueType
#       undef TreeKeyType
#   endif /* SKIP_TYPE_UNDEFS */

#endif /* defined TreeKeyType && defined TreeValueType */

