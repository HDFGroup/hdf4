/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                      *
 *                                                                          *
 ****************************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/* "tbbt.c" -- Routines for using threaded, balanced, binary trees. */
/* Extended from (added threads to) Knuth 6.2.3, Algorithm A (AVL trees) */
/* Basic tree structure by Adel'son-Vel'skii and Landis */

#include <stdio.h>  /* NULL */
#include "hdf.h"
#define TBBT_INTERNALS
#include "tbbt.h"
#define   Alloc(cnt,typ)   (typ *) HDgetspace( (cnt) * sizeof(typ) )
#define   Free(x)           (HDfreespace((VOIDP)x))

# define   KEYcmp(k1,k2,a)   (  (NULL!=compar) ? (*compar)( k1, k2, a)         \
                             : HDmemcmp( k1, k2, 0<(a) ? (a) : HDstrlen(k1) )  )

VOID tbbt1dump
	PROTO((TBBT_NODE *node,intn method));

/* #define TESTING */

/* Returns pointer to end-most (to LEFT or RIGHT) node of tree: */
#ifdef PROTOTYPE
static TBBT_NODE *tbbt_end(TBBT_NODE *root, intn side)
#else
static TBBT_NODE *tbbt_end( root, side )
TBBT_NODE *root;
intn side;
#endif
{
    if(  NULL == root  )
        return( root );
    while(  HasChild( root, side )  ) {
        root= root->link[side];
    }
    return( root );
}

#ifdef PROTOTYPE
TBBT_NODE *tbbtfirst(TBBT_NODE *root)
#else
TBBT_NODE *tbbtfirst( root )
  TBBT_NODE *root;
#endif
{
    return(  tbbt_end( root, LEFT )  );
}

#ifdef PROTOTYPE
TBBT_NODE *tbbtlast(TBBT_NODE *root)
#else
TBBT_NODE *tbbtlast( root )
  TBBT_NODE *root;
#endif
{
    return(  tbbt_end( root, RIGHT )  );
}

/* Return address of parent's pointer to neighboring node (to LEFT or RIGHT) **
#ifdef PROTOTYPE
static TBBT_NODE **tbbt_anbr(TBBT_NODE *ptr, intn side )
#else
static TBBT_NODE **tbbt_anbr( ptr, side )
TBBT_NODE *ptr;
intn       side;
#endif
{
  TBBT_NODE **aptr;

    if(  ! HasChild( ptr, side )  )
        return(  & ptr->link[side]  );
    aptr= & ptr->link[side];
    while(  HasChild( *aptr, Other(side) )  )
        aptr= & (*aptr)->link[Other(side)];
    return( aptr );
}*/

/* Returns pointer to neighboring node (to LEFT or RIGHT): */
#ifdef PROTOTYPE
static TBBT_NODE *tbbt_nbr(TBBT_NODE * ptr, intn  side )
#else
static TBBT_NODE *tbbt_nbr( ptr, side )
TBBT_NODE *ptr;
intn       side;
#endif
{
    /* return( *tbbt_anbr(ptr,side) ); */

    if(  ! HasChild( ptr, side )  )
        return(  ptr->link[side]  ); 
/*        return(NULL); */
    ptr= ptr->link[side];
    while(  HasChild( ptr, Other(side) )  )
        ptr= ptr->link[Other(side)];
    return( ptr );
}

/* Returns pointer to node with previous key value: */
#ifdef PROTOTYPE
TBBT_NODE *tbbtnext( TBBT_NODE *node )
#else
TBBT_NODE *tbbtnext( node )
TBBT_NODE *node;
#endif
{
    return(  tbbt_nbr( node, RIGHT )  );
}

/* Returns pointer to node with next key value: */
#ifdef PROTOTYPE
TBBT_NODE *tbbtprev( TBBT_NODE *node)
#else
TBBT_NODE *tbbtprev( node )
TBBT_NODE *node;
#endif
{
    return(  tbbt_nbr( node, LEFT )  );
}

/* tbbtfind -- Look up a node in a tree based on a key value */
/* Returns a pointer to the found node (or NULL) */
#ifdef PROTOTYPE
TBBT_NODE *tbbtfind(TBBT_NODE *root,VOIDP key, intn (*compar) PROTO((VOIDP,VOIDP,intn)),intn arg,
        TBBT_NODE    **pp )
