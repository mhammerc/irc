#include <signal.h>
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "irc.h"

START_TEST(hash_table)
{
	printf("Testing hash_table... ");

	t_hash_table 	*ht;
	char			*key1;
	char			*el1;
	char			*key2;
	char			*el2;
	char			*key3;
	char			*el3;
	char			*ret;

	ht = ht_create(10);
	
	key1 = "Hello!\n";
	el1 = "oh yeah";
	key2 = "nop";
	el2 = "LOL";
	key3 = "Hello!\nMy name is Martin";
	el3 = "oh yeah, I hope everything is going to work.";

	ht_insert(ht, key1, strlen(key1), el1);
	ret = ht_search(ht, key1, strlen(key1));
	ck_assert_str_eq(ret, el1);
	ck_assert_uint_eq(ht_num_elements_saved(ht), 1);
	ht_delete(ht, key1, strlen(key1));
	ret = ht_search(ht, key1, strlen(key1));
	ck_assert_ptr_eq(ret, NULL);
	ck_assert_uint_eq(ht_num_elements_saved(ht), 0);
	ht_destroy(ht);

	ht = ht_create(1);
	ck_assert_uint_eq(ht_num_elements_saved(ht), 0);
	ht_insert(ht, key1, strlen(key1), el1);
	ck_assert_uint_eq(ht_num_elements_saved(ht), 1);
	ht_insert(ht, key2, strlen(key2), el2);
	ck_assert_uint_eq(ht_num_elements_saved(ht), 2);
	ht_insert(ht, key3, strlen(key3), el3);
	ck_assert_uint_eq(ht_num_elements_saved(ht), 3);
	
	ret = ht_search(ht, key1, strlen(key1));
	ck_assert_str_eq(ret, el1);

	ret = ht_search(ht, key2, strlen(key2));
	ck_assert_str_eq(ret, el2);

	ret = ht_search(ht, key3, strlen(key3));
	ck_assert_str_eq(ret, el3);

	ht_delete(ht, key2, strlen(key2));
	ck_assert_uint_eq(ht_num_elements_saved(ht), 2);

	ret = ht_search(ht, key1, strlen(key1));
	ck_assert_str_eq(ret, el1);

	ret = ht_search(ht, key2, strlen(key2));
	ck_assert_ptr_eq(ret, NULL);

	ret = ht_search(ht, key3, strlen(key3));
	ck_assert_str_eq(ret, el3);

	ht_delete(ht, key1, strlen(key1));
	ck_assert_uint_eq(ht_num_elements_saved(ht), 1);

	ret = ht_search(ht, key1, strlen(key1));
	ck_assert_ptr_eq(ret, NULL);

	ret = ht_search(ht, key2, strlen(key2));
	ck_assert_ptr_eq(ret, NULL);

	ret = ht_search(ht, key3, strlen(key3));
	ck_assert_str_eq(ret, el3);

	ht_destroy(ht);

	unsigned int ret2;

	ret2 = ht_hash(key1, strlen(key1));
	ck_assert_uint_eq(ret2, 2136292893);

	ret2 = ht_hash(key2, strlen(key2));
	ck_assert_uint_eq(ret2, 1119791147);

	ret2 = ht_hash(key3, strlen(key3));
	ck_assert_uint_eq(ret2, 4038996974);

	printf("done!\n");
}
END_TEST

