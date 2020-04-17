/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */
/* Description: Double Linked List Helpers */

#ifndef LLIST_H
#define LLIST_H

/* Initialize a list to an empty state, or an item to an unlinked state. */
#define LINK_LIST_INIT(_p_) \
    do {  \
        (_p_)->next = (_p_); \
        (_p_)->prev = (_p_); \
    } while (false)

/* Cast a list item to its parent type. */
/*lint --emacro((413),LINK_LIST_ITEM) */
/*lint --emacro((511),LINK_LIST_ITEM) */
/*lint --emacro((613),LINK_LIST_ITEM) */
/*lint --emacro((923),LINK_LIST_ITEM) */
/*lint --emacro((9033),LINK_LIST_ITEM) */
/* Warning 413: Likely use of null pointer 'unknown-name' in left argument to operator '->' [MISRA 2012 Rule 1.3, required] */
/* Warning 511: Size incompatibility in cast from 'unsigned long' (8 bytes) to 'struct ZbMsgT *' (4 bytes) */
/* Warning 613: Possible use of null pointer 'zb' in left argument to operator '->' */
/* Note 923: cast from pointer to unsigned long [MISRA 2012 Rule 11.6, required] */
/* Note 9033: Impermissible cast of composite expression (different essential type categories) [MISRA 2012 Rule 10.8, required] */
#define LINK_LIST_ITEM(_p_, _type_, _member_)   ((_type_ *)((unsigned long)(_p_) - (unsigned long)(&((_type_ *)0)->_member_)))

/* Returns the next/previous list item or NULL if at the end of the list. */
#define LINK_LIST_NEXT(_p_, _list_)     (((_p_)->next != (_list_)) ? (_p_)->next : NULL)
#define LINK_LIST_PREV(_p_, _list_)     (((_p_)->prev != (_list_)) ? (_p_)->prev : NULL)

/* Checks if an item is linked into the list. */
#define LINK_LIST_ISLINKED(_p_)         (((_p_)->next != (_p_)) && ((_p_)->prev != (_p_)))

/* Unlinks an item from a list, has no effect if already unlinked. */
#define LINK_LIST_UNLINK(_p_) \
    do { \
        (_p_)->next->prev = (_p_)->prev; \
        (_p_)->prev->next = (_p_)->next; \
        (_p_)->next = (_p_); \
        (_p_)->prev = (_p_); \
    } while (false)

/* Returns the head/tail of a list, or NULL if empty. */
#define LINK_LIST_HEAD(_list_)          LINK_LIST_NEXT(_list_, _list_)
#define LINK_LIST_TAIL(_list_)          LINK_LIST_PREV(_list_, _list_)

#define LINK_LIST_INSERT_BEFORE(_p_, _add_) \
    do { \
        (_add_)->next = (_p_); \
        (_add_)->prev = (_p_)->prev; \
        (_add_)->next->prev = (_add_); \
        (_add_)->prev->next = (_add_); \
    } while (false)

#define LINK_LIST_INSERT_AFTER(_p_, _add_) \
    do { \
        (_add_)->prev = (_p_); \
        (_add_)->next = (_p_)->next; \
        (_add_)->prev->next = (_add_); \
        (_add_)->next->prev = (_add_); \
    } while (false)

#define LINK_LIST_INSERT_HEAD(_list_, _add_) LINK_LIST_INSERT_AFTER(_list_, _add_)
#define LINK_LIST_INSERT_TAIL(_list_, _add_) LINK_LIST_INSERT_BEFORE(_list_, _add_)

/* Adds an item to a list while keeping the list sorted. */
#define LINK_LIST_INSERT_SORT(_list_, _add_, _cmp_) \
    do { \
        struct LinkListT *_i_ = (_list_)->next; \
        while (true) { \
            if (_i_ == (_list_)) { \
                break; \
            } \
            if (_cmp_(_add_, _i_) < 0) { \
                break; \
            } \
            _i_ = _i_->next; \
        } \
        LINK_LIST_INSERT_BEFORE(_i_, _add_); \
    } while (false)

/* Checks if a list is empty. */
#define LINK_LIST_ISEMPTY(_list_)       (!LINK_LIST_ISLINKED(_list_))

/* List iteration macro. */
#define LINK_LIST_FOREACH(_p_, _list_) \
    for ((_p_) = LINK_LIST_HEAD(_list_); (_p_) != NULL; (_p_) = LINK_LIST_NEXT(_p_, _list_))

/* Linked list type */
struct LinkListT {
    struct LinkListT *next;
    struct LinkListT *prev;
};

#endif /* LLIST_H */
