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
struct ds_node *lln__new(void) {
        struct ds_node *res = malloc(sizeof(struct ds_node));
        if (res == NULL) {
                errno = ENOMEM;
                return NULL;
        }
        res->next = NULL;
        res->prev = NULL;
        memset(&res->data, 0, sizeof(struct ds_data));
        return res;
}

struct ds_data lln__remove_obj(struct ds_node *rm, int destroy_data) {
        struct ds_data ret = rm->data;
        if (rm->prev != NULL) {
                rm->prev->next = rm->next;
        }
        if (rm->next != NULL) {
                rm->next->prev = rm->prev;
        }
        if (destroy_data)
                free(rm->data._ptr);
        free(rm);
        return ret;
}

void lln__delete(struct ds_node *first, int destroy_data) {
        struct ds_node *fre;
        while (first != NULL) {
                fre = first;
                first = first->next;
                if (destroy_data)
                        free(fre->data._ptr);
                free(fre);
        }
}

void ll__new(struct ds_linked_list *dest, uint64_t type) {
        dest->first = lln__new();
        if (dest->first == NULL) {
                errno = ENOMEM;
                return;
        }

        dest->last = dest->first;
        dest->flow = dest->first;
        dest->type = type;
        dest->length = 0;
}

int ll__should_delete_data(struct ds_linked_list *list) {
         return list->type == LL_DATA_TYPE_VOID_AUTO_DELETE;
}

size_t ll__length(struct ds_linked_list *list) {
        return list->length;
}

