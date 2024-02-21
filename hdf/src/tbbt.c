/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* "tbbt.c" -- Routines for using threaded, balanced, binary trees. */
/* Extended from (added threads to) Knuth 6.2.3, Algorithm A (AVL trees) */
/* Basic tree structure by Adel'son-Vel'skii and Landis */

#include "hdf_priv.h"
#include "tbbt_priv.h"

#define PARENT 0
#define LEFT   1
#define RIGHT  2

/* Quick hack to implement PIMPL pattern for TBBT nodes
 *
 * Simple copy-n-paste, so not as robust as a function-like macro (but
 * simpler to read and looks more like the original code)
 */
#define Flags  priv->flags
#define Link   priv->link
#define Rcnt   priv->rcnt
#define Lcnt   priv->lcnt
#define Parent Link[PARENT]
#define Lchild Link[LEFT]
#define Rchild Link[RIGHT]

/* Quick hack to implement PIMPL pattern for TBBT trees
 *
 * Simple copy-n-paste, so not as robust as a function-like macro (but
 * simpler to read and looks more like the original code)
 */
#define Count        priv->count
#define Fast_compare priv->fast_compare
#define Compar       priv->compar
#define Cmparg       priv->cmparg

#define TBBT_FLAG unsigned long
#define TBBT_LEAF unsigned long

/* TBBT flag values */
#define TBBT_HEAVY(s) s /* If the `s' sub-tree is deeper than the other */
#define TBBT_DOUBLE   4 /* If "heavy" sub-tree is two levels deeper */
#define TBBT_INTERN   8 /* If node is internal (has two children) */
#define TBBT_UNBAL    (TBBT_HEAVY(LEFT) | TBBT_HEAVY(RIGHT))
#define TBBT_FLAGS    (TBBT_UNBAL | TBBT_INTERN | TBBT_DOUBLE)
#define TBBT_CHILD(s) (TBBT_INTERN | TBBT_HEAVY(s))

#define LeftCnt(node)  ((node)->Lcnt) /* Left descendants */
#define RightCnt(node) ((node)->Rcnt) /* Left descendants */
#define Cnt(node, s)   (LEFT == (s) ? LeftCnt(node) : RightCnt(node))
#define HasChild(n, s) (Cnt(n, s) > 0)
#define Heavy(n, s)    ((s) & (LeftCnt(n) > RightCnt(n) ? LEFT : LeftCnt(n) == RightCnt(n) ? 0 : RIGHT))
#define Intern(n)      (LeftCnt(n) && RightCnt(n))
#define UnBal(n)       (LeftCnt(n) > RightCnt(n) ? LEFT : LeftCnt(n) == RightCnt(n) ? 0 : RIGHT)
#define Double(n)      (TBBT_DOUBLE & (n)->Flags)
#define Other(side)    (LEFT + RIGHT - (side))
#define Delta(n, s)    ((Heavy(n, s) ? 1 : -1) * (Double(n) ? 2 : UnBal(n) ? 1 : 0))

#define SetFlags(n, s, b, i)                                                                                 \
    ((-2 < (b) && (b) < 2 ? 0 : TBBT_DOUBLE) |                                                               \
     (0 > (b)   ? TBBT_HEAVY(s)                                                                              \
      : (b) > 0 ? TBBT_HEAVY(Other(s))                                                                       \
                : 0) |                                                                                       \
     ((i) ? TBBT_INTERN : 0))

/* Return maximum of two scalar values */
#define Max(a, b) ((a) > (b) ? (a) : (b))

/* Private TBBT information */
typedef struct tbbt_node_private {
    TBBT_NODE *link[3]; /* Pointers to parent, left child, and right child */
    TBBT_FLAG  flags;   /* TBBT flags: (see above) */
    TBBT_LEAF  lcnt;    /* Count of left children */
    TBBT_LEAF  rcnt;    /* Count of right children */
} TBBT_NODE_PRIV;

typedef struct tbbt_tree_private {
    unsigned long count;        /* The number of nodes in the tree currently */
    unsigned      fast_compare; /* Use a faster in-line compare (with casts) instead of function call */
    int (*compar)(void *k1, void *k2, int cmparg);
    int cmparg;
} TBBT_TREE_PRIV;

/* Pointer to the tbbt node free list */
static TBBT_NODE *tbbt_free_list = NULL;

#define KEYcmp(k1, k2, a)                                                                                    \
    ((NULL != compar) ? (*compar)(k1, k2, a) : memcmp(k1, k2, 0 < (a) ? (a) : (int)strlen(k1)))

void tbbt1dump(TBBT_NODE *node, int method);

/* Function Prototypes */
extern void tbbt_printNode(TBBT_NODE *node, void (*key_dump)(void *, void *));
extern void tbbt_dumpNode(TBBT_NODE *node, void (*key_dump)(void *, void *), int method);
extern void tbbt_dump(TBBT_TREE *ptree, void (*key_dump)(void *, void *), int method);

static TBBT_NODE *tbbt_get_node(void);
static void       tbbt_release_node(TBBT_NODE *nod);

/* Returns pointer to end-most (to LEFT or RIGHT) node of tree: */
static TBBT_NODE *
tbbt_end(TBBT_NODE *root, int side)
{
    if (root == NULL)
        return NULL;
    while (HasChild(root, side)) {
        root = root->Link[side];
    }
    return root;
}

TBBT_NODE *
tbbtfirst(TBBT_NODE *root)
{
    return tbbt_end(root, LEFT);
}

TBBT_NODE *
tbbtlast(TBBT_NODE *root)
{
    return tbbt_end(root, RIGHT);
}

/* Returns pointer to neighboring node (to LEFT or RIGHT): */
static TBBT_NODE *
tbbt_nbr(TBBT_NODE *ptr, int side)
{
    /* return( *tbbt_anbr(ptr,side) ); */

    if (!HasChild(ptr, side))
        return ptr->Link[side];
    /*        return NULL; */
    ptr = ptr->Link[side];
    if (ptr == NULL)
        return NULL;
    while (HasChild(ptr, Other(side)))
        ptr = ptr->Link[Other(side)];
    return ptr;
}

/* Returns pointer to node with previous key value: */
TBBT_NODE *
tbbtnext(TBBT_NODE *node)
{
    return tbbt_nbr(node, RIGHT);
}

/* Returns pointer to node with next key value: */
TBBT_NODE *
tbbtprev(TBBT_NODE *node)
{
    return tbbt_nbr(node, LEFT);
}

/* tbbtfind -- Look up a node in a tree based on a key value */
/* tbbtffind is based on this routine - fix bugs in both places! */
/* Returns a pointer to the found node (or NULL) */
TBBT_NODE *
tbbtfind(TBBT_NODE *root, void *key, int (*compar)(void *, void *, int), int arg, TBBT_NODE **pp)
{
    TBBT_NODE *ptr    = root;
    TBBT_NODE *parent = NULL;
    int        cmp    = 1;

    if (ptr) {
        int side;

        while (0 != (cmp = KEYcmp(key, ptr->key, arg))) {
            parent = ptr;
            side   = (cmp < 0) ? LEFT : RIGHT;
            if (!HasChild(ptr, side))
                break;
            ptr = ptr->Link[side];
        }
    }
    if (NULL != pp)
        *pp = parent;
    return (0 == cmp) ? ptr : NULL;
}

/* tbbtffind -- Look up a node in a tree based on a key value */
/* This routine is based on tbbtfind (fix bugs in both places!) */
/* Returns a pointer to the found node (or NULL) */
static TBBT_NODE *
tbbtffind(TBBT_NODE *root, void *key, unsigned fast_compare, TBBT_NODE **pp)
{
    TBBT_NODE *ptr    = root;
    TBBT_NODE *parent = NULL;
    int        cmp    = 1;

    switch (fast_compare) {
        case TBBT_FAST_UINT16_COMPARE:
            if (ptr) {
                int side;

                while (0 != (cmp = (*(uint16 *)key - *(uint16 *)ptr->key))) {
                    parent = ptr;
                    side   = (cmp < 0) ? LEFT : RIGHT;
                    if (!HasChild(ptr, side))
                        break;
                    ptr = ptr->Link[side];
                }
            }
            if (NULL != pp)
                *pp = parent;
            return (0 == cmp) ? ptr : NULL;

        case TBBT_FAST_INT32_COMPARE:
            if (ptr) {
                int side;

                while (0 != (cmp = (*(int32 *)key - *(int32 *)ptr->key))) {
                    parent = ptr;
                    side   = (cmp < 0) ? LEFT : RIGHT;
                    if (!HasChild(ptr, side))
                        break;
                    ptr = ptr->Link[side];
                }
            }
            if (NULL != pp)
                *pp = parent;
            return (0 == cmp) ? ptr : NULL;

        default:
            return NULL;
    } /* end switch */
} /* tbbtffind() */

/* tbbtdfind -- Look up a node in a "described" tree based on a key value */
/* Returns a pointer to the found node (or NULL) */
TBBT_NODE *
tbbtdfind(TBBT_TREE *tree, void *key, TBBT_NODE **pp)
{
    if (tree == NULL)
        return NULL;
    if (tree->Fast_compare != 0)
        return tbbtffind(tree->root, key, tree->Fast_compare, pp);
    else
        return tbbtfind(tree->root, key, tree->Compar, tree->Cmparg, pp);
}