START_TEST(message_parser)
{
	printf("Testing message_parser... ");

	char *str;
	
	t_irc_message *message;

	message = irc_message_parse("LIST\r\n", 6);
	ck_assert_ptr_ne(message, NULL);
	ck_assert_str_eq(message->command, "LIST");
	ck_assert_uint_eq(message->params_size, 0);
	ck_assert_ptr_eq(*message->params, 0);
	irc_message_destroy(&message);

	str = "JOIN channel\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_ne(message, NULL);
	ck_assert_str_eq(message->command, "JOIN");
	ck_assert_uint_eq(message->params_size, 1);
	ck_assert_str_eq(message->params[0], "channel");
	ck_assert_ptr_eq(message->params[1], NULL);
	irc_message_destroy(&message);

	str = "PRIVMSG asm_dev :Salut les gars :D Vous allez bien ?\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_ne(message, NULL);
	ck_assert_str_eq(message->command, "PRIVMSG");
	ck_assert_uint_eq(message->params_size, 2);
	ck_assert_str_eq(message->params[0], "asm_dev");
	ck_assert_str_eq(message->params[1], "Salut les gars :D Vous allez bien ?");
	ck_assert_ptr_eq(message->params[2], NULL);
	irc_message_destroy(&message);

	// an IRC command is only letters. Thus, an '_' should not work.
	str = "FAKE_COMMAND :etoui je teste bien les doubles points\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_eq(message, NULL);

	str = "CAPS LS 302\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_ne(message, NULL);
	ck_assert_str_eq(message->command, "CAPS");
	ck_assert_uint_eq(message->params_size, 2);
	ck_assert_str_eq(message->params[0], "LS");
	ck_assert_str_eq(message->params[1], "302");
	ck_assert_ptr_eq(message->params[2], NULL);
	irc_message_destroy(&message);

	// an IRC command can also be 3 digits.
	str = "001\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_ne(message, NULL);
	ck_assert_str_eq(message->command, "001");
	ck_assert_uint_eq(message->params_size, 0);
	ck_assert_ptr_eq(message->params[0], NULL);
	irc_message_destroy(&message);

	str = "420\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_ne(message, NULL);
	ck_assert_str_eq(message->command, "420");
	ck_assert_uint_eq(message->params_size, 0);
	ck_assert_ptr_eq(message->params[0], NULL);
	irc_message_destroy(&message);

	// a digit IRC command is strictly 3 digits
	str = "01\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_eq(message, NULL);

	str = "1\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_eq(message, NULL);

	str = "0001\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_eq(message, NULL);

	// We can not mix digits and letters
	str = "A001\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_eq(message, NULL);

	str = "LIST001\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_eq(message, NULL);

	str = "001LIST\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_eq(message, NULL);

	str = "A01LIST\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_eq(message, NULL);

	// Testing spaces, and empty messages
	str = "   CAPS    LS    302   \r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_ne(message, NULL);
	ck_assert_str_eq(message->command, "CAPS");
	ck_assert_uint_eq(message->params_size, 2);
	ck_assert_str_eq(message->params[0], "LS");
	ck_assert_str_eq(message->params[1], "302");
	ck_assert_ptr_eq(message->params[2], NULL);
	irc_message_destroy(&message);

	str = "   CAPS        LS 302\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_ne(message, NULL);
	ck_assert_str_eq(message->command, "CAPS");
	ck_assert_uint_eq(message->params_size, 2);
	ck_assert_str_eq(message->params[0], "LS");
	ck_assert_str_eq(message->params[1], "302");
	ck_assert_ptr_eq(message->params[2], NULL);
	irc_message_destroy(&message);

	str = "   CAPS        LS      :302\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_ne(message, NULL);
	ck_assert_str_eq(message->command, "CAPS");
	ck_assert_uint_eq(message->params_size, 2);
	ck_assert_str_eq(message->params[0], "LS");
	ck_assert_str_eq(message->params[1], "302");
	ck_assert_ptr_eq(message->params[2], NULL);
	irc_message_destroy(&message);

	str = "\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_eq(message, NULL);

	str = "      \r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_eq(message, NULL);

	str = "";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_eq(message, NULL);

	str = "    ";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_eq(message, NULL);

	str = "\r\n\r\n";
	message = irc_message_parse(str, strlen(str));
	ck_assert_ptr_eq(message, NULL);

	printf("done!\n");
}
END_TEST

START_TEST(circular_buffer)
{
    printf("Testing circular_buffer... ");

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


    printf("done!\n");
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
    tcase_add_test(tc1_1, message_parser);
    tcase_add_test(tc1_1, hash_table);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}