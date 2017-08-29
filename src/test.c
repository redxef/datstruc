/**
 * @author      redxef
 * @file        test.c
 * @version     0.0.0-r0
 * @since       
 * 
 * @brief       A brief documentation about the file.
 *
 * A detailed documentation.
 */

#include "datstruc.h"
#include <assert.h>

/* V A R I A B L E S                                                          */


/* C O D E                                                                    */
int test_list(void) {
        uint8_t src[3] = {'a', 'b', 'c'};
        char str[1024];
        uint8_t arr[10];
        struct data data;
        struct ll_linked_list ll;

        ll__new(&ll);
        ll__from_u8_array(&ll, src, 3);
        
        ll__sprint(str, &ll);
        assert(strcmp("{97, 98, 99}", str) == 0);

        ll__next(&ll, &data);
        assert(data._uint == 'a');

        ll__next(&ll, &data);
        assert(data._uint == 'b');

        ll__insert(&ll, (struct data) {LL_DATA_TYPE_U8, {'d'}});
        ll__sprint(str, &ll);
        assert(strcmp("{97, 98, 99, 100}", str) == 0);
        ll__insert_at(&ll, (struct data) {LL_DATA_TYPE_U8, {'e'}}, 3);
        ll__sprint(str, &ll);
        assert(strcmp("{97, 98, 99, 101, 100}", str) == 0);
        ll__remove_at(&ll, 4);
        ll__sprint(str, &ll);
        assert(strcmp("{97, 98, 99, 101}", str) == 0);
        ll__remove_at(&ll, 3);
        ll__sprint(str, &ll);
        assert(strcmp("{97, 98, 99}", str) == 0);
        ll__next(&ll, &data);
        assert(data._uint == 'c');

        ll__remove(&ll);
        assert(ll.length == 2);

        ll__to_u8_array(&ll, arr);
        // assert(arr[0] == 'a');
        // assert(arr[1] == 'c');

        ll__clear(&ll);

        return 0;
}

int test_hash_map(void) {
        struct hm_hash_map hm;
        struct hm_entry ent;

        hm__new(&hm, 3);
        hm.hash = hm__default_hash;

        hm__put(&hm, (struct hm_entry) {"ABC", {LL_DATA_TYPE_U64, {101}}});
        hm__put(&hm, (struct hm_entry) {"B", {LL_DATA_TYPE_U64, {102}}});
        hm__put(&hm, (struct hm_entry) {"C", {LL_DATA_TYPE_U64, {103}}});
        hm__put(&hm, (struct hm_entry) {"D", {LL_DATA_TYPE_U64, {104}}});
        hm__put(&hm, (struct hm_entry) {"E", {LL_DATA_TYPE_U64, {105}}});
        hm__put(&hm, (struct hm_entry) {"F", {LL_DATA_TYPE_U64, {106}}});


        ent = hm__get(&hm, "ABC");
        assert(ent.value.type == LL_DATA_TYPE_U64);
        assert(ent.value._uint == 101);
        ent = hm__get(&hm, "C");
        assert(ent.value.type == LL_DATA_TYPE_U64);
        assert(ent.value._uint == 103);
        ent = hm__get(&hm, "B");
        assert(ent.value.type == LL_DATA_TYPE_U64);
        assert(ent.value._uint == 102);
        ent = hm__get(&hm, "D");
        assert(ent.value.type == LL_DATA_TYPE_U64);
        assert(ent.value._uint == 104);
        ent = hm__get(&hm, "E");
        assert(ent.value.type == LL_DATA_TYPE_U64);
        assert(ent.value._uint == 105);
        ent = hm__get(&hm, "F");
        assert(ent.value.type == LL_DATA_TYPE_U64);
        assert(ent.value._uint == 106);

        hm__delete(&hm, "ABC");

        ent = hm__get(&hm, "ABC");
        assert(ent.value.type == LL_DATA_TYPE_NONE);
        assert(ent.value._uint == 0);
        ent = hm__get(&hm, "C");
        assert(ent.value.type == LL_DATA_TYPE_U64);
        assert(ent.value._uint == 103);
        ent = hm__get(&hm, "B");
        assert(ent.value.type == LL_DATA_TYPE_U64);
        assert(ent.value._uint == 102);
        ent = hm__get(&hm, "D");
        assert(ent.value.type == LL_DATA_TYPE_U64);
        assert(ent.value._uint == 104);
        ent = hm__get(&hm, "E");
        assert(ent.value.type == LL_DATA_TYPE_U64);
        assert(ent.value._uint == 105);
        ent = hm__get(&hm, "F");
        assert(ent.value.type == LL_DATA_TYPE_U64);
        assert(ent.value._uint == 106);
        return 0;
}

int main(int argc, char **argv) {
        printf("test_list: ");
        test_list()? printf("failed\n") : printf("passed\n");
        printf("test_hash_map: ");
        test_hash_map()? printf("failed\n") : printf("passed\n");
        return 0;
}
