#include <signal.h>
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "irc.h"

START_TEST(circular_buffer)
{
    printf("Testing circular_buffer...");

	t_circular_buffer	buf;

	buf.begin = 1;
	buf.end = 1;
	buf.buffer = (void*)1;
	buf.buffer_size = 1;

	/*
	** Test initialisation
	*/
	circular_buffer_init(&buf, 10);

	ck_assert_int_eq(buf.begin, 0);
	ck_assert_int_eq(buf.end, 0);
	ck_assert_uint_eq(buf.buffer_size, 10);
	ck_assert_ptr_ne(buf.buffer, (void*)1);

	/*
	** Test destruction
	*/
	circular_buffer_destroy(&buf);

	ck_assert_int_eq(buf.begin, 0);
	ck_assert_int_eq(buf.end, 0);
	ck_assert_uint_eq(buf.buffer_size, 0);
	ck_assert_ptr_eq(buf.buffer, (void*)0);

	/*
	** Test writing
	*/
	circular_buffer_init(&buf, 10);

	circular_buffer_write(&buf, "Hello!", 6);
	ck_assert(strncmp("Hello!", buf.buffer, 6) == 0);

	circular_buffer_write(&buf, "Hello!", 6);
	ck_assert(strncmp("o!llo!Hell", buf.buffer, 10) == 0);

	circular_buffer_write(&buf, "Oh no", 5);
	ck_assert(strncmp("o!Oh noell", buf.buffer, 10) == 0);

	char *very_long_line =
"hahaha this test is awesome. You know what? I want lambooooooos! Buy bitcoin. I bought bitcoin at 4$! No I joke :(. Everyone who will write 'Bitcoin' in github search bar will find this repository. Haha your are cursed by reading this very long line. No luck to you. You can send me a thank letter at yourecursed@hell.com. Thanks, bye!";
	circular_buffer_write(&buf, very_long_line, strlen(very_long_line));
	ck_assert(strncmp("s, bye!ank", buf.buffer, 10) == 0);

	circular_buffer_write(&buf, "b\nLLLL", 2);
	ck_assert(strncmp("s, bye!b\nk", buf.buffer, 10) == 0);

	circular_buffer_write(&buf, "hello", 5);
	ck_assert(strncmp("elloye!b\nh", buf.buffer, 10) == 0);

	/*
	** Test reading
	*/
	char *ret;
	size_t ret_size;

	memcpy(buf.buffer, "123456789\n", 10);
	buf.begin = 0;
	buf.end = 0;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(ret_size, 10);
	ck_assert(strncmp("123456789\n", ret, 10) == 0);
	ck_assert_uint_eq(buf.begin, 0);
	ck_assert_uint_eq(buf.end, 0);
	free(ret);
	ret = NULL;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(buf.begin, 0);
	ck_assert_uint_eq(buf.end, 0);
	ck_assert_uint_eq(ret_size, 0);
	ck_assert_ptr_eq(ret, NULL);

	memcpy(buf.buffer, "\n123456789", 10);
	buf.begin = 1;
	buf.end = 1;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(ret_size, 10);
	ck_assert(strncmp("123456789\n", ret, 10) == 0);
	ck_assert_uint_eq(buf.begin, 1);
	ck_assert_uint_eq(buf.end, 1);
	free(ret);
	ret = NULL;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(ret_size, 0);
	ck_assert_ptr_eq(ret, NULL);
	ck_assert_uint_eq(buf.begin, 1);
	ck_assert_uint_eq(buf.end, 1);
	free(ret);

	memcpy(buf.buffer, "9\n12345678", 10);
	buf.begin = 2;
	buf.end = 2;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(ret_size, 10);
	ck_assert(strncmp("123456789\n", ret, 10) == 0);
	ck_assert_uint_eq(buf.begin, 2);
	ck_assert_uint_eq(buf.end, 2);
	free(ret);
	ret = NULL;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(ret_size, 0);
	ck_assert_ptr_eq(ret, NULL);
	ck_assert_uint_eq(buf.begin, 2);
	ck_assert_uint_eq(buf.end, 2);
	free(ret);

	memcpy(buf.buffer, "456789\n123", 10);
	buf.begin = 7;
	buf.end = 7;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(ret_size, 10);
	ck_assert(strncmp("123456789\n", ret, 10) == 0);
	ck_assert_uint_eq(buf.begin, 7);
	ck_assert_uint_eq(buf.end, 7);
	free(ret);
	ret = NULL;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(ret_size, 0);
	ck_assert_ptr_eq(ret, NULL);
	ck_assert_uint_eq(buf.begin, 7);
	ck_assert_uint_eq(buf.end, 7);
	free(ret);

	memcpy(buf.buffer, "23456789\n1", 10);
	buf.begin = 9;
	buf.end = 9;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(ret_size, 10);
	ck_assert(strncmp("123456789\n", ret, 10) == 0);
	ck_assert_uint_eq(buf.begin, 9);
	ck_assert_uint_eq(buf.end, 9);
	free(ret);
	ret = NULL;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(ret_size, 0);
	ck_assert_ptr_eq(ret, NULL);
	ck_assert_uint_eq(buf.begin, 9);
	ck_assert_uint_eq(buf.end, 9);
	free(ret);

	memcpy(buf.buffer, "a\nbc\nd\nef\n", 10);
	buf.begin = 0;
	buf.end = 0;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(ret_size, 2);
	ck_assert(strncmp("a\n", ret, 2) == 0);
	ck_assert_uint_eq(buf.begin, 2);
	ck_assert_uint_eq(buf.end, 0);
	free(ret);
	ret = NULL;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(ret_size, 3);
	ck_assert(strncmp("bc\n", ret, 3) == 0);
	ck_assert_uint_eq(buf.begin, 5);
	ck_assert_uint_eq(buf.end, 0);
	free(ret);
	ret = NULL;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(ret_size, 2);
	ck_assert(strncmp("d\n", ret, 2) == 0);
	ck_assert_uint_eq(buf.begin, 7);
	ck_assert_uint_eq(buf.end, 0);
	free(ret);
	ret = NULL;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(ret_size, 3);
	ck_assert(strncmp("ef\n", ret, 3) == 0);
	ck_assert_uint_eq(buf.begin, 0);
	ck_assert_uint_eq(buf.end, 0);
	free(ret);
	ret = NULL;

	ret_size = circular_buffer_read(&buf, '\n', &ret);
	ck_assert_uint_eq(ret_size, 0);
	ck_assert_ptr_eq(ret, NULL);
	ck_assert_uint_eq(buf.begin, 0);
	ck_assert_uint_eq(buf.end, 0);
	free(ret);


    printf(" done!\n");
}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("IRC");
    TCase *tc1_1 = tcase_create("IRC");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, circular_buffer);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}