/**
 * @author      redxef
 * @file        datstruc.h
 * @version     0.0.0-r0
 * @since
 *
 * @brief       A brief documentation about the file.
 *
 * A detailed documentation.
 */

#ifndef __DATSTRUC_H_
#define __DATSTRUC_H_

/* I N C L U D E S                                                            */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* P R A G M A S                                                              */
#define LL_DATA_TYPE_NONE               0
#define LL_DATA_TYPE_VOID               1
#define LL_DATA_TYPE_I8                 2
#define LL_DATA_TYPE_I16                3
#define LL_DATA_TYPE_I32                4
#define LL_DATA_TYPE_I64                5
#define LL_DATA_TYPE_U8                 6
#define LL_DATA_TYPE_U16                7
#define LL_DATA_TYPE_U32                8
#define LL_DATA_TYPE_U64                9


#define HM_MODE_STRING                  1
#define HM_MODE_BINARY                  2

#define LL_DATA_HM_ENTRY                100

/* S T R U C T S                                                              */

/* basic generic data type */
struct ds_data {
        union {
                int64_t  _int;
                uint64_t _uint;
                double   _double;
                void    *_ptr;
        };
};

/* node of the linked list */
struct ds_node {
        struct ds_node *prev;
        struct ds_node *next;
        struct ds_data data;
};

/**
 * Doubly linked list implemented to behave as the java implementation.
 */
struct ds_linked_list {
        struct ds_node *first;
        struct ds_node *last;
        struct ds_node *flow;
        uint64_t type;
        size_t length;
};

/* entry of the hash map */
struct ds_hm_entry {
        void *key;
        struct ds_data value;
};

/**
 * HashMap with Strings as keys
 */
struct ds_hash_map {
        struct ds_hm_entry none;
        struct ds_linked_list *buckets;
        uint64_t (*hash)(struct ds_hash_map *hm, const void *key);
        uint64_t size;
        uint64_t entry_size;    /* number of max bytes per entry */
};

/* V A R I A B L E S                                                          */


/* F U N C T I O N   P R O T O T Y P E S                                      */

/**
 * Creates a new linked list.
 * If dest is NULL, then a new linked list will be allocated on the heap,
 * otherwise the specified memory location will be used, assuming, that there is
 * only uninitialized data.
 *
 * @param       dest    the destination of the linked list
 * @returns             the pointer to the linked list struct
 */
struct ds_linked_list *ll__new(struct ds_linked_list *dest, uint64_t type);

/**
 * Returns the length of the linked list.
 *
 * @param       list    the list from which to get the length
 * @returns             the length of this list
 */
size_t ll__length(struct ds_linked_list *list);

/**
 * Clears the linked list of all items.
 *
 * @param       list    the list to be cleared
 * @param       free    should the items be free'd
 */
void ll__clear(struct ds_linked_list *list);

/**
 * Deletes the whole linked list and sets the pointer to it to NULL.
 * If using a normal struct to hold the linked list (instead of a pointer),
 * the list must first be cleared, and then manually deallocated:
 *  1. clear the list
 *  2. free the list.first element:
 *  2.1 (optional) free the data pointer
 *  2.2 free the list.first element
 *  3. profit
 *
 * @param       list    the list to be deallocated
 * @param       dealloc should the items be free'd
 */
void ll__delete(struct ds_linked_list **list);

/**
 * Appends the specified data to the end of the list.
 *
 * @param       list    the list to use
 * @param       data    the data to be appended
 * @param       type    the type of data
 */
void ll__append(struct ds_linked_list *list, struct ds_data data);

/**
 * Inserts the specified data into the list at the current position.
 * When iterating over the list with ll__next and ll__prev, an internal pointer
 * iterates over the list, when performing operation such as ll__insert or
 * ll__remove those operations happen at this position.
 * The item specified is inserted immediatly after the current item.
 *
 * @param       list    the list to operate on
 * @param       data    the data to be inserted
 * @param       type    the type of the data
 */
void ll__insert(struct ds_linked_list *list, struct ds_data data);

/**
 * Inserts the specified data into the list at the specified position.
 *
 * @param       list    the list to operate on
 * @param       data    the data to insert
 * @param       type    the type of the data
 * @param       at      the position at wich to insert (index)
 */
void ll__insert_at(struct ds_linked_list *list, struct ds_data data, size_t at);

/**
 * Returns the first element of this list.
 *
 * @param       list    the list to operate on
 * @param       data    the struct to be filled with the appropriate data.
 */
void ll__get_first(struct ds_linked_list *list, struct ds_data *data);

/**
 * Returns the last element of this list.
 *
 * @param       list    the list to operate on
 * @param       data    the struct to be filled with the appropriate data.
 */
void ll__get_last(struct ds_linked_list *list, struct ds_data *data);

/**
 * Removes the currently selected list node and deletes it.
 *
 * @param       list    the list to operate on
 * @param       free    should the data be free'd
 */
void ll__remove(struct ds_linked_list *list);

/**
 * Removes the list node at the specified position.
 *
 * @param       list    the list to operate on
 * @param       at      the index of the node to be removed
 * @param       free    should the data be free'd
 */
void ll__remove_at(struct ds_linked_list *list, size_t at);

/**
 * Checks if there is another item in the list.
 *
 * @param       list    the list to operate on
 * @returns             wheter there is another item in the list or not
 */
uint8_t ll__has_next(struct ds_linked_list *list);

/**
 * Checks if there is another item in the list.
 *
 * @param       list    the list to operate on
 * @returns             wheter there is another item in the list or not
 */
uint8_t ll__has_prev(struct ds_linked_list *list);