void ll__clear(struct ds_linked_list *list) {
        lln__delete(list->first, ll__should_delete_data(list));
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

void ll__delete(struct ds_linked_list *list) {
        lln__delete(list->first, ll__should_delete_data(list));
}

void ll__rewind(struct ds_linked_list *list) {
        list->flow = list->first;
}

void ll__append(struct ds_linked_list *list, struct ds_data data) {
        struct ds_node *nd = lln__new();
        if (nd == NULL) {
                errno = ENOMEM;
                return;
        }
        list->last->data = data;
        list->last->next = nd;
        list->last->next->prev = list->last;
        list->last = list->last->next;
        list->length += 1;
}

void ll__insert_front(struct ds_linked_list *list, struct ds_data data) {
        struct ds_node *nd = lln__new();
        struct ds_node *flow;
        if (nd == NULL) {
                errno = ENOMEM;
                return;
        }
        nd->data = data;

        flow = list->flow;
        if (flow->prev == NULL)
                list->first = nd;
        list->flow = nd;
        nd->prev = flow->prev;
        nd->next = flow;
        nd->prev->next = nd;
        nd->next->prev = nd;
        flow->prev = nd;
        list->length += 1;
}

void ll__insert_back(struct ds_linked_list *list, struct ds_data data) {
        struct ds_node *nd = lln__new();

        if (nd == NULL) {
                errno = ENOMEM;
                return;
        }
        nd->data = data;

        nd->prev = list->flow;
        nd->next = list->flow->next;
        list->flow->next = nd;
        nd->next->prev = nd;
        list->length += 1;
}

void ll__insert_at(struct ds_linked_list *list, struct ds_data data, size_t at) {
        size_t i;
        struct ds_node *old_flow = list->flow;
        list->flow = list->first;

        if (at >= list->length)
                return;

        for (i = 0; i < at; i++)
                list->flow = list->flow->next;

        ll__insert_front(list, data);
        list->flow = old_flow;
}

void ll__get_first(struct ds_linked_list *list, struct ds_data *data) {
        if (list->length < 1)
                memset(data, 0, sizeof(*data));
        else
                *data = list->first->data;
}

void ll__get_last(struct ds_linked_list *list, struct ds_data *data) {
        if (list->length < 1)
                memset(data, 0, sizeof(*data));
        else
                *data = list->last->prev->data;
}

void ll__remove(struct ds_linked_list *list) {
        struct ds_node *prev = list->flow->prev;
        struct ds_node *next = list->flow->next;

        lln__remove_obj(list->flow, ll__should_delete_data(list));

        if (prev == NULL)
                list->first = next;
        else if (next == NULL)
                list->last = prev;
        if (next == NULL)
                list->flow = prev;
        else
                list->flow = next;
        list->length -= 1;
}

void ll__remove_at(struct ds_linked_list *list, size_t at) {
        size_t i;
        struct ds_node *old_flow = list->flow;
        list->flow = list->first;

        if (at >= list->length)
                return;

        for (i = 0; i < at; i++)
                list->flow = list->flow->next;

        if (old_flow == list->flow) {
                if (old_flow->next == NULL)
                        old_flow = old_flow->prev;
                else
                        old_flow = old_flow->next;
        }
        ll__remove(list);

        list->flow = old_flow;
}

int ll__has_next(struct ds_linked_list *list) {
        return (list->flow->next != NULL);
}

int ll__has_prev(struct ds_linked_list *list) {
        return (list->flow->prev != NULL);
}

void ll__current(struct ds_linked_list *list, struct ds_data *data) {
        *data = list->flow->data;
}

void ll__next(struct ds_linked_list *list, struct ds_data *data) {
        if (!ll__has_next(list)) {
                memset(data, 0, sizeof(*data));
                return;
        }
        list->flow = list->flow->next;
        if (data != NULL)
                ll__current(list, data);
}

void ll__prev(struct ds_linked_list *list, struct ds_data *data) {
        if (!ll__has_prev(list)) {
                memset(data, 0, sizeof(*data));
                return;
        }
        list->flow = list->flow->prev;
        if (data != NULL)
                ll__current(list, data);
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

ll__from_N_array(i8, int8_t, _int, LL_DATA_TYPE_I8)
ll__from_N_array(i16, int16_t, _int, LL_DATA_TYPE_I16)
ll__from_N_array(i32, int32_t, _int, LL_DATA_TYPE_I32)
ll__from_N_array(i64, int64_t, _int, LL_DATA_TYPE_I64)
ll__from_N_array(u8, uint8_t, _uint, LL_DATA_TYPE_U8)
ll__from_N_array(u16, uint16_t, _uint, LL_DATA_TYPE_U16)
ll__from_N_array(u32, uint32_t, _uint, LL_DATA_TYPE_U32)
ll__from_N_array(u64, uint64_t, _uint, LL_DATA_TYPE_U64)
ll__from_N_array(void, void *, _ptr, LL_DATA_TYPE_VOID)

void ll__print(struct ds_linked_list *list) {
        struct ds_node *flow = list->first;

        printf("{");
        while (flow->next != NULL) {
                printf("%lu, ", flow->data._uint);
                flow = flow->next;
        }
        printf("\b\b}");
}

void ll__sprint(char *str, struct ds_linked_list *list) {
        struct ds_node *flow = list->first;

        char *end = str;

        sprintf(end, "{");
        end = strchr(end, '\0');
        while (flow->next != NULL) {
                sprintf(end, "%lu, ", flow->data._uint);
                end = strchr(end, '\0');
                flow = flow->next;
        }
        end[-2] = '}';
        end[-1] = '\0';
        end = strchr(end, '\0');
}

void hm__new(struct ds_hash_map *dest, uint64_t size, uint64_t entry_size) {
        uint64_t i;

        dest->none.key = malloc(entry_size <= 0 ? 1 : entry_size);
        if (dest->none.key == NULL) {
                errno = ENOMEM;
                return;
        }
        memset(&dest->none.value, 0, sizeof(dest->none.value));
        memset(dest->none.key, 0, entry_size <= 0 ? 1 : entry_size);

        dest->buckets = malloc(size * sizeof(struct ds_linked_list));
        if (dest->buckets == NULL) {
                free(dest->none.key);
                errno = ENOMEM;
                return;
        }

        for (i = 0; i < size; i++) {
                ll__new(&dest->buckets[i], LL_DATA_TYPE_HM_ENTRY);
        }

        if (entry_size <= 0)
                dest->hash = hm__default_hash_string;
        else
                dest->hash = hm__default_hash_binary;
        dest->size = size;
        dest->entry_size = entry_size;
}

uint64_t hm__default_hash_string(struct ds_hash_map *hm, const void *key) {
        uint64_t hash = 0x42;
        const char *k = key;

        (void) hm;

        while (*k) {
                hash ^= *k;
                hash *= 7;
                k++;
        }
        return hash;
}

uint64_t hm__default_hash_binary(struct ds_hash_map *hm, const void *key) {
        uint64_t i;
        uint64_t hash = 0x42;
        const uint8_t *k = key;

        for (i = 0; i < hm->entry_size; i++) {
                hash ^= k[i];
                hash *= 7;
        }
        return hash;
}

void hm__put(struct ds_hash_map *hm, const void *key, struct ds_data value) {
        struct ds_hm_entry *ecpy;
        struct ds_data dat;
        uint64_t hash;

        ecpy = malloc(sizeof(struct ds_hm_entry));
        if (ecpy == NULL) {
                errno = ENOMEM;
                return;
        }
        ecpy->value = value;
        if (hm->entry_size <= 0) {
                ecpy->key = calloc(1, strlen(key) + 1);
                strcpy(ecpy->key, key);
        } else {
                ecpy->key = calloc(1, hm->entry_size);
                memcpy(ecpy->key, key, hm->entry_size);
        }
        dat._ptr = ecpy;

        hash = hm->hash(hm, key) % hm->size;

        ll__append(&(hm->buckets[hash]), dat);
}

struct ds_hm_entry hm__get(struct ds_hash_map *hm, const void *key) {
        uint64_t hash = hm->hash(hm, (const uint8_t *) key) % hm->size;
        struct ds_data dat;
        struct ds_hm_entry *entry;
        struct ds_linked_list ll = hm->buckets[hash];

        ll__rewind(&ll);
        while (ll__has_next(&ll)) {
                ll__current(&ll, &dat);
                entry = dat._ptr;
                if (hm->entry_size <= 0) {
                        if (strcmp(key, entry->key) == 0)
                                return *entry;
                } else {
                        if (memcmp(key, entry->key, hm->entry_size) == 0)
                                return *entry;
                }
                ll__next(&ll, NULL);
        }

        return hm->none;
}

void hm__delete(struct ds_hash_map *hm, const void *key) {
        uint64_t hash;
        struct ds_data dat;
        struct ds_hm_entry *entry;
        struct ds_linked_list ll;

        hash = hm->hash(hm, key) % hm->size;
        ll = hm->buckets[hash];


        ll__rewind(&ll);
        while (ll__has_next(&ll)) {
                ll__current(&ll, &dat);
                entry = dat._ptr;
                if (strcmp(key, entry->key) == 0) {
                        ll__prev(&ll, &dat);
                        ll__prev(&ll, &dat);
                        free(entry);
                        ll__remove(&ll);
                        hm->buckets[hash] = ll;
                        return;
                }
                ll__next(&ll, NULL);
        }
}