/* tbbtless -- Find a node in a tree which is less than a key, */
/*  based on a key value */
/* Returns a pointer to the found node (or NULL) */
TBBT_NODE *
tbbtless(TBBT_NODE *root, void *key, int (*compar)(void *, void *, int), int arg, TBBT_NODE **pp)
{
    TBBT_NODE *ptr    = root;
    TBBT_NODE *parent = NULL;
    int        cmp    = 1;

    if (ptr) {
        int side;

        while (0 != (cmp = KEYcmp(key, ptr->key, arg))) {
            parent = ptr;
            side   = (cmp < 0) ? LEFT : RIGHT;
            if (!HasChild(ptr, side))
                break;
            ptr = ptr->Link[side];
        }
    }
    if (cmp != 0)
    /* didn't find an exact match, search back up the tree until a node */
    /* is found with a key less than the key searched for */
    {
        while ((ptr = ptr->Parent) != NULL) {
            cmp = KEYcmp(key, ptr->key, arg);
            if (cmp < 0) /* found a node which is less than the search for one */
                break;
        }                /* end while */
        if (ptr == NULL) /* didn't find a node in the tree which was less */
            cmp = 1;
        else /* reset this for cmp test below */
            cmp = 0;
    } /* end if */
    if (NULL != pp)
        *pp = parent;
    return (0 == cmp) ? ptr : NULL;
}

/* tbbtdless -- Find a node less than a key in a "described" tree */
/* Returns a pointer to the found node (or NULL) */
TBBT_NODE *
tbbtdless(TBBT_TREE *tree, void *key, TBBT_NODE **pp)
{
    if (tree == NULL)
        return NULL;
    return tbbtless(tree->root, key, tree->Compar, tree->Cmparg, pp);
}

/* tbbtindx -- Look up the Nth node (in key order) */
/* Returns a pointer to the `indx'th node (or NULL) */
TBBT_NODE *
tbbtindx(TBBT_NODE *root, int32 indx)
{
    TBBT_NODE *ptr = root;

    /* I believe indx is 1 based */
    if (NULL == ptr || indx < 1)
        return NULL;
    /* Termination condition is if the index equals the number of children on
       out left plus the current node */
    while (ptr != NULL && indx != ((int32)LeftCnt(ptr)) + 1) {
        if (indx <= (int32)LeftCnt(ptr)) {
            ptr = ptr->Lchild;
        }
        else if (HasChild(ptr, RIGHT)) { /* subtract children count from leftchild plus current node when
                                            we descend into a right branch */
            indx -= (int32)(LeftCnt(ptr) + 1);
            ptr = ptr->Rchild;
        }
        else
            return NULL; /* Only `indx' or fewer nodes in tree */
    }
    return ptr;
}

/* swapkid -- Often referred to as "rotating" nodes.  ptr and ptr's `side'
 * child, kid, are swapped so ptr becomes kid's `Other(side)' child.
 * Here is how a single swap (rotate) works:
 *
 *           |           --side-->         |
 *         (ptr)                         (kid)
 *        /     \                       /     \
 *    +-A-+    (kid)                 (ptr)    +-C-+
 *    |   |   /     \               /     \   |   |
 *    |...| +-B-+  +-C-+         +-A-+  +-B-+ |...|
 *          |   |  |   |         |   |  |   |
 *          |...|  |...|         |...|  |...|
 * `deep' contains the relative depths of the subtrees so, since we set
 * `deep[1]' (the relative depth of subtree [B]) to 0, `deep[2]' is the depth
 * of [C] minus the depth of [B] (-1, 0, or 1 since `kid' is never too out of
 * balance) and `deep[0]' is the depth of [A] minus the depth of [B].  These
 * values are used to compute the balance levels after the rotation.  Note that
 * [A], [B], or [C] can have depth 0 so `link[]' contains threads rather than
 * pointers to children.
 */
static TBBT_NODE *
swapkid(TBBT_NODE **root, TBBT_NODE *ptr, int side)
{
    TBBT_NODE *kid = ptr->Link[side]; /* Sibling to be swapped with parent */
    int        deep[3];               /* Relative depths of three sub-trees involved. */
    /* 0:ptr->Link[Other(side)], 1:kid->Link[Other(side)], 2:kid->Link[side] */
    TBBT_FLAG ptrflg;       /* New value for ptr->Flags (ptr->Flags used after set) */
    TBBT_LEAF plcnt, prcnt, /* current values of the ptr's and kid's leaf count */
        klcnt, krcnt;

    deep[2]     = (deep[1] = 0) + Delta(kid, side);
    deep[0]     = Max(0, deep[2]) + 1 - Delta(ptr, side);
    kid->Parent = ptr->Parent;
    ptrflg =
        (TBBT_FLAG)SetFlags(ptr, side, deep[0], HasChild(ptr, Other(side)) && HasChild(kid, Other(side)));
    plcnt = LeftCnt(ptr);
    prcnt = RightCnt(ptr);
    klcnt = LeftCnt(kid);
    krcnt = RightCnt(kid);
    if (HasChild(kid, Other(side))) {
        ptr->Link[side]         = kid->Link[Other(side)]; /* Real child */
        ptr->Link[side]->Parent = ptr;
    }
    else {
        ptr->Link[side] = kid; /* Thread */
    }
    /* Update grand parent's pointer: */
    if (NULL == ptr->Parent) {
        *root = kid;
    }
    else if (ptr /*->Lchild*/ == ptr->Parent->Lchild) {
        ptr->Parent->Lchild = kid;
    }
    else {
        ptr->Parent->Rchild = kid;
    }
    ptr->Parent            = kid;
    kid->Link[Other(side)] = ptr;
    kid->Flags = (TBBT_FLAG)SetFlags(kid, Other(side), deep[2] - 1 - Max(deep[0], 0), HasChild(kid, side));

    /* update leaf counts */
    if (side == LEFT) {                /* kid's left count doesn't change, nor ptr's r-count */
        kid->Rcnt = prcnt + krcnt + 1; /* kid's leafs+former parent's leafs+parent */
        ptr->Lcnt = krcnt;
    }                                  /* end if */
    else {                             /* kid's right count doesn't change, nor ptr's l-count */
        kid->Lcnt = plcnt + klcnt + 1; /* kid's leafs+former parent's leafs+parent */
        ptr->Rcnt = klcnt;
    } /* end if */
    ptr->Flags = ptrflg;
    return kid;
}