/**
 * Returns the next data from the list.
 *
 * @param       list    the list to operate on
 * @param       data    the destination pointer of the data
 * @param       type    the destination pointer of the type
 */
void ll__next(struct ds_linked_list *list, struct ds_data *data);

/**
 * Returns the previous data from the list.
 *
 * @param       list    the list to operate on
 * @param       data    the destination pointer of the data
 * @param       type    the destination pointer of the type
 */
void ll__prev(struct ds_linked_list *list, struct ds_data *data);

/**
 * The ll__to_X_array family of functions turn the list into an
 * array.
 *
 * @param       list    the list to convert
 * @param       arr     a pointer to a preallocated array
 */
#define ll__to_N_array_header(name, type)                               \
void ll__to_##name##_array(struct ds_linked_list *list, type *arr)

#define ll__to_N_array(name, type, use_union_field)                     \
void ll__to_##name##_array(struct ds_linked_list *list, type *arr) {    \
        size_t i = 0;                                                   \
        struct ds_data dat;                                             \
        struct ds_node *old_flow = list->flow;                          \
        list->flow = list->first;                                       \
        while (ll__has_next(list)) {                                    \
                ll__next(list, &dat);                                   \
                arr[i++] = (type) dat.use_union_field;                  \
        }                                                               \
        list->flow = old_flow;                                          \
}

ll__to_N_array_header(i8, int8_t);
ll__to_N_array_header(i16, int16_t);
ll__to_N_array_header(i32, int32_t);
ll__to_N_array_header(i64, int64_t);
ll__to_N_array_header(u8, uint8_t);
ll__to_N_array_header(u16, uint16_t);
ll__to_N_array_header(u32, uint32_t);
ll__to_N_array_header(u64, uint64_t);
ll__to_N_array_header(void, void *);

/**
 * The ll__from_X_array family of functions fill a list with the
 * values specified by the given array.
 *
 * @param       list    the list to be filled
 * @param       arr     the array from which the values should be taken
 * @param       len     the length of the array
 */
#define ll__from_N_array_header(name, type)                             \
void ll__from_##name##_array(struct ds_linked_list *list,               \
        type *arr, size_t len)

#define ll__from_N_array(name, type_, use_union_field, data_type_macro) \
void ll__from_##name##_array(struct ds_linked_list *list, type_ *arr,   \
        size_t len) {                                                   \
        size_t i;                                                       \
        struct ds_data dat;                                             \
        list->type = data_type_macro;                                   \
        for (i = 0; i < len; i++) {                                     \
                dat.use_union_field = arr[i];                           \
                ll__append(list, dat);                                  \
        }                                                               \
}

ll__from_N_array_header(i8, int8_t);
ll__from_N_array_header(i16, int16_t);
ll__from_N_array_header(i32, int32_t);
ll__from_N_array_header(i64, int64_t);
ll__from_N_array_header(u8, uint8_t);
ll__from_N_array_header(u16, uint16_t);
ll__from_N_array_header(u32, uint32_t);
ll__from_N_array_header(u64, uint64_t);
ll__from_N_array_header(void, void *);

/**
 * Prints out the list, assuming that the data is a character.
 *
 * @param       list    the list to be printed
 */
void ll__print(struct ds_linked_list *list);

/**
 * Prints the list into str, assuming that the data is a character.
 *
 * @param       str     the destination buffer
 * @param       list    the list to be printed
 */
void ll__sprint(char *str, struct ds_linked_list *list);

/**
 * Creates a new hash map with size buckets. Collisions are resolved with a
 * linked list. When the string mode is selected, then
 * keys can be supplied as strings without much further though.
 * If binary mode is selected, then the key must always be a entry_size
 * long byte array containing the data. A entry size of <= 0 implys string
 * mode.
 *
 * @param       dest            the destination struct
 * @param       size            the target bucket count
 * @param       entry_size      the number of bytes per entry
 * @returns             the pointer to the resulting hash_map struct
 */
struct ds_hash_map *hm__new(struct ds_hash_map *dest, uint64_t size, uint64_t entry_size);

/**
 * The default hash function of the map for strings.
 * This function computes the hash until it encounters a zero, this is
 * for zero terminated strings.
 *
 * @param       key     the key to be hashed
 * @returns             the hash value
 */
uint64_t hm__default_hash_string(struct ds_hash_map *hm, const void *key);

/**
 * The default hash function of the map for binary data.
 * This function computes the hash over the while key array, it checks
 * every item of the allocated array in hm_entry.
 *
 * @param       key     the key to be hashed
 * @returns             the hash value
 */
uint64_t hm__default_hash_binary(struct ds_hash_map *hm, const void *key);

/**
 * Saves some data in the hash map using a key. The hm_entry struct contains
 * both key and value.
 *
 * @param       hm      the hash map
 * @param       key     the key under which to save the value
 * @param       value   the value to be saved (can be any of: int, uint, double, void *)
 */
#define hm__put(hm, key, val)   hm__put_(hm, key, (struct ds_data) {{val}})
void hm__put_(struct ds_hash_map *hm, const void *key, struct ds_data value);

/**
 * Retrieves the entry from the hash map based on the key. If no entry exists,
 * then an empty entry is returned with the data set to none
 * (type = LL_DATA_TYPE_NONE).
 *
 * @param       hm      the hash map
 * @param       key     the key for the value
 * @returns             the entry in the hash map
 */
struct ds_hm_entry hm__get(struct ds_hash_map *hm, const void *key);

/**
 * Deletes an entry from the hash map. The user needs to manually free data, if
 * required.
 *
 * @param       hm      the hash map
 * @param       key     the key of the entry to delete
 */
void hm__delete(struct ds_hash_map *hm, const void *key);
#endif