#else
TBBT_NODE *tbbtfind( root, key, compar, arg, pp )
  TBBT_NODE     *root;  /* Pointer to root node of tree (NULL if 0 elements) */
  VOIDP         key;    /* Key value of node we are looking for */
  intn         (*compar)(/* void *k1, void *k2, int arg */);
                        /* memcmp()-like routine to compare key values */
  intn           arg;   /* Third argument for (*compar)() */
  TBBT_NODE    **pp;    /* Place for pointer to parent (or NULL) */
#endif
{
    TBBT_NODE *ptr= root;
    TBBT_NODE *parent= NULL;
    intn       cmp= 1;

    if(ptr) {
        intn side;

        while(  0 != ( cmp= KEYcmp(key,ptr->key,arg) )  ) {
            parent= ptr;
            side= (cmp<0) ? LEFT : RIGHT;
            if(  ! HasChild( ptr, side )  )
                break;
            ptr= ptr->link[side];
        }
    }
    if(  NULL != pp  )
        *pp= parent;
    return( (0==cmp) ? ptr : NULL );
}

/* tbbtdfind -- Look up a node in a "described" tree based on a key value */
/* Returns a pointer to the found node (or NULL) */
#ifdef PROTOTYPE
TBBT_NODE *tbbtdfind(TBBT_TREE *tree,VOIDP key,TBBT_NODE **pp)
#else
TBBT_NODE *tbbtdfind( tree, key, pp )
TBBT_TREE     *tree;  /* Pointer to tree description record */
VOIDP          key;   /* Pointer to key value to find node with */
TBBT_NODE    **pp;    /* Address of buffer for pointer to parent node */
#endif
{
    return(  tbbtfind( tree->root, key, tree->compar, tree->cmparg, pp )  );
}

/* tbbtindx -- Look up the Nth node (in key order) */
/* Returns a pointer to the `indx'th node (or NULL) */
#ifdef PROTOTYPE
TBBT_NODE *tbbtindx(TBBT_NODE *root,int32 indx)
#else
TBBT_NODE *tbbtindx( root, indx )
TBBT_NODE *root;  /* Pointer to root node of tree (NULL if 0 elements) */
int32      indx;  /* Sequence number of desired node (0 for first) */
#endif
{
    TBBT_NODE *ptr= root;

    if(  NULL == ptr  ||  indx < 0  )
        return( NULL );
    while(  indx != (int32)LeftCnt(ptr)  ) {
        if(  indx < (int32)LeftCnt(ptr)  ) {
            indx -= LeftCnt(ptr);
            ptr= ptr->Lchild;
        } else if(  HasChild(ptr,RIGHT)  )
            ptr= ptr->Rchild;
        else
            return( NULL ); /* Only `indx' or fewer nodes in tree */
    }
    return( ptr );
}

/* swapkid -- Often refered to as "rotating" nodes.  ptr and ptr's `side'
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
#ifdef PROTOTYPE
static TBBT_NODE *swapkid(TBBT_NODE **root,TBBT_NODE *ptr,intn side )
#else
static TBBT_NODE *swapkid( root, ptr, side )
  TBBT_NODE    **root;  /* Pointer to root of tree */
  TBBT_NODE     *ptr;   /* Pointer to parent node to be swapped */
  intn           side;  /* Which of parent's children is to be swapped */
