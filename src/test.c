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

/* V A R I A B L E S                                                          */


/* C O D E                                                                    */
// int main(int argc, char **argv) {
//         struct data data;

//         struct ll_linked_list ll;
//         ll__new(&ll);
//         ll__append(&ll, (struct data) {LL_DATA_TYPE_U8, {'a'}});
//         ll__append(&ll, (struct data) {LL_DATA_TYPE_U8, {'b'}});
//         ll__append(&ll, (struct data) {LL_DATA_TYPE_U8, {'c'}});
//         ll__print(&ll);

//         ll__next(&ll, &data);
//         printf("%c\n", (char) data._uint);

//         ll__next(&ll, &data);
//         printf("%c\n", (char) data._uint);

//         ll__insert(&ll, (struct data) {LL_DATA_TYPE_U8, {'d'}});
//         ll__print(&ll);
//         ll__insert_at(&ll, (struct data) {LL_DATA_TYPE_U8, {'e'}}, 3);
//         ll__print(&ll);
//         ll__remove_at(&ll, 4);
//         ll__print(&ll);
//         ll__remove_at(&ll, 3);
//         ll__print(&ll);
//         ll__next(&ll, &data);
//         printf("%c\n", (char) data._uint);

//         ll__remove(&ll);
//         printf("%llu\n", ll.length);
//         ll__print(&ll);
//         return 0;
// }

int main(int argc, char **argv) {
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
        printf("%llu: %llu\n", ent.value.type, ent.value._uint);
        ent = hm__get(&hm, "C");
        printf("%llu: %llu\n", ent.value.type, ent.value._uint);
        ent = hm__get(&hm, "B");
        printf("%llu: %llu\n", ent.value.type, ent.value._uint);
        ent = hm__get(&hm, "D");
        printf("%llu: %llu\n", ent.value.type, ent.value._uint);
        ent = hm__get(&hm, "E");
        printf("%llu: %llu\n", ent.value.type, ent.value._uint);
        ent = hm__get(&hm, "F");
        printf("%llu: %llu\n", ent.value.type, ent.value._uint);
        printf("---\n");

        hm__delete(&hm, "ABC");

        ent = hm__get(&hm, "ABC");
        printf("%llu: %llu\n", ent.value.type, ent.value._uint);
        ent = hm__get(&hm, "C");
        printf("%llu: %llu\n", ent.value.type, ent.value._uint);
        ent = hm__get(&hm, "B");
        printf("%llu: %llu\n", ent.value.type, ent.value._uint);
        ent = hm__get(&hm, "D");
        printf("%llu: %llu\n", ent.value.type, ent.value._uint);
        ent = hm__get(&hm, "E");
        printf("%llu: %llu\n", ent.value.type, ent.value._uint);
        ent = hm__get(&hm, "F");
        printf("%llu: %llu\n", ent.value.type, ent.value._uint);
        printf("---\n");

        return 0;
}
