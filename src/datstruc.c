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

struct ll_linked_list *ll__new(struct ll_linked_list *dest) {
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
                *data = (struct data) { LL_DATA_TYPE_NONE, {0}};
                return;
        }
        if (data != NULL)
                *data = list->flow->data;
        list->flow = list->flow->next;
}

void ll__prev(struct ll_linked_list *list, struct data *data) {
        if (!ll__has_prev(list)) {
                *data = (struct data) { LL_DATA_TYPE_NONE, {0}};
                return;
        }
        if (data != NULL)
                *data = list->flow->prev->data;
        list->flow = list->flow->prev;
}

void ll__print(struct ll_linked_list *list) {
        struct ll_node *flow = list->first;

        printf("{");
        while (flow->next != NULL) {
                printf("%llu, ", flow->data._uint);
                flow = flow->next;
        }
        printf("\b\b}\n");
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
                ll__new(&dest->buckets[i]);
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
        dat.type = LL_DATA_HM_ENTRY;
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
        return (struct hm_entry) {"", {LL_DATA_TYPE_NONE, {0}}};
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