#endif
{
  TBBT_NODE *kid= ptr->link[side];  /* Sibling to be swapped with parent */
  intn  deep[3];        /* Relative depths of three sub-trees involved. */
  /* 0:ptr->link[Other(side)], 1:kid->link[Other(side)], 2:kid->link[side] */
  TBBT_FLAG ptrflg;     /* New value for ptr->flags (ptr->flags used after set) */
  TBBT_LEAF plcnt,prcnt,  /* current values of the ptr's and kid's leaf count */
      klcnt,krcnt;

#ifdef TESTING
printf("swapkid(): check 1, ptr=%p, ptr->flags=%x, side=%d, kid=%p, kid->flags=%x\n",ptr,ptr->flags,side,kid,kid->flags);
#endif
    deep[2]= ( deep[1]= 0 ) + Delta(kid,side);
    deep[0]= Max(0,deep[2]) + 1 - Delta(ptr,side);
#ifdef TESTING
printf("swapkid(): check 1.5, deep=%d,%d,%d\n",deep[0],deep[1],deep[2]);
#endif
    kid->Parent= ptr->Parent;
#ifdef QAK
    ptrflg= SetFlags( ptr, side, Cnt(ptr,side) - Cnt(kid,side) - 1,
            /*deep[1]-deep[0] */ deep[0]-deep[1], 
	    HasChild(ptr,Other(side)) && HasChild(kid,Other(side)) );
#else
    ptrflg= SetFlags( ptr, side, deep[0],
	    HasChild(ptr,Other(side)) && HasChild(kid,Other(side)) );
    plcnt= LeftCnt(ptr);
    prcnt= RightCnt(ptr);
    klcnt= LeftCnt(kid);
    krcnt= RightCnt(kid);
#endif
    if(  HasChild(kid,Other(side))  ) {
#ifdef TESTING
printf("swapkid(): check 2, ptr=%p, side=%d, kid=%p\n",ptr,side,kid);
#endif
        ptr->link[side]= kid->link[Other(side)];    /* Real child */
        ptr->link[side]->Parent= ptr;
    } else {
#ifdef TESTING
printf("swapkid(): check 3, ptr=%p, side=%d, kid=%p\n",ptr,side,kid);
#endif
        ptr->link[side]= kid;                       /* Thread */
	}
    /* Update grand parent's pointer: */
    if(  NULL == ptr->Parent  ) {
        *root= kid;
	}
    else if(  ptr/*->Lchild*/ == ptr->Parent->Lchild  ) {
        ptr->Parent->Lchild= kid;
	}
    else {
        ptr->Parent->Rchild= kid;
	}
    ptr->Parent= kid;
    kid->link[Other(side)]= ptr;
#ifdef QAK
    kid->flags= SetFlags( kid, Other(side),
            Cnt(ptr,Other(side)) + 1 + Cnt(kid,Other(side)),
            deep[2]-1-Max(deep[0],deep[1]), HasChild(kid,side) );
#else
    kid->flags= SetFlags( kid, Other(side),
            deep[2]-1-Max(deep[0],0), HasChild(kid,side) );

    /* update leaf counts */
    if(side==LEFT) { /* kid's left count doesn't change, nor ptr's r-count */
        kid->rcnt=prcnt+krcnt+1; /* kid's leafs+former parent's leafs+parent */
	ptr->lcnt=krcnt;
      } /* end if */
    else {  /* kid's right count doesn't change, nor ptr's l-count */
        kid->lcnt=plcnt+klcnt+1; /* kid's leafs+former parent's leafs+parent */
	ptr->rcnt=klcnt;
      } /* end if */
#endif
    ptr->flags= ptrflg;
#ifdef TESTING
printf("swapkid(): check 4, ptr=%p, ptr->flags=%x, ptr->Parent=%p, kid->flags=%x\n",ptr,ptr->flags,ptr->Parent,kid->flags);
#endif
    return( kid );
}

/* balance -- Move up tree, incrimenting number of left children when needed
 * and looking for unbalanced ancestors.  Adjust all balance factors and re-
 * balance through "rotation"s when needed.
 */
#ifdef PROTOTYPE
static VOID balance(TBBT_NODE **root,TBBT_NODE *ptr,intn side,intn added )
#else
static VOID balance( root, ptr, side, added )
  TBBT_NODE    **root;  /* Root of tree being balanced */
  TBBT_NODE     *ptr;   /* Parent that just gained/lost a child */
  intn           side;  /* Which side child was inserted on or removed from */
  intn           added; /* 1 if a child was just added; -1 if just deleted */