/* balance -- Move up tree, incrimenting number of left children when needed
 * and looking for unbalanced ancestors.  Adjust all balance factors and re-
 * balance through "rotation"s when needed.
 */
static void
balance(TBBT_NODE **root, TBBT_NODE *ptr, int side, int added)
{
    int deeper = added; /* 1 if sub-tree got longer; -1 if got shorter */
    int odelta;
    int obal;

    while (NULL != ptr) {
        odelta = Delta(ptr, side); /* delta before the node was added */
        obal   = UnBal(ptr);
        if (LEFT == side) /* One more/fewer left child: */
            if (0 < added)
                ptr->Lcnt++; /* LeftCnt(ptr)++ */
            else
                ptr->Lcnt--; /* LeftCnt(ptr)-- */
        else if (0 < added)
            ptr->Rcnt++; /* RightCnt(ptr)++ */
        else
            ptr->Rcnt--;   /* RightCnt(ptr)-- */
        if (0 != deeper) { /* One leg got longer or shorter: */
            if ((deeper < 0 && odelta < 0) || (deeper > 0 && odelta > 0)) { /* Became too unbalanced: */
                TBBT_NODE *kid;

                ptr->Flags |= TBBT_DOUBLE; /* Mark node too unbalanced */
                if (deeper < 0)            /* Just removed a node: */
                    side = Other(side);    /* Swap with child from other side. */
                else
                    /* Just inserted a node: */ if (ptr->Parent && UnBal(ptr->Parent)) {
                        deeper = 0; /* Fix will re-shorten sub-tree. */
                    }
                kid = ptr->Link[side];
                if (Heavy(kid, Other(side))) { /* Double rotate needed: */
                    kid = swapkid(root, kid, Other(side));
                    ptr = swapkid(root, ptr, side);
                }
                else {                       /* Just rotate parent and kid: */
                    if (HasChild(kid, side)) /* In this case, sub-tree gets */
                        if (ptr->Parent && UnBal(ptr->Parent)) {
                            deeper = 0; /* re-lengthened after a node removed. */
                        }
                    ptr = swapkid(root, ptr, side);
                }
            }
            else if (obal) { /* Just became balanced: */
                ptr->Flags &= ~TBBT_UNBAL;
                if (0 < deeper) {              /* Shorter of legs lengthened */
                    ptr->Flags |= TBBT_INTERN; /* Mark as internal node now */
                    deeper = 0;                /* so max length unchanged */
                }                              /* end if */
            }
            else if (deeper < 0) { /* Just became unbalanced: */
                if (ptr->Link[Other(side)] != NULL && ptr->Link[Other(side)]->Parent == ptr) {
                    ptr->Flags |= (TBBT_FLAG)TBBT_HEAVY(Other(side)); /* Other side longer */
                    if (ptr->Parent)
                        if (ptr->Parent->Rchild == ptr) { /* we're the right child */
                            if (Heavy(ptr->Parent, RIGHT) && LeftCnt(ptr->Parent) == 1)
                                deeper = 0;
                            else
                                /* we're the left child */ if (Heavy(ptr->Parent, LEFT))
                                    if (ptr->Parent->Rchild && !UnBal(ptr->Parent->Rchild))
                                        deeper = 0;
                        }
                }
            }
            else {                                         /* Just became unbalanced: */
                ptr->Flags |= (TBBT_FLAG)TBBT_HEAVY(side); /* 0<deeper: Our side longer */
            }                                              /* end else */
        }
        if (ptr->Parent) {
            if (ptr == (ptr->Parent->Rchild))
                side = RIGHT;
            else
                side = LEFT;
        }                  /* end if */
        ptr = ptr->Parent; /* Move up the tree */
    }
    /* total tree depth += deeper; */
}
/* Here is how rotatation rebalances a tree:
 * Either the deletion of a node shortened the sub-tree [A] (to length `h')
 * while [B] or [C] or both are length `h+1'  or  the addition of a node
 * lengthened [B] or [C] to length `h+1' while the other and [A] are both
 * length `h'.  Each case changes `ptr' from being "right heavy" to being
 * overly unbalanced.
 * This           |                      Becomes:      |
 * sub-tree:    (ptr)                                (kid)
 *             /     \          --side-->           /     \
 *         +-A-+    (kid)                        (ptr)   +-C-+
 *         |   |   /     \                      /     \  |   |
 *         | h | +-B-+  +-C-+               +-A-+  +-B-+ | h |
 *         |   | |   |  |   |               |   |  |   | |   |
 *         +---+ | h |  | h |               | h |  | h | +---+
 *         : - : |   |  |   |               |   |  |   | : 1 :
 *         `- -' +---+  +---+               +---+  +---+ + - +
 *               : 1 :  : 1 :                      : 1 :
 *               + - +  + - +                      + - +
 *
 * However, if [B] is long (h+1) while [C] is short (h), a double rotate is
 * required to rebalance.  In this case, [A] was shortened or [X] or [Y] was
 * lengthened so [A] is length `h' and one of [X] and [Y] is length `h' while
 * the other is length `h-1'.  Swap `kid' with `babe' then `ptr' with `babe'.
 * This          |                         Becomes:     |
 * sub-tree:   (ptr)                                  (babe)
 *            /     \             --side-->          /      \
 *       +-A-+       (kid)                      (ptr)       (kid)
 *       |   |      /     \                    /     \     /     \
 *       | h |    (babe)   +-C-+             +-A-+ +-X-+ +-Y-+ +-C-+
 *       |   |   /      \  |   |             |   | |h-1| |h-1| |   |
 *       +---+ +-X-+ +-Y-+ | h |             | h | +---+ +---+ | h |
 *       : - : |h-1| |h-1| |   |             |   | : 1 : : 1 : |   |
 *       `- -' +---+ +---+ +---+             +---+ + - + + - + +---+
 *             : 1 : : 1 :
 *             + - + + - +
 *
 * Note that in the node insertion cases total sub-tree length always increases
 * by one then decreases again so after the rotation(s) no more rebalancing is
 * required.  In the node removal cases, the single rotation reduces total sub-
 * tree length unless [B] is length `h+1' (`ptr' ends of "right heavy") while
 * the double rotation ALWAYS reduces total sub-tree length.  Thus removing a
 * single node can require log(N) rotations for rebalancing.  On average, only
 * are usually required.
 */

