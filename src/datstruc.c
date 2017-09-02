/**
 * @author      redxef
 * @file        datstruc.c
 * @version     0.0.0-r0
 * @since       
 * 
 * @brief       A brief documentation about the file.
 *
 * A detailed documentation.
 */

#include "datstruc.h"

/* V A R I A B L E S                                                          */


/* C O D E                                                                    */
struct ll_node *lln__new(void) {
        struct ll_node *res = malloc(sizeof(struct ll_node));
        if (res == NULL) {
                errno = ENOMEM;
                return NULL;
        }
        res->next = NULL;
        res->prev = NULL;
        memset(&res->data, 0, sizeof(struct data));
        return res;
}

struct data lln__remove_obj(struct ll_node *rm) {
        struct data ret = rm->data;
        if (rm->prev != NULL) {
                rm->prev->next = rm->next;
        }
        if (rm->next != NULL) {
                rm->next->prev = rm->prev;
        }
        free(rm);
        return ret;
}

void lln__delete(struct ll_node *first) {
        struct ll_node *fre;
        while (first != NULL) {
                fre = first;
                first = first->next;
                free(fre);
        }
}

struct ll_linked_list *ll__new(struct ll_linked_list *dest, uint64_t type) {
        if (dest == NULL) {
                dest = malloc(sizeof(struct ll_linked_list));
                if (dest == NULL) {
                        errno = ENOMEM;
                        return NULL;
                }
        }

        dest->first = lln__new();
        if (dest->first == NULL) {
                errno = ENOMEM;
                free(dest);
                return NULL;
        }

        dest->last = dest->first;
        dest->flow = dest->first;
        dest->length = 0;

        dest->type = type;

        return dest;
}

void ll__clear(struct ll_linked_list *list) {
        lln__delete(list->first);
        list->first = lln__new();
        if (list->first == NULL) {
                list->first = NULL;
                list->last = NULL;
                list->flow = NULL;
                list->length = 0;
                return;
        }
        list->last = list->first;
        list->flow = list->first;
        list->length = 0;
}

void ll__delete(struct ll_linked_list **list) {
        lln__delete((*list)->first);
        free(*list);
        *list = NULL;
}

void ll__rewind(struct ll_linked_list *list) {
        list->flow = list->first;
}

void ll__append(struct ll_linked_list *list, struct data data) {
        list->last->data = data;
        list->last->next = lln__new();
        list->last->next->prev = list->last;
        list->last = list->last->next;
        list->length += 1;
}

void ll__insert(struct ll_linked_list *list, struct data data) {
        struct ll_node *nd = lln__new();
        if (nd == NULL) {
                errno = ENOMEM;
                return;
        }
        nd->prev = list->flow;
        nd->next = list->flow->next;
        list->flow->next = nd;
        list->flow->next->next->prev = nd;
        nd->data = data;
        list->length += 1;
}

void ll__insert_at(struct ll_linked_list *list, struct data data, size_t at) {
        size_t i;
        struct ll_node *old_flow = list->flow;
        list->flow = list->first;

        if (at >= list->length) return;
        for (i = 1; i < at; i++)
                list->flow = list->flow->next;

        ll__insert(list, data);
        list->flow = old_flow;
}

void ll__remove(struct ll_linked_list *list) {
        struct ll_node *new_flow = list->flow->next;
        /* we don't need to handle the last item, since it will never be accessed by the user */
        uint8_t first = 0;
        if (list->flow == list->first)
                first = 1;
        lln__remove_obj(list->flow);
        if (first)
                list->first = new_flow;
        list->flow = new_flow;
        list->length -= 1;
}

void ll__remove_at(struct ll_linked_list *list, size_t at) {
        size_t i;
        struct ll_node *old_flow = list->flow;
        list->flow = list->first;

        if (at >= list->length) return;
        for (i = 0; i < at; i++)
                list->flow = list->flow->next;

        ll__remove(list);
        list->flow = old_flow;
}

uint8_t ll__has_next(struct ll_linked_list *list) {
        return (list->flow->next != NULL);
}

uint8_t ll__has_prev(struct ll_linked_list *list) {
        return (list->flow->prev != NULL);
}

void ll__next(struct ll_linked_list *list, struct data *data) {
        if (!ll__has_next(list)) {
                *data = (struct data) {{0}};
                return;
        }
        if (data != NULL)
                *data = list->flow->data;
        list->flow = list->flow->next;
}

void ll__prev(struct ll_linked_list *list, struct data *data) {
        if (!ll__has_prev(list)) {
                *data = (struct data) {{0}};
                return;
        }
        if (data != NULL)
                *data = list->flow->prev->data;
        list->flow = list->flow->prev;
}

#define ll__to_N_array(name, type, use_union_field)                     \
void ll__to_##name##_array(struct ll_linked_list *list, type *arr) {    \
        size_t i = 0;                                                   \
        struct data dat;                                                \
        struct ll_node *old_flow = list->flow;                          \
        list->flow = list->first;                                       \
        while (ll__has_next(list)) {                                    \
                ll__next(list, &dat);                                   \
                arr[i++] = (type) dat.use_union_field;                  \
        }                                                               \
        list->flow = old_flow;                                          \
}