#endif
{
  intn deeper= added;  /* 1 if sub-tree got longer; -1 if got shorter */
  intn odelta;
  intn obal;

#ifdef TESTING
printf("balance(): side=%d, added=%d\n",side,added);
#endif
    while(  NULL != ptr  ) {
	odelta=Delta(ptr,side);     /* delta before the node was added */
	obal=UnBal(ptr);
#ifdef TESTING
printf("balance(): check 0, deeper=%d, ptr=%p, side=%d, odelta=%d, obal=%d\n",deeper,ptr,side,odelta,obal);
#endif
        if(  LEFT == side  )        /* One more/fewer left child: */
            if(  0 < added  )
                ptr->lcnt ++;       /* LeftCnt(ptr)++ */
            else
                ptr->lcnt --;       /* LeftCnt(ptr)-- */
	else
            if(  0 < added  )
                ptr->rcnt ++;       /* RightCnt(ptr)++ */
            else
                ptr->rcnt --;       /* RightCnt(ptr)-- */
        if(  0 != deeper  ) {   /* One leg got longer or shorter: */
            if( deeper == odelta ) { /* Became too unbalanced: */
              TBBT_NODE *kid;

#ifdef TESTING
printf("balance(): check 1, deeper=%d, ptr=%p\n",deeper,ptr);
#endif
                ptr->flags |= TBBT_DOUBLE;  /* Mark node too unbalanced */
                if(  deeper < 0  )      /* Just removed a node: */
                    side= Other(side);  /* Swap with child from other side. */
                else                    /* Just inserted a node: */
		    if(ptr->Parent && UnBal(ptr->Parent)) {
#ifdef TESTING
printf("balance(): check 1.2, ptr=%p\n",ptr);
#endif
                        deeper= 0;          /* Fix will re-shorten sub-tree. */
		      }
                kid= ptr->link[side];
                if(  Heavy(kid,Other(side))  ) {    /* Double rotate needed: */
#ifdef TESTING
printf("balance(): check 1.3, ptr=%p\n",ptr);
#endif
                    kid= swapkid( root, kid, Other(side) );
                    ptr= swapkid( root, ptr, side );
                } else {                    /* Just rotate parent and kid: */
#ifdef TESTING
printf("balance(): check 1.6, ptr=%p, root=%p, side=%d, ptr->flags=%x, ptr->Parent=%p\n",ptr,root,side,ptr->flags,ptr->Parent);
#endif
                    if(  HasChild(kid,side)  ) /* In this case, sub-tree gets */
			if(ptr->Parent && UnBal(ptr->Parent)) {
#ifdef TESTING
printf("balance(): check 1.7, ptr->Parent=%p, ptr->Parent->flags=%x\n",ptr->Parent,ptr->Parent->flags);
#endif
                            deeper= 0;  /* re-lengthened after a node removed. */
			  }
                        ptr= swapkid( root, ptr, side );
                }
#ifdef TESTING
printf("balance(): check 1.8, ptr=%p, ptr->Parent=%p\n",ptr,ptr->Parent);
#endif
            } else if( obal ) {     /* Just became balanced: */
#ifdef TESTING
printf("balance(): check 2, ptr=%p\n",ptr);
#endif
                ptr->flags &= ~TBBT_UNBAL;
                if(  0 < deeper  ) {  /* Shorter of legs lengthened */
#ifdef TESTING
printf("balance(): check 2.5\n");
#endif
		    ptr->flags|=TBBT_INTERN;  /* Mark as internal node now */
                    deeper= 0;      /* so max length unchanged */
		  } /* end if */
            } else if(  deeper < 0  ) {     /* Just became unbalanced: */
#ifdef TESTING
printf("balance(): check 3, ptr=%p, deeper=%d\n",ptr,deeper);
#endif
                if(ptr->link[Other(side)]!=NULL && ptr->link[Other(side)]->Parent==ptr) {
#ifdef TESTING
printf("balance(): check 3.2, ptr=%p, deeper=%d\n",ptr,deeper);
#endif
                    ptr->flags|=TBBT_HEAVY(Other(side)); /* Other side longer */
		    if(ptr->Parent)
			if(ptr->Parent->Rchild==ptr) /* we're the right child */
			   if(Heavy(ptr->Parent,RIGHT) && LeftCnt(ptr->Parent)==1)
			       deeper=0;
			else /* we're the left child */
			   if(Heavy(ptr->Parent,LEFT))
			       if(ptr->Parent->Rchild && !UnBal(ptr->Parent->Rchild))
				   deeper=0;
		  }
            } else {                        /* Just became unbalanced: */
#ifdef TESTING
printf("balance(): check 4, side=%d, lcnt=%d, rcnt=%d\n",side,LeftCnt(ptr),RightCnt(ptr));
#endif
                ptr->flags |= TBBT_HEAVY(side); /* 0<deeper: Our side longer */
              } /* end else */
        }
        if(ptr->Parent) {
            if(ptr == (ptr->Parent->Rchild))
                side=RIGHT;
            else
                side=LEFT;
          } /* end if */
        ptr= ptr->Parent;   /* Move up the tree */
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
#ifdef PROTOTYPE
TBBT_NODE *tbbtins(TBBT_NODE **root,VOIDP item,VOIDP key,intn (*compar)
        PROTO((VOIDP k1,VOIDP k2,intn arg)),intn arg )
#else
TBBT_NODE *tbbtins( root, item, key, compar, arg )
TBBT_NODE    **root;  /* Address of pointer to root node of tree */
VOIDP         item;  /* Data to be associated with node */
VOIDP         key;   /* Key of node to be added */
intn         (*compar)(/* void *k1, void *k2, int arg */);
                        /* memcmp()-like routine to compare key values */
intn           arg;   /* Third argument for (*compar)() */
#endif
{
    intn cmp;
    TBBT_NODE *ptr, *parent;

    if(NULL != tbbtfind( *root, (key ? key : item), compar, arg, &parent )
            || NULL == ( ptr= Alloc(1,TBBT_NODE) ) )
        return( NULL );
    ptr->data= item;
    ptr->key= key ? key : item;
    ptr->Parent= parent;
    ptr->flags= 0L;     /* No children on either side */
    ptr->lcnt=0;
    ptr->rcnt=0;
    if(  NULL == parent  ) {    /* Adding first node to tree: */
        *root= ptr;
        ptr->Lchild= ptr->Rchild= NULL;
        return( ptr );
    }
    cmp= KEYcmp( ptr->key, parent->key, arg);
    if(  cmp < 0  ) {
        ptr->Lchild= parent->Lchild;    /* Parent's thread now new node's */
        ptr->Rchild= parent;            /* New nodes right thread is parent */
        parent->Lchild= ptr;            /* Parent now has a left child */
    } else {
        ptr->Rchild= parent->Rchild;
        ptr->Lchild= parent;
        parent->Rchild= ptr;
    }
    balance( root, parent, (cmp<0) ? LEFT : RIGHT, 1 );
    return( ptr );
}

/* tbbtdins -- Insert a node into a "described" tree */
             /* Returns a pointer to the inserted node */
#ifdef PROTOTYPE
TBBT_NODE *tbbtdins(TBBT_TREE *tree,VOIDP item,VOIDP key )
#else
TBBT_NODE *tbbtdins( tree, item, key )
TBBT_TREE *tree;      /* Pointer to tree description record */
VOIDP     item;      /* Pointer to data item for new node */
VOIDP     key;       /* Pointer to key value for new node (or NULL) */
#endif
{
    TBBT_NODE *ret_node; /* the node to return */

    ret_node=tbbtins( &(tree->root), item, key, tree->compar, tree->cmparg );
    if(ret_node!=NULL)
	tree->count++;
    return(ret_node);
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
#ifdef PROTOTYPE
VOIDP tbbtrem(TBBT_NODE **root,TBBT_NODE *node,VOIDP *kp)
#else
VOIDP tbbtrem( root, node, kp )
  TBBT_NODE **root; /* Root of tree to delete node from */
  TBBT_NODE  *node; /* Pointer to node to be deleted */
  VOIDP      *kp;   /* NULL or address of buffer for pointer to key value */
#endif
{
  TBBT_NODE *leaf;  /* Node with one or zero children */
  TBBT_NODE *par;   /* Parent of `leaf' */
  TBBT_NODE *next;  /* Next/prev node near `leaf' (`leaf's `side' thread) */
  intn  side;       /* `leaf' is `side' child of `par' */
  VOIDP data;       /* Saved pointer to data item of deleted node */

    if(  NULL == node  )
        return( NULL ); /* Argument couldn't find node to delete */
    data= node->data;   /* Save pointer to data item to be returned at end */
    if(  NULL != kp  )
        *kp= node->key;
    /* If the node to be removed is "internal" (children on both sides), we
     * replace it with it's previous (or next) node in the tree and delete that
     * previous (next) node (which has one or no children) instead. */
    if(  Intern(node)  ) {              /* Replace with a non-internal node: */
#ifdef TESTING
printf("tbbtrem(): check 0, node->flags=%x\n",node->flags);
#endif
        if(  Heavy(node,RIGHT)  ) {     /* Pick "near-leaf" node from the */
#ifdef TESTING
printf("tbbtrem(): check 0.1, right side heavier\n");
#endif
            side= LEFT;                 /* heavier of the sub-trees. */
#ifdef QAK
	    node->flags &= ~(TBBT_HEAVY(RIGHT));
#endif
	  }
        else if(  Heavy(node,LEFT)  ) {
#ifdef TESTING
printf("tbbtrem(): check 0.2, left side heavier\n");
#endif
            side= RIGHT;
#ifdef QAK
	    node->flags &= ~(TBBT_HEAVY(LEFT));
#endif
	  }
        else {      /* If no sub-tree heavier, pick at "random" for "better */
#ifdef TESTING
printf("tbbtrem(): check 0.3, NO side heavier\n");
#endif
            side= (0x10 & *(short *)&node) ? LEFT : RIGHT;      /* balance" */
#ifdef QAK
	    node->flags |= TBBT_HEAVY(side);
	    node->flags &= ~(TBBT_INTERN);
#endif
	  }
#ifdef TESTING
printf("tbbtrem(): check 0.4, side=%d\n",side);
#endif
        leaf= tbbt_nbr( next= node, Other(side) );
#ifdef TESTING
printf("tbbtrem(): check 0.5, leaf=%p\n",leaf);
#endif
        par= leaf->Parent;
#ifdef TESTING
printf("tbbtrem(): check 0.6, par=%p, next=%p\n",par,next);
#endif
        if(  par == next  ) {   /* Case 2x: `node' had exactly 2 descendants */
            side= Other(side);  /* Transform this to Case 2 */
            next= leaf->link[side];
        }
#ifdef TESTING
printf("tbbtrem(): check 0.7, side=%d, par=%p, next=%p\n",side,par,next);
#endif
        node->data= leaf->data;
        node->key= leaf->key;
    } else {                /* Node has one or zero children: */
#ifdef TESTING
printf("tbbtrem(): check 1\n");
#endif
        leaf= node;         /* Simply remove THIS node */
        par= leaf->Parent;
        if(  NULL == par  ) { /* Case 3: Remove root (of 1- or 2-node tree) */
            side= (intn)UnBal(node);  /* Which side root has a child on */
#ifdef TESTING
printf("tbbtrem(): check 2, side=%d\n",side);
#endif
            if(  side  ) {  /* Case 3a: Remove root of 2-node tree: */
                *root= leaf= node->link[side];
#ifdef TESTING
printf("tbbtrem(): check 2.5 leaf=%p\n",leaf);
#endif
                leaf->Parent= leaf->link[Other(side)]= NULL;
                leaf->flags= 0; /* No left children, balanced, not internal */
            } else {        /* Case 3b: Remove last node of tree: */
#ifdef TESTING
printf("tbbtrem(): check 3\n");
#endif
                *root= NULL;
	      } /* end else */
            Free( node );
#ifdef TESTING
printf("tbbtrem(): check 4\n");
#endif
            return( data );
        }
        side=  ( par->Rchild == leaf ) ? RIGHT : LEFT;
#ifdef TESTING
printf("tbbtrem(): check 4.5 side=%d\n",side);
#endif
        next= leaf->link[side];
    }
    /* Now the deletion has been reduced to the following cases (and Case 3 has
     * been handled completely above and Case 2x has been transformed into
     * Case 2).  `leaf' is a node with one or zero children that we are going
     * to remove.  `next' points where the `side' thread of `leaf' points.
     * `par' is the parent of `leaf'.  The only posibilities (not counting
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
    if(  ! UnBal(leaf)  ) {         /* Case 2: `leaf' has no children: */
#ifdef TESTING
printf("tbbtrem(): check 5, par->flags=%x par->Parent=%p\n",(unsigned)par->flags,par->Parent);
#endif
        par->link[side]= leaf->link[side];
        par->flags &= ~( TBBT_INTERN | TBBT_HEAVY(side) );
#ifdef TESTING
printf("tbbtrem(): check 5.5, par->flags=%x, par->Parent=%p\n",(unsigned)par->flags,par->Parent);
#endif
    } else {                        /* Case 1: `leaf' has one child: */
#ifdef OLD_WAY
        TBBT_NODE *n= leaf->link[Other(side)];
#ifdef TESTING
printf("tbbtrem(): check 6\n");
#endif
        par->link[side]= n;
        n->Parent= par;
        n->link[side]= next;
#else
        TBBT_NODE *n;

#ifdef TESTING
printf("tbbtrem(): check 6, side=%d, leaf=%p, par=%p, next=%p\n",side,leaf,par,next);
#endif
        if(HasChild(leaf,side)) {  /* two-in-a-row cases */
            n=leaf->link[side];
#ifdef TESTING
printf("tbbtrem(): check 6.3, n=%p\n",n);
#endif
            par->link[side]=n;
            n->Parent=par;
	    if(HasChild(n,Other(side))) 
		while(HasChild(n,Other(side)))
		    n=n->link[Other(side)];
            n->link[Other(side)]=par;
	  } /* end if */
	else {  /* zig-zag cases */
            n=leaf->link[Other(side)];
#ifdef TESTING
printf("tbbtrem(): check 6.6, n=%p\n",n);
#endif
            par->link[side]=n;
            n->Parent=par;
	    if(HasChild(n,side)) 
		while(HasChild(n,side))
		    n=n->link[side];
            n->link[side]=next;
	  } /* end else */
#endif
    }
    Free( leaf );
#ifdef TESTING
printf("tbbtrem(): check 7, par->flags=%x\n",(unsigned)par->flags);
#endif
    balance( root, par, side, -1 );
#ifdef TESTING
printf("tbbtrem(): check 7.5, par=%p, par->lcnt=%d, par->rcnt=%d\n",par,par->lcnt,par->rcnt);
#endif
	((TBBT_TREE *)root)->count--;
    return( data );
}

/* tbbtdmake - Allocate a new tree description record for an empty tree */
/* Returns a pointer to the description record */
#ifdef PROTOTYPE
TBBT_TREE *tbbtdmake(intn (*cmp) PROTO((VOIDP k1,VOIDP k2,intn arg)), intn arg)
#else
TBBT_TREE *tbbtdmake( cmp, arg )
intn (*cmp)(/* void *k1, void *k2, int arg */);
                /* memcmp()-link routine for comparing keys (or NULL) */
intn   arg;   /* Third argument for (*cmp)() */
#endif
{
  TBBT_TREE *tree= Alloc( 1, TBBT_TREE );

    if(  NULL == tree  )
        return( NULL );
    tree->root= NULL;
    tree->count=0;
    tree->compar= cmp;
    tree->cmparg= arg;
    return( tree );
}

#ifdef WASTE_STACK
/* You can have a very simple recursive version that wastes lots of stack
 * space, this next less-simple recursive version that wastes less stack space,
 * or the last non-recursive version which isn't simple but saves stack space.
 */
static VOID (*FD) PROTO((VOIDP item)), (*FK) PROTO((VOIDP key));
#ifdef PROTOTYPE
static VOID tbbt1free( TBBT_NODE *node)
#else
static VOID tbbt1free( node )
TBBT_NODE *node;
#endif
{
    if(  HasChild(node,LEFT)  )     tbbt1free( node->Lchild );
    if(  HasChild(node,RIGHT)  )    tbbt1free( node->Rchild );
    if(  NULL != FD  )              (*FD)( node->data );
    if(  NULL != FK  )              (*FK)( node->key );
    Free( node );
}

#ifdef PROTOTYPE
VOID tbbtfree(TBBT_NODE **root, VOID (*fd) PROTO((VOIDP item)),VOID (*fk) PROTO((VOIDP key)))
#else
VOID tbbtfree( root, fd, fk )
TBBT_NODE    **root;
VOID         (*fd)(/* void *item */);
VOID         (*fk)(/* void *key */);
#endif
{
    if(  NULL == *root  )
        return;
    FD= fd;   FK= fk;
    tbbt1free( *root );
    *root= NULL;
}
#else /* WASTE_STACK */

/* tbbtfree() - Free an entire tree not allocated with tbbtdmake(). */
#ifdef PROTOTYPE
VOID tbbtfree(TBBT_NODE **root, VOID (*fd) PROTO((VOIDP item)),VOID (*fk) PROTO((VOIDP key)))
#else
VOID tbbtfree( root, fd, fk )
TBBT_NODE    **root;                  /* Address of pointer to root node */
VOID         (*fd)(/* void *item */); /* Routine to free data items */
VOID         (*fk)(/* void *key */);  /* Routine to free key values */
#endif
{
  TBBT_NODE *par, *node= *root;

    while(  NULL != *root  ) {  /* While nodes left to be free()d */
        /* First time at this node (just moved down a new leg of tree) */
        if(  ! HasChild(node,LEFT)  )    node->Lchild= NULL;
        if(  ! HasChild(node,RIGHT)  )   node->Rchild= NULL;
        do {
            par= NULL;  /* Assume we aren't ready to move up tree yet */
            if(  NULL != node->Lchild  )
                node= node->Lchild; /* Move down this leg next */
            else if(  NULL != node->Rchild  )
                node= node->Rchild; /* Move down this leg next */
            else {                  /* No children; free node an move up: */
                par= node->Parent;  /* Move up tree (stay in loop) */
                if(  NULL != fd  )   (*fd)( node->data );
                if(  NULL != fk  )   (*fk)( node->key );
                if(  NULL == par  )     /* Just free()d last node */
                    *root= NULL;    /* NULL=par & NULL=*root gets fully out */
                else if(  node == par->Lchild  )
                    par->Lchild= NULL;  /* Now no longer has this child */
                else
                    par->Rchild= NULL;  /* Ditto */

                Free( node );

                node = par;  /* Move up tree; remember which node to do next */
            }
        } while(  NULL != par  );   /* While moving back up tree */
    }
}
#endif /* WASTE_STACK */

#ifdef PROTOTYPE
VOID tbbtprint(TBBT_NODE *node)
#else
VOID tbbtprint(node)
TBBT_NODE *node;
#endif
{
	printf("node=%p, key=%p, data=%p, flags=%x\n",node,node->key,node->data,node->flags);
	printf("Lcnt=%d, Rcnt=%d\n",node->lcnt,node->rcnt);
printf("*key=%d\n",*(int32 *)(node->key));
	printf("Lchild=%p, Rchild=%p, Parent=%p\n",node->Lchild,node->Rchild,node->Parent);
}	/* end tbbtprint() */

#ifdef PROTOTYPE
VOID tbbt1dump(TBBT_NODE *node,intn method)
#else
VOID tbbt1dump(node,method)
TBBT_NODE *node;
intn method;
#endif
{
	if(node==NULL)
		return;
	switch(method) {
		case -1:	/* Pre-Order Traversal */
			tbbtprint(node);
			if(HasChild(node,LEFT))
				tbbt1dump(node->Lchild,method);
			if(HasChild(node,RIGHT))
				tbbt1dump(node->Rchild,method);
			break;

		case 1:		/* Post-Order Traversal */
			if(HasChild(node,LEFT))
				tbbt1dump(node->Lchild,method);
			if(HasChild(node,RIGHT))
				tbbt1dump(node->Rchild,method);
			tbbtprint(node);
			break;

		case 0:		/* In-Order Traversal */
		default:
			if(HasChild(node,LEFT))
				tbbt1dump(node->Lchild,method);
			tbbtprint(node);
			if(HasChild(node,RIGHT))
				tbbt1dump(node->Rchild,method);
			break;

	  }	/* end switch() */
}	/* end tbbt1dump() */

#ifdef PROTOTYPE
VOID tbbtdump(TBBT_TREE *tree,intn method)
#else
VOID tbbtdump(tree, method)
TBBT_TREE *tree;
intn method;
#endif
{
    if(*(TBBT_NODE **)tree!=NULL) {
        printf("Number of nodes in the tree: %ld\n",tree->count);
	tbbt1dump(tree->root,method);
      } /* end if */
    else
	printf("Tree is empty\n");
}	/* end tbbtdump() */

/* Always returns NULL */
#ifdef PROTOTYPE
TBBT_TREE *tbbtdfree(TBBT_TREE *tree,VOID (*fd) PROTO((VOIDP item)), VOID (*fk) PROTO((VOIDP key)))
#else
TBBT_TREE *tbbtdfree( tree, fd, fk )
TBBT_TREE *tree;                  /* Pointer to tree description record */
VOID     (*fd)(/* void *item */); /* Routine to free data items */
VOID     (*fk)(/* void *key */);  /* Routine to free key values */
#endif
{
    tbbtfree( &tree->root, fd, fk );
    Free( tree );
    return( NULL );
}


/* returns the number of nodes in the tree */
#ifdef PROTOTYPE
long tbbtcount(TBBT_TREE *tree)
#else
long tbbtcount(tree)
TBBT_TREE *tree;                  /* Pointer to tree description record */
#endif
{
    if(tree==NULL)
        return(-1);
    else
	return(tree->count);
}
