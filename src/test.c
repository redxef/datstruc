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
        struct ds_data data;
        struct ds_linked_list ll;

        ll__new(&ll, LL_DATA_TYPE_U8);
        ll__from_u8_array(&ll, src, 3);

        ll__sprint(str, &ll);
        assert(strcmp("{97, 98, 99}", str) == 0);

        ll__next(&ll, &data);
        assert(data._uint == 'a');

        ll__next(&ll, &data);
        assert(data._uint == 'b');

        ll__insert(&ll, (struct ds_data) {{'d'}});
        ll__sprint(str, &ll);
        assert(strcmp("{97, 98, 99, 100}", str) == 0);
        ll__insert_at(&ll, (struct ds_data) {{'e'}}, 3);
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
        assert(arr[0] == 'a');
        assert(arr[1] == 'b');

        ll__get_first(&ll, &data);
        assert(data._uint == 'a');

        ll__get_last(&ll, &data);
        assert(data._uint == 'b');

        ll__clear(&ll);

        return 0;
}

int test_hash_map(void) {
        struct ds_hash_map hm;
        struct ds_hm_entry ent;

        hm__new(&hm, 3);
        hm.hash = hm__default_hash_string;

        hm__put(&hm, (struct ds_hm_entry) {"ABC", {{101}}});
        hm__put(&hm, (struct ds_hm_entry) {"B", {{102}}});
        hm__put(&hm, (struct ds_hm_entry) {"C", {{103}}});
        hm__put(&hm, (struct ds_hm_entry) {"D", {{104}}});
        hm__put(&hm, (struct ds_hm_entry) {"E", {{105}}});
        hm__put(&hm, (struct ds_hm_entry) {"F", {{106}}});


        ent = hm__get(&hm, "ABC");
        assert(ent.value._uint == 101);
        ent = hm__get(&hm, "C");
        assert(ent.value._uint == 103);
        ent = hm__get(&hm, "B");
        assert(ent.value._uint == 102);
        ent = hm__get(&hm, "D");
        assert(ent.value._uint == 104);
        ent = hm__get(&hm, "E");
        assert(ent.value._uint == 105);
        ent = hm__get(&hm, "F");
        assert(ent.value._uint == 106);

        hm__delete(&hm, "ABC");

        ent = hm__get(&hm, "ABC");

        assert(ent.value._uint == 0);
        ent = hm__get(&hm, "C");
        assert(ent.value._uint == 103);
        ent = hm__get(&hm, "B");
        assert(ent.value._uint == 102);
        ent = hm__get(&hm, "D");
        assert(ent.value._uint == 104);
        ent = hm__get(&hm, "E");
        assert(ent.value._uint == 105);
        ent = hm__get(&hm, "F");
        assert(ent.value._uint == 106);
        return 0;
}

int main(int argc, char **argv) {
        (void) argc;
        (void) argv;

        printf("test_list: ");
        test_list()? printf("failed\n") : printf("passed\n");
        printf("test_hash_map: ");
        test_hash_map()? printf("failed\n") : printf("passed\n");
        return 0;
}