ll__to_N_array(i8, int8_t, _int)
ll__to_N_array(i16, int16_t, _int)
ll__to_N_array(i32, int32_t, _int)
ll__to_N_array(i64, int64_t, _int)
ll__to_N_array(u8, uint8_t, _uint)
ll__to_N_array(u16, uint16_t, _uint)
ll__to_N_array(u32, uint32_t, _uint)
ll__to_N_array(u64, uint64_t, _uint)
ll__to_N_array(void, void *, _ptr)

#define ll__from_N_array(name, type_, use_union_field, data_type_macro)                 \
void ll__from_##name##_array(struct ll_linked_list *list, type_ *arr, size_t len) {     \
        size_t i;                                                                       \
        struct data dat;                                                                \
        list->type = data_type_macro;                                                   \
        for (i = 0; i < len; i++) {                                                     \
                dat.use_union_field = arr[i];                                           \
                ll__append(list, dat);                                                  \
        }                                                                               \
}

ll__from_N_array(i8, int8_t, _int, LL_DATA_TYPE_I8)
ll__from_N_array(i16, int16_t, _int, LL_DATA_TYPE_I16)
ll__from_N_array(i32, int32_t, _int, LL_DATA_TYPE_I32)
ll__from_N_array(i64, int64_t, _int, LL_DATA_TYPE_I64)
ll__from_N_array(u8, uint8_t, _uint, LL_DATA_TYPE_U8)
ll__from_N_array(u16, uint16_t, _uint, LL_DATA_TYPE_U16)
ll__from_N_array(u32, uint32_t, _uint, LL_DATA_TYPE_U32)
ll__from_N_array(u64, uint64_t, _uint, LL_DATA_TYPE_U64)
ll__from_N_array(void, void *, _ptr, LL_DATA_TYPE_VOID)

void ll__print(struct ll_linked_list *list) {
        struct ll_node *flow = list->first;

        printf("{");
        while (flow->next != NULL) {
                printf("%llu, ", flow->data._uint);
                flow = flow->next;
        }
        printf("\b\b}");
}

void ll__sprint(char *str, struct ll_linked_list *list) {
        struct ll_node *flow = list->first;
        
        char *end = str;

        sprintf(end, "{");
        end = strchr(end, '\0');
        while (flow->next != NULL) {
                sprintf(end, "%llu, ", flow->data._uint);
                end = strchr(end, '\0');
                flow = flow->next;
        }
        end[-2] = '}';
        end[-1] = '\0';
        end = strchr(end, '\0');
}

struct hm_hash_map *hm__new(struct hm_hash_map *dest, uint64_t size) {
        uint64_t i;
        if (dest == NULL) {
                dest = malloc(sizeof(struct hm_hash_map));
                if (dest == NULL) {
                        errno = ENOMEM;
                        return NULL;
                }
        }
        dest->buckets = malloc(size * sizeof(struct ll_linked_list));

        for (i = 0; i < size; i++) {
                ll__new(&dest->buckets[i], LL_DATA_HM_ENTRY);
        }
        dest->hash = NULL;
        dest->size = size;
        return dest;
}

uint64_t hm__default_hash(const char *key) {
        uint64_t hash = 0x42;
        uint8_t i = 0;

        while (*key) {
                hash ^= *key;
                hash *= 7;
                key++;
                i++;
        }
        return hash;
}

void hm__put(struct hm_hash_map *hm, struct hm_entry entry) {
        struct hm_entry *ecpy;
        struct data dat;
        uint64_t hash;

        ecpy = malloc(sizeof(struct hm_entry));
        if (ecpy == NULL) {
                errno = ENOMEM;
                return;
        }

        memcpy(ecpy, &entry, sizeof(struct hm_entry));
        dat._ptr = ecpy;

        hash = hm->hash(entry.key) % hm->size;

        ll__append(&(hm->buckets[hash]), dat);
}

struct hm_entry hm__get(struct hm_hash_map *hm, const char *key) {
        uint64_t hash = hm->hash(key) % hm->size;
        struct data dat;
        struct hm_entry *entry;
        struct ll_linked_list ll = hm->buckets[hash];
        ll.flow = ll.first;

        while (ll__has_next(&ll)) {
                ll__next(&ll, &dat);
                entry = dat._ptr;
                if (strcmp(key, entry->key) == 0) {
                        return *entry;
                }
        }
        return (struct hm_entry) {"", {{0}}};
}

void hm__delete(struct hm_hash_map *hm, const char *key) {
        uint64_t hash;
        struct data dat;
        struct hm_entry *entry;
        struct ll_linked_list ll;

        hash = hm->hash(key) % hm->size;
        ll = hm->buckets[hash];
        ll.flow = ll.first;

        while (ll__has_next(&ll)) {
                ll__next(&ll, &dat);
                entry = dat._ptr;
                if (strcmp(key, entry->key) == 0) {
                        ll__prev(&ll, &dat);
                        ll__prev(&ll, &dat);
                        free(entry);
                        ll__remove(&ll);
                        hm->buckets[hash] = ll;
                        return;
                }
        }
}