/* Returns pointer to inserted node (or NULL) */
TBBT_NODE *
tbbtins(TBBT_NODE **root, void *item, void *key,
        int (*compar)(void * /* k1 */, void * /* k2 */, int /* arg */), int arg)
{
    int        cmp;
    TBBT_NODE *ptr, *parent;

    if (NULL != tbbtfind(*root, (key ? key : item), compar, arg, &parent) || NULL == (ptr = tbbt_get_node()))
        return NULL;
    ptr->data   = item;
    ptr->key    = key ? key : item;
    ptr->Parent = parent;
    ptr->Flags  = 0L; /* No children on either side */
    ptr->Lcnt   = 0;
    ptr->Rcnt   = 0;
    if (NULL == parent) { /* Adding first node to tree: */
        *root       = ptr;
        ptr->Lchild = ptr->Rchild = NULL;
        return ptr;
    }
    cmp = KEYcmp(ptr->key, parent->key, arg);
    if (cmp < 0) {
        ptr->Lchild    = parent->Lchild; /* Parent's thread now new node's */
        ptr->Rchild    = parent;         /* New nodes right thread is parent */
        parent->Lchild = ptr;            /* Parent now has a left child */
    }
    else {
        ptr->Rchild    = parent->Rchild;
        ptr->Lchild    = parent;
        parent->Rchild = ptr;
    }
    balance(root, parent, (cmp < 0) ? LEFT : RIGHT, 1);
    return ptr;
}

/* tbbtdins -- Insert a node into a "described" tree */
/* Returns a pointer to the inserted node */
TBBT_NODE *
tbbtdins(TBBT_TREE *tree, void *item, void *key)
{
    TBBT_NODE *ret_node; /* the node to return */

    if (tree == NULL)
        return NULL;
    ret_node = tbbtins(&(tree->root), item, key, tree->Compar, tree->Cmparg);
    if (ret_node != NULL)
        tree->Count++;
    return ret_node;
}

/* tbbtrem -- Remove a node from a tree.  You pass in the address of the
 * pointer to the root node of the tree along, a pointer to the node you wish
 * to remove, and optionally the address of a pointer to hold the address of
 * the key value of the deleted node.  The second argument is usually the
 * result from a lookup function call (tbbtfind, tbbtdfind, or tbbtindx) so if
 * it is NULL, tbbtrem returns NULL.  Otherwise tbbtrem removes the node from
 * the tree and returns a pointer to the data item for that node and, if the
 * third argument is not NULL, the address of the key value for the deleted
 * node is placed in the buffer that it points to.
 */
/* Data item pointer of deleted node is returned */
void *
tbbtrem(TBBT_NODE **root, TBBT_NODE *node, void **kp)
{
    TBBT_NODE *leaf; /* Node with one or zero children */
    TBBT_NODE *par;  /* Parent of `leaf' */
    TBBT_NODE *next; /* Next/prev node near `leaf' (`leaf's `side' thread) */
    int        side; /* `leaf' is `side' child of `par' */
    void      *data; /* Saved pointer to data item of deleted node */

    if (NULL == root || NULL == node)
        return NULL;   /* Argument couldn't find node to delete */
    data = node->data; /* Save pointer to data item to be returned at end */
    if (NULL != kp)
        *kp = node->key;
    /* If the node to be removed is "internal" (children on both sides), we
     * replace it with it's previous (or next) node in the tree and delete that
     * previous (next) node (which has one or no children) instead. */
    if (Intern(node)) {           /* Replace with a non-internal node: */
        if (Heavy(node, RIGHT)) { /* Pick "near-leaf" node from the */
            side = LEFT;          /* heavier of the sub-trees. */
        }
        else if (Heavy(node, LEFT)) {
            side = RIGHT;
        }
        else { /* If no sub-tree heavier, pick at "random" for "better */
            side = (0x10 & *(short *)&node) ? LEFT : RIGHT; /* balance" */
        }
        leaf = tbbt_nbr(next = node, Other(side));
        if (NULL == leaf)
            return NULL; /* Badness */
        par = leaf->Parent;
        if (par == next) {      /* Case 2x: `node' had exactly 2 descendants */
            side = Other(side); /* Transform this to Case 2 */
            next = leaf->Link[side];
        }
        node->data = leaf->data;
        node->key  = leaf->key;
    }
    else {           /* Node has one or zero children: */
        leaf = node; /* Simply remove THIS node */
        par  = leaf->Parent;
        if (NULL == par) {           /* Case 3: Remove root (of 1- or 2-node tree) */
            side = (int)UnBal(node); /* Which side root has a child on */
            if (side) {              /* Case 3a: Remove root of 2-node tree: */
                *root = leaf = node->Link[side];
                leaf->Parent = leaf->Link[Other(side)] = NULL;
                leaf->Flags                            = 0; /* No left children, balanced, not internal */
            }
            else { /* Case 3b: Remove last node of tree: */
                *root = NULL;
            } /* end else */
            tbbt_release_node(node);
            return data;
        }
        side = (par->Rchild == leaf) ? RIGHT : LEFT;
        next = leaf->Link[side];
    }
    /* Now the deletion has been reduced to the following cases (and Case 3 has
     * been handled completely above and Case 2x has been transformed into
     * Case 2).  `leaf' is a node with one or zero children that we are going
     * to remove.  `next' points where the `side' thread of `leaf' points.
     * `par' is the parent of `leaf'.  The only possibilities (not counting
     * left/right reversals) are shown below:
     *       [Case 1]                  [Case 2]              [Case 2x]
     *            (next)                 (next)         ^         (next & par)
     *           /  ^   \               /  ^   \        |        /  ^         \
     *     . . .    |             . . .    |            |  (leaf)   /
     *   /          |           /          |            \_/      \_/
     * (par)        |         (par)        |             ^threads^
     *      \       |              \       |
     *     (leaf)   /             (leaf)   /            [Case 3a]    [Case 3b]
     *    /  ^   \_/<thread             \_/<thread       (root)
     * (n)   /                                                 \       (root)
     *    \_/<thread        --"side"-->                         (n)
     * Note that in Cases 1 and 2, `leaf's `side' thread can be NULL making
     * `next' NULL as well.  If you remove a node from a 2-node tree, removing
     * the root falls into Case 3a while removing the only leaf falls into
     * Case 2 (with `next' NULL and `par' the root node). */
    if (!UnBal(leaf)) { /* Case 2: `leaf' has no children: */
        par->Link[side] = leaf->Link[side];
        par->Flags &= (TBBT_FLAG)(~(TBBT_INTERN | TBBT_HEAVY(side)));
    }
    else { /* Case 1: `leaf' has one child: */
        TBBT_NODE *n;

        if (HasChild(leaf, side)) { /* two-in-a-row cases */
            n               = leaf->Link[side];
            par->Link[side] = n;
            n->Parent       = par;
            if (HasChild(n, Other(side)))
                while (HasChild(n, Other(side)))
                    n = n->Link[Other(side)];
            n->Link[Other(side)] = par;
        }      /* end if */
        else { /* zig-zag cases */
            n               = leaf->Link[Other(side)];
            par->Link[side] = n;
            n->Parent       = par;
            if (HasChild(n, side))
                while (HasChild(n, side))
                    n = n->Link[side];
            n->Link[side] = next;
        } /* end else */
    }
    tbbt_release_node(leaf);
    balance(root, par, side, -1);
    ((TBBT_TREE *)root)->Count--;
    return data;
}

/* tbbtdmake - Allocate a new tree description record for an empty tree */
/* Returns a pointer to the description record */
TBBT_TREE *
tbbtdmake(int (*cmp)(void * /* k1 */, void * /* k2 */, int /* arg */), int arg, unsigned fast_compare)
{
    TBBT_TREE *tree = NULL;

    if (NULL == (tree = (TBBT_TREE *)calloc(1, sizeof(TBBT_TREE))))
        goto error;
    if (NULL == (tree->priv = (TBBT_TREE_PRIV *)calloc(1, sizeof(TBBT_TREE_PRIV))))
        goto error;

    tree->root         = NULL;
    tree->Count        = 0;
    tree->Fast_compare = fast_compare;
    tree->Compar       = cmp;
    tree->Cmparg       = arg;

    return tree;
error:
    if (tree)
        free(tree->priv);
    free(tree);
    return NULL;
}

/* tbbtfree() - Free an entire tree not allocated with tbbtdmake(). */
void
tbbtfree(TBBT_NODE **root, void (*fd)(void * /* item */), void (*fk)(void * /* key */))
{
    TBBT_NODE *par, *node = *root;

    while (NULL != *root) { /* While nodes left to be free()d */
        /* First time at this node (just moved down a new leg of tree) */
        if (!HasChild(node, LEFT))
            node->Lchild = NULL;
        if (!HasChild(node, RIGHT))
            node->Rchild = NULL;
        do {
            par = NULL; /* Assume we aren't ready to move up tree yet */
            if (NULL != node->Lchild)
                node = node->Lchild; /* Move down this leg next */
            else if (NULL != node->Rchild)
                node = node->Rchild; /* Move down this leg next */
            else {                   /* No children; free node an move up: */
                par = node->Parent;  /* Move up tree (stay in loop) */
                if (NULL != fd)
                    (*fd)(node->data);
                if (NULL != fk)
                    (*fk)(node->key);
                if (NULL == par)  /* Just free()d last node */
                    *root = NULL; /* NULL=par & NULL=*root gets fully out */
                else if (node == par->Lchild)
                    par->Lchild = NULL; /* Now no longer has this child */
                else
                    par->Rchild = NULL; /* Ditto */

                tbbt_release_node(node);

                node = par; /* Move up tree; remember which node to do next */
            }
        } while (NULL != par); /* While moving back up tree */
    }
}

void
tbbtprint(TBBT_NODE *node)
{
    if (node == NULL)
        return;
    printf("node=%p, key=%p, data=%p, flags=%x\n", (void *)node, node->key, node->data,
           (unsigned)node->Flags);
    printf("Lcnt=%d, Rcnt=%d\n", (int)node->Lcnt, (int)node->Rcnt);
    printf("*key=%d\n", (int)*(int32 *)(node->key));
    printf("Lchild=%p, Rchild=%p, Parent=%p\n", (void *)node->Lchild, (void *)node->Rchild,
           (void *)node->Parent);
} /* end tbbtprint() */

void
tbbt1dump(TBBT_NODE *node, int method)
{
    if (node == NULL)
        return;
    switch (method) {
        case -1: /* Pre-Order Traversal */
            tbbtprint(node);
            if (HasChild(node, LEFT))
                tbbt1dump(node->Lchild, method);
            if (HasChild(node, RIGHT))
                tbbt1dump(node->Rchild, method);
            break;

        case 1: /* Post-Order Traversal */
            if (HasChild(node, LEFT))
                tbbt1dump(node->Lchild, method);
            if (HasChild(node, RIGHT))
                tbbt1dump(node->Rchild, method);
            tbbtprint(node);
            break;

        case 0: /* In-Order Traversal */
        default:
            if (HasChild(node, LEFT))
                tbbt1dump(node->Lchild, method);
            tbbtprint(node);
            if (HasChild(node, RIGHT))
                tbbt1dump(node->Rchild, method);
            break;

    } /* end switch() */
} /* end tbbt1dump() */

void
tbbtdump(TBBT_TREE *tree, int method)
{
    if (tree != NULL && *(TBBT_NODE **)tree != NULL) {
        printf("Number of nodes in the tree: %ld\n", tree->Count);
        tbbt1dump(tree->root, method);
    } /* end if */
    else
        printf("Tree is empty\n");
} /* end tbbtdump() */

void
tbbt_printNode(TBBT_NODE *node, void (*key_dump)(void *, void *))
{

    if (node == NULL) {
        printf("ERROR:  null node pointer\n");
        return;
    }
    printf("node=%p, flags=%x, Lcnt=%ld, Rcnt=%ld\n", (void *)node, (unsigned)node->Flags, (long)node->Lcnt,
           (long)node->Rcnt);
    printf("Lchild=%p, Rchild=%p, Parent=%p\n", (void *)node->Lchild, (void *)node->Rchild,
           (void *)node->Parent);
    if (key_dump != NULL) {
        (*key_dump)(node->key, node->data);
    }
    fflush(stdout);
} /* end tbbt_printNode() */

void
tbbt_dumpNode(TBBT_NODE *node, void (*key_dump)(void *, void *), int method)
{
    if (node == NULL)
        return;
    switch (method) {
        case -1: /* Pre-Order Traversal */
            tbbt_printNode(node, key_dump);
            if (HasChild(node, LEFT))
                tbbt_dumpNode(node->Lchild, key_dump, method);
            if (HasChild(node, RIGHT))
                tbbt_dumpNode(node->Rchild, key_dump, method);
            break;

        case 1: /* Post-Order Traversal */
            if (HasChild(node, LEFT))
                tbbt_dumpNode(node->Lchild, key_dump, method);
            if (HasChild(node, RIGHT))
                tbbt_dumpNode(node->Rchild, key_dump, method);
            tbbt_printNode(node, key_dump);
            break;

        case 0: /* In-Order Traversal */
        default:
            if (HasChild(node, LEFT))
                tbbt_dumpNode(node->Lchild, key_dump, method);
            tbbt_printNode(node, key_dump);
            if (HasChild(node, RIGHT))
                tbbt_dumpNode(node->Rchild, key_dump, method);
            break;

    } /* end switch() */
} /* end tbbt_dumpNode() */

void
tbbt_dump(TBBT_TREE *ptree, void (*key_dump)(void *, void *), int method)
{
    TBBT_TREE *tree;

    tree = (TBBT_TREE *)ptree;
    printf("TBBT-tree dump  %p:\n\n", (void *)ptree);
    printf("capacity = %ld\n", (long)tree->Count);
    printf("\n");
    tbbt_dumpNode(tree->root, key_dump, method);
}

/* Always returns NULL */
TBBT_TREE *
tbbtdfree(TBBT_TREE *tree, void (*fd)(void * /* item */), void (*fk)(void * /* key */))
{
    if (tree == NULL)
        return NULL;

    tbbtfree(&tree->root, fd, fk);
    free(tree->priv);
    free(tree);
    return NULL;
}

/* returns the number of nodes in the tree */
long
tbbtcount(TBBT_TREE *tree)
{
    if (tree == NULL)
        return -1;
    else
        return (long)tree->Count;
}

/******************************************************************************
 NAME
     tbbt_get_node - Gets a tbbt node

 DESCRIPTION
    Either gets a tbbt node from the free list (if there is one available)
    or allocates a node.

 RETURNS
    Returns tbbt ptr if successful and NULL otherwise

*******************************************************************************/
static TBBT_NODE *
tbbt_get_node(void)
{
    TBBT_NODE *ret_value = NULL;

    if (tbbt_free_list != NULL) {
        ret_value      = tbbt_free_list;
        tbbt_free_list = tbbt_free_list->Lchild;
    }
    else {
        if (NULL == (ret_value = (TBBT_NODE *)calloc(1, sizeof(TBBT_NODE))))
            goto error;
        if (NULL == (ret_value->priv = (TBBT_NODE_PRIV *)calloc(1, sizeof(TBBT_NODE_PRIV))))
            goto error;
    }

    return ret_value;
error:
    if (ret_value)
        free(ret_value->priv);
    free(ret_value);
    return NULL;
} /* end tbbt_get_node() */

/******************************************************************************
 NAME
     tbbt_release_node - Releases a tbbt node

 DESCRIPTION
    Puts a tbbt node into the free list

 RETURNS
    No return value

*******************************************************************************/
static void
tbbt_release_node(TBBT_NODE *nod)
{
    /* Insert the atom at the beginning of the free list */
    nod->Lchild    = tbbt_free_list;
    tbbt_free_list = nod;
} /* end tbbt_release_node() */

/*--------------------------------------------------------------------------
 NAME
    tbbt_shutdown
 PURPOSE
    Terminate various static buffers.
 USAGE
    int tbbt_shutdown()
 RETURNS
    Returns SUCCEED/FAIL
 DESCRIPTION
    Free various buffers allocated in the tbbt routines.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    Should only ever be called by the "atexit" function HDFend
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int
tbbt_shutdown(void)
{
    TBBT_NODE *curr;

    /* Release the free-list if it exists */
    if (tbbt_free_list != NULL) {
        while (tbbt_free_list != NULL) {
            curr           = tbbt_free_list;
            tbbt_free_list = tbbt_free_list->Lchild;
            free(curr->priv);
            free(curr);
        }
    }
    return SUCCEED;
} /* end tbbt_shutdown() */
