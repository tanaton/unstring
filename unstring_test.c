#include "unstring.h"
#include <string.h>
#include <stdio.h>

static void check_int(int a, int b);
static void check_unstr(const unstr_t *s1, const unstr_t *s2);
static void check_unstr_char(const unstr_t *s1, const char *s2);
static void check_char(const char *s1, const char *s2);
static void check_null(void *p);
static void test_assert(int flag);

static void test_unstr_init(void);
static void test_unstr_init_memory(void);
static void test_unstr_alloc(void);
static void test_unstr_free(void);
//static void test_unstr_free_func(void);
static void test_unstr_delete(void);
static void test_unstr_zero(void);
static void test_unstr_isset(void);
static void test_unstr_empty(void);
static void test_unstr_strlen(void);
static void test_unstr_write(void);
static void test_unstr_copy(void);
static void test_unstr_strcpy(void);
static void test_unstr_strcpy_char(void);
static void test_unstr_substr(void);
static void test_unstr_substr_char(void);
static void test_unstr_strcat(void);
static void test_unstr_strcat_char(void);
static void test_unstr_strcmp(void);
static void test_unstr_strcmp_char(void);
static void test_unstr_strstr(void);
static void test_unstr_strstr_char(void);
static void test_unstr_explode(void);
static void test_unstr_sprintf(void);
static void test_unstr_sscanf(void);
static void test_unstr_reverse(void);
static void test_unstr_itoa(void);
//static void test_unstr_file_get_contents(void);
//static void test_unstr_file_put_contents(void);
static void test_unstr_replace(void);
static void test_unstr_strpos(void);
static void test_unstr_substr_count(void);
static void test_unstr_substr_count_char(void);
static void test_unstr_strtok(void);
static void test_unstr_repeat(void);
static void test_unstr_repeat_char(void);


int main(int argc, char *argv[])
{
	test_unstr_init();
	test_unstr_init_memory();
	test_unstr_alloc();
	test_unstr_free();
	//test_unstr_free_func();
	test_unstr_delete();
	test_unstr_zero();
	test_unstr_isset();
	test_unstr_empty();
	test_unstr_strlen();
	test_unstr_write();
	test_unstr_copy();
	test_unstr_strcpy();
	test_unstr_strcpy_char();
	test_unstr_substr();
	test_unstr_substr_char();
	test_unstr_strcat();
	test_unstr_strcat_char();
	test_unstr_strcmp();
	test_unstr_strcmp_char();
	test_unstr_strstr();
	test_unstr_strstr_char();
	test_unstr_explode();
	test_unstr_sprintf();
	test_unstr_sscanf();
	test_unstr_reverse();
	test_unstr_itoa();
	//test_unstr_file_get_contents();
	//test_unstr_file_put_contents();
	test_unstr_replace();
	test_unstr_strpos();
	test_unstr_substr_count();
	test_unstr_substr_count_char();
	test_unstr_strtok();
	test_unstr_repeat();
	test_unstr_repeat_char();
	return 0;
}

static void check_int(int a, int b)
{
	if(a != b){
		printf("数値が一致しませんでした %d != %d\n", a, b);
	}
}

static void check_unstr(const unstr_t *s1, const unstr_t *s2)
{
	if(unstr_strcmp(s1, s2)){
		printf(
			"文字列が一致しませんでした\n"
			"\t'%s' != '%s'\n",
			s1->data, s2->data
		);
	}
}

static void check_unstr_char(const unstr_t *s1, const char *s2)
{
	if(unstr_strcmp_char(s1, s2)){
		printf(
			"文字列が一致しませんでした\n"
			"\t'%s' != '%s'\n",
			s1->data, s2
		);
	}
}

static void check_char(const char *s1, const char *s2)
{
	if(strcmp(s1, s2)){
		printf(
			"文字列が一致しませんでした\n"
			"\t'%s' != '%s'\n",
			s1, s2
		);
	}
}

static void check_null(void *p)
{
	if(p != NULL){
		printf("NULLチェック失敗\n");
	}
}

static void test_assert(int flag)
{
	if(!flag){
		printf("比較失敗\n");
	}
}

/*================================================================*/

static void test_unstr_init(void)
{
	char *s = "1234567890";
	unstr_t *str = 0;
	printf("unstr_init\n");
	{
		str = unstr_init(NULL);
		check_null(str);
		unstr_free(str);
	}{
		str = unstr_init(s);
		check_int(unstr_strlen(str), 10);
		check_unstr_char(str, s);
		unstr_free(str);
	}
}

static void test_unstr_init_memory(void)
{
	unstr_t *str = 0;
	size_t size = 0;
	printf("unstr_init_memory\n");
	{
		str = unstr_init_memory(0);
		check_null(str);
		unstr_free(str);
	}{
		size = 10;
		str = unstr_init_memory(size);
		if(str->heap < size){
			printf("メモリ確保(%lu)に失敗\n", size);
		}
		unstr_free(str);
	}{
		size = 1024 * 1024 * 1024;
		str = unstr_init_memory(size);
		if(str->heap < size){
			printf("メモリ確保(%lu)に失敗\n", size);
		}
		unstr_free(str);
	}
}

static void test_unstr_alloc(void)
{
	unstr_t *str = unstr_init_memory(10);
	size_t hsize = str->heap;
	size_t i = 0;
	size_t s[3] = {
		10,
		0,
		1024 * 1024 * 1024
	};
	printf("unstr_alloc\n");
	for(i = 0; i < 3; i++){
		unstr_alloc(str, s[i]);
		if(str->heap < (hsize + s[i])){
			printf("メモリ拡張(+%lu)に失敗\n", s[i]);
		}
		hsize = str->heap;
	}
	unstr_free(str);
}

static void test_unstr_free(void)
{
	unstr_t *str = unstr_init_memory(100);
	printf("unstr_free\n");
	unstr_free(str);
	check_null(str);
}

static void test_unstr_delete(void)
{
	unstr_t *str[3] = {0};
	printf("unstr_delete\n");
	str[1] = unstr_init_memory(100);
	unstr_delete(3, str[0], str[1], str[2]);
}

static void test_unstr_zero(void)
{
	unstr_t *str = unstr_init("unkokkokussakusa");
	printf("unstr_zero\n");
	unstr_zero(str);
	check_int(str->length, 0);
	unstr_free(str);
}

static void test_unstr_isset(void)
{
	unstr_t *str = unstr_init("unkokkokussakusa");
	printf("unstr_isset\n");
	test_assert(unstr_isset(NULL) == UNSTRING_FALSE);
	test_assert(unstr_isset(str) == UNSTRING_TRUE);
	unstr_zero(str);
	test_assert(unstr_isset(str) == UNSTRING_TRUE);
	unstr_free(str);
}

static void test_unstr_empty(void)
{
	unstr_t *str = unstr_init("unkokkokussakusa");
	printf("unstr_empty\n");
	test_assert(unstr_empty(NULL) == UNSTRING_TRUE);
	test_assert(unstr_empty(str) == UNSTRING_FALSE);
	unstr_zero(str);
	test_assert(unstr_empty(str) == UNSTRING_TRUE);
	unstr_free(str);
}

static void test_unstr_strlen(void)
{
	unstr_t *str = unstr_init("1234567890");
	printf("unstr_strlen\n");
	check_int(unstr_strlen(NULL), 0);
	check_int(unstr_strlen(str), 10);
	unstr_zero(str);
	check_int(unstr_strlen(str), 0);
	unstr_free(str);
}

static void test_unstr_write(void)
{
	char *ans = "1234567890";
	char ans2[] = "1234567890 1234567890";
	unstr_t *str = unstr_init_memory(1);
	ans2[10] = '\0';
	printf("unstr_write\n");
	test_assert(unstr_write(NULL, ans, 0, 0) == UNSTRING_FALSE);
	test_assert(unstr_write(NULL, "", 0, 0) == UNSTRING_FALSE);
	test_assert(unstr_write(str, NULL, 0, 0) == UNSTRING_FALSE);

	test_assert(unstr_write(str, ans, 0, 10) == UNSTRING_TRUE);
	check_unstr_char(str, ans);
	test_assert(unstr_write(str, "", 10, 0) == UNSTRING_TRUE);
	check_unstr_char(str, ans);
	test_assert(unstr_write(str, ans, 11, 10) == UNSTRING_TRUE);
	test_assert(memcmp(str->data, ans2, 21) == 0);

	unstr_free(str);
}

static void test_unstr_copy(void)
{
	unstr_t *tmp = 0;
	unstr_t *str = unstr_init("1234567890");
	printf("unstr_copy\n");
	tmp = unstr_copy(NULL);
	check_null(tmp);
	unstr_free(tmp);

	tmp = unstr_copy(str);
	check_int(unstr_strlen(str), unstr_strlen(tmp));
	check_unstr(str, tmp);

	unstr_delete(2, str, tmp);
}

static void test_unstr_strcpy(void)
{
	unstr_t *tmp = unstr_init_memory(1);
	unstr_t *emp = unstr_init_memory(1);
	unstr_t *str = unstr_init("unkokkokussakusa");
	printf("unstr_strcpy\n");
	test_assert(unstr_strcpy(NULL, str) == UNSTRING_FALSE);
	test_assert(unstr_strcpy(NULL, tmp) == UNSTRING_FALSE);
	test_assert(unstr_strcpy(tmp, NULL) == UNSTRING_FALSE);

	test_assert(unstr_strcpy(tmp, str) == UNSTRING_TRUE);
	check_unstr(tmp, str);

	test_assert(unstr_strcpy(tmp, emp) == UNSTRING_TRUE);
	test_assert(unstr_empty(tmp) == UNSTRING_TRUE);

	unstr_delete(3, str, tmp, emp);
}

static void test_unstr_strcpy_char(void)
{
	char *str = "unkokkokussakusa";
	unstr_t *tmp = unstr_init_memory(1);
	printf("unstr_strcpy_char\n");
	test_assert(unstr_strcpy_char(NULL, str) == UNSTRING_FALSE);
	test_assert(unstr_strcpy_char(NULL, "") == UNSTRING_FALSE);
	test_assert(unstr_strcpy_char(tmp, NULL) == UNSTRING_FALSE);

	test_assert(unstr_strcpy_char(tmp, str) == UNSTRING_TRUE);
	check_unstr_char(tmp, str);

	test_assert(unstr_strcpy_char(tmp, "") == UNSTRING_TRUE);
	test_assert(unstr_empty(tmp) == UNSTRING_TRUE);

	unstr_free(tmp);
}

static void test_unstr_substr(void)
{
	char *ans[] = {
		"123",
		"unkokko"
	};
	unstr_t *tmp = unstr_init_memory(1);
	unstr_t *emp = unstr_init_memory(1);
	unstr_t *str = unstr_init("1234567890");
	printf("unstr_substr\n");
	test_assert(unstr_substr(NULL, str, 3) == UNSTRING_FALSE);
	test_assert(unstr_substr(tmp, NULL, 3) == UNSTRING_FALSE);
	test_assert(unstr_substr(tmp, emp, 3) == UNSTRING_FALSE);

	test_assert(unstr_substr(tmp, str, 0) == UNSTRING_TRUE);
	test_assert(unstr_empty(tmp) == UNSTRING_TRUE);

	test_assert(unstr_substr(tmp, str, 3) == UNSTRING_TRUE);
	check_unstr_char(tmp, ans[0]);

	unstr_strcpy_char(str, "unkokkokussakusa");
	test_assert(unstr_substr(tmp, str, 7) == UNSTRING_TRUE);
	check_unstr_char(tmp, ans[1]);

	unstr_delete(3, str, tmp, emp);
}

static void test_unstr_substr_char(void)
{
	char *ans[] = {
		"123",
		"unkokko"
	};
	unstr_t *tmp = unstr_init_memory(1);
	char *str = "1234567890";
	printf("unstr_substr_char\n");
	test_assert(unstr_substr_char(NULL, str, 3) == UNSTRING_FALSE);
	test_assert(unstr_substr_char(tmp, NULL, 3) == UNSTRING_FALSE);
	test_assert(unstr_substr_char(tmp, "", 3) == UNSTRING_FALSE);

	test_assert(unstr_substr_char(tmp, str, 0) == UNSTRING_TRUE);
	test_assert(unstr_empty(tmp) == UNSTRING_TRUE);

	test_assert(unstr_substr_char(tmp, str, 3) == UNSTRING_TRUE);
	check_unstr_char(tmp, ans[0]);

	str = "unkokkokussakusa";
	test_assert(unstr_substr_char(tmp, str, 7) == UNSTRING_TRUE);
	check_unstr_char(tmp, ans[1]);

	unstr_free(tmp);
}

static void test_unstr_strcat(void)
{
	char *ans[] = {
		"1234567890",
		"12345678901234567890",
	};
	unstr_t *tmp = unstr_init_memory(1);
	unstr_t *emp = unstr_init_memory(1);
	unstr_t *str = unstr_init(ans[0]);
	printf("unstr_strcat\n");
	test_assert(unstr_strcat(NULL, str) == UNSTRING_FALSE);
	test_assert(unstr_strcat(NULL, tmp) == UNSTRING_FALSE);
	test_assert(unstr_strcat(tmp, NULL) == UNSTRING_FALSE);
	test_assert(unstr_strcat(tmp, emp) == UNSTRING_FALSE);

	test_assert(unstr_strcat(tmp, str) == UNSTRING_TRUE);
	check_int(unstr_strlen(tmp), unstr_strlen(str));
	check_unstr(tmp, str);

	test_assert(unstr_strcat(tmp, str) == UNSTRING_TRUE);
	check_int(unstr_strlen(tmp), strlen(ans[1]));
	check_unstr_char(tmp, ans[1]);

	unstr_delete(3, str, tmp, emp);
}

static void test_unstr_strcat_char(void)
{
	char *ans[] = {
		"1234567890",
		"12345678901234567890",
	};
	unstr_t *tmp = unstr_init_memory(1);
	printf("unstr_strcat_char\n");
	test_assert(unstr_strcat_char(NULL, ans[0]) == UNSTRING_FALSE);
	test_assert(unstr_strcat_char(NULL, "") == UNSTRING_FALSE);
	test_assert(unstr_strcat_char(tmp, NULL) == UNSTRING_FALSE);
	test_assert(unstr_strcat_char(tmp, "") == UNSTRING_FALSE);

	test_assert(unstr_strcat_char(tmp, ans[0]) == UNSTRING_TRUE);
	check_int(unstr_strlen(tmp), strlen(ans[0]));
	check_unstr_char(tmp, ans[0]);

	test_assert(unstr_strcat_char(tmp, ans[0]) == UNSTRING_TRUE);
	check_int(unstr_strlen(tmp), strlen(ans[1]));
	check_unstr_char(tmp, ans[1]);

	unstr_free(tmp);
}

static void test_unstr_strcmp(void)
{
	unstr_t *str = unstr_init("1234567890");
	unstr_t *ans = unstr_copy(str);
	unstr_t *ans_fail = unstr_init("1234567891");
	unstr_t *emp = unstr_init_memory(1);
	printf("unstr_strcmp\n");
	test_assert(unstr_strcmp(NULL, str) == 0x100);
	test_assert(unstr_strcmp(NULL, emp) == 0x100);
	test_assert(unstr_strcmp(str, NULL) == 0x100);
	test_assert(unstr_strcmp(str, emp) == 0x100);
	test_assert(unstr_strcmp(emp, str) == 0x100);

	test_assert(unstr_strcmp(str, ans) == 0);
	test_assert(unstr_strcmp(str, ans_fail) < 0);
	test_assert(unstr_strcmp(ans_fail, str) > 0);

	unstr_delete(4, str, ans, ans_fail, emp);
}

static void test_unstr_strcmp_char(void)
{
	char *ans = "1234567890";
	char *ans_fail = "1234567891";
	unstr_t *str = unstr_init(ans);
	unstr_t *emp = unstr_init_memory(1);
	printf("unstr_strcmp_char\n");
	test_assert(unstr_strcmp_char(NULL, ans) == 0x100);
	test_assert(unstr_strcmp_char(NULL, "") == 0x100);
	test_assert(unstr_strcmp_char(str, NULL) == 0x100);
	test_assert(unstr_strcmp_char(str, "") == 0x100);
	test_assert(unstr_strcmp_char(emp, ans) == 0x100);

	test_assert(unstr_strcmp_char(str, ans) == 0);
	test_assert(unstr_strcmp_char(str, ans_fail) < 0);

	unstr_delete(2, str, emp);
}

static void test_unstr_strstr(void)
{
	char *ret = 0;
	char *ans1 = "4567890";
	char *ans2 = "7890";
	unstr_t *str = unstr_init("1234567890");
	unstr_t *s1 = unstr_init("45");
	unstr_t *s2 = unstr_init("7890");
	unstr_t *s3 = unstr_init("hoge");
	unstr_t *emp = unstr_init_memory(1);
	printf("unstr_strstr\n");
	check_null(unstr_strstr(NULL, str));
	check_null(unstr_strstr(NULL, emp));
	check_null(unstr_strstr(str, NULL));
	check_null(unstr_strstr(emp, NULL));
	check_null(unstr_strstr(str, emp));
	check_null(unstr_strstr(emp, str));
	check_null(unstr_strstr(str, s3));

	ret = unstr_strstr(str, s1);
	test_assert(ret != NULL);
	check_char(ret, ans1);

	ret = unstr_strstr(str, s2);
	test_assert(ret != NULL);
	check_char(ret, ans2);

	unstr_delete(5, str, s1, s2, s3, emp);
}

static void test_unstr_strstr_char(void)
{
	char *ret = 0;
	char *ans1 = "4567890";
	char *ans2 = "7890";
	char *s1 = "45";
	char *s2 = "7890";
	char *s3 = "hoge";
	unstr_t *str = unstr_init("1234567890");
	unstr_t *emp = unstr_init_memory(1);
	printf("unstr_strstr_char\n");
	check_null(unstr_strstr_char(NULL, s1));
	check_null(unstr_strstr_char(NULL, ""));
	check_null(unstr_strstr_char(str, NULL));
	check_null(unstr_strstr_char(emp, NULL));
	check_null(unstr_strstr_char(str, ""));
	check_null(unstr_strstr_char(emp, s1));
	check_null(unstr_strstr_char(str, s3));

	ret = unstr_strstr_char(str, s1);
	test_assert(ret != NULL);
	check_char(ret, ans1);

	ret = unstr_strstr_char(str, s2);
	test_assert(ret != NULL);
	check_char(ret, ans2);

	unstr_delete(2, str, emp);
}

static void test_unstr_explode(void)
{
	size_t i = 0;
	size_t len = 0;
	unstr_t *str = unstr_init("1 2 3 4 5 6 7 8 9 0 ");
	char *delim = " ";
	char *ans[11] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ""};
	unstr_t *emp = unstr_init_memory(1);
	unstr_t **ret = 0;
	printf("unstr_explode\n");
	check_null(unstr_explode(NULL, delim, &len));
	check_null(unstr_explode(emp, delim, &len));
	check_null(unstr_explode(str, NULL, &len));
	check_null(unstr_explode(str, "", &len));
	check_null(unstr_explode(str, delim, NULL));

	len = 1234;
	ret = unstr_explode(str, delim, &len);
	test_assert(len < 1234);
	for(i = 0; i < len; i++){
		check_unstr_char(ret[i], ans[i]);
		unstr_free(ret[i]);
	}
	unstr_delete(2, str, emp);
	free(ret);
}

static void test_unstr_sprintf(void)
{
	unstr_t *tmp = 0;
	unstr_t *p1 = 0;
	printf("unstr_sprintf\n");

	tmp = unstr_sprintf(NULL, NULL);
	test_assert(tmp == NULL);

	tmp = unstr_sprintf(NULL, "");
	test_assert(tmp != NULL);
	unstr_free(tmp);

	tmp = unstr_sprintf(NULL, "|%d|", 1234567890);
	check_unstr_char(tmp, "|1234567890|");

	unstr_sprintf(tmp, "%x|", 0x5aa5aa55);
	check_unstr_char(tmp, "5aa5aa55|");

	unstr_sprintf(tmp, "|%X", 0x55AA5AA5);
	check_unstr_char(tmp, "|55AA5AA5");

	unstr_sprintf(tmp, "%s%s", "AaA", "bBb");
	check_unstr_char(tmp, "AaAbBb");

	p1 = unstr_init("unko");
	unstr_sprintf(tmp, "%%$%$%%$", p1);
	check_unstr_char(tmp, "%$unko%$");
	unstr_free(p1);

	unstr_sprintf(tmp, "%d == %x == %X", 255, 0xff, 0xFF);
	check_unstr_char(tmp, "255 == ff == FF");

	p1 = unstr_init("456");
	unstr_sprintf(tmp, "%s->%$->%d", "123", p1, 789);
	check_unstr_char(tmp, "123->456->789");
	unstr_free(p1);

	unstr_free(tmp);
}

static void test_unstr_sscanf(void)
{
	size_t ret = 0;
	unstr_t *source1 = unstr_init("unko<>hoge<>fuga");
	unstr_t *source2 = unstr_init("<>unko<>hoge");
	unstr_t *source3 = unstr_init("unko<>hoge<>fuga<>");
	unstr_t *source4 = unstr_init("unko<><>fuga");
	unstr_t *source5 = unstr_init("unko$hoge$fuga");
	unstr_t *p1 = unstr_init_memory(1);
	unstr_t *p2 = unstr_init_memory(1);
	unstr_t *p3 = unstr_init_memory(1);
	printf("unstr_sscanf\n");

	ret = unstr_sscanf(NULL, "<>$<>", p1);
	check_int(ret, 0);
	ret = unstr_sscanf(source1, NULL, p1);
	check_int(ret, 0);
	ret = unstr_sscanf(source1, "unko<>", p1);
	check_int(ret, 0);

	ret = unstr_sscanf(source1, "$<>$<>$<>", p1, p2, p3);
	check_int(ret, 3);
	check_unstr_char(p1, "unko");
	check_unstr_char(p2, "hoge");
	check_unstr_char(p3, "fuga");

	ret = unstr_sscanf(source2, "$<>$<><>$", p1, p2, p3);
	check_int(ret, 2);
	test_assert(unstr_empty(p1) == UNSTRING_TRUE);
	check_unstr_char(p2, "unko<>hoge");

	ret = unstr_sscanf(source3, "$<>$<>$", p1, p2, p3);
	check_int(ret, 3);
	check_unstr_char(p1, "unko");
	check_unstr_char(p2, "hoge");
	check_unstr_char(p3, "fuga<>");

	ret = unstr_sscanf(source4, "$<>$<>$", p1, p2, p3);
	check_int(ret, 3);
	check_unstr_char(p1, "unko");
	test_assert(unstr_empty(p2) == UNSTRING_TRUE);
	check_unstr_char(p3, "fuga");

	ret = unstr_sscanf(source5, "$$$$$$", p1, p2, p3);
	check_int(ret, 3);
	check_unstr_char(p1, "unko");
	check_unstr_char(p2, "hoge");
	check_unstr_char(p3, "fuga");

	unstr_delete(8, source1, source2, source3, source4, source5, p1, p2, p3);
}

static void test_unstr_reverse(void)
{
	unstr_t *ret = 0;
	unstr_t *tmp = 0;
	unstr_t *emp = unstr_init_memory(1);
	unstr_t *source1 = unstr_init("1234567890");
	unstr_t *source2 = unstr_init("123456789");
	printf("unstr_reverse\n");

	ret = unstr_reverse(NULL);
	check_null(ret);
	unstr_free(ret);
	ret = unstr_reverse(emp);
	check_null(ret);
	unstr_free(ret);

	ret = unstr_reverse(source1);
	check_unstr_char(ret, "0987654321");
	unstr_free(ret);

	ret = unstr_reverse(source2);
	check_unstr_char(ret, "987654321");
	unstr_free(ret);

	ret = unstr_reverse(source1);
	tmp = unstr_reverse(ret);
	check_unstr(source1, tmp);
	unstr_free(ret);
	unstr_free(tmp);

	unstr_delete(5, source1, source2, ret, emp, tmp);
}

static void test_unstr_itoa(void)
{
	unstr_t *ret = 0;
	printf("unstr_itoa\n");

	ret = unstr_itoa(0, 1);
	check_null(ret);
	unstr_free(ret);
	ret = unstr_itoa(0, 37);
	check_null(ret);
	unstr_free(ret);

	ret = unstr_itoa(1234567890, 2);
	check_unstr_char(ret, "1001001100101100000001011010010");
	unstr_free(ret);

	ret = unstr_itoa(1234567890, 10);
	check_unstr_char(ret, "1234567890");
	unstr_free(ret);

	// マイナス対応は今のところ10進数のみ
	ret = unstr_itoa(-1234567890, 10);
	check_unstr_char(ret, "-1234567890");
	unstr_free(ret);

	ret = unstr_itoa(1234567890, 16);
	check_unstr_char(ret, "499602d2");
	unstr_free(ret);

	unstr_free(ret);
}

static void test_unstr_replace(void)
{
	unstr_t *ret = 0;
	unstr_t *emp = unstr_init_memory(1);
	unstr_t *data = unstr_init_memory(1);
	unstr_t *search = unstr_init_memory(1);
	unstr_t *replace = unstr_init_memory(1);
	printf("unstr_replace\n");

	unstr_strcpy_char(data, "unkokkokokkokokkokokekokko");
	unstr_strcpy_char(search, "ko");
	unstr_strcpy_char(replace, "unko");

	ret = unstr_replace(NULL, search, replace);
	check_null(ret);
	unstr_free(ret);
	ret = unstr_replace(emp, search, replace);
	check_null(ret);
	unstr_free(ret);
	ret = unstr_replace(data, NULL, replace);
	check_null(ret);
	unstr_free(ret);
	ret = unstr_replace(data, emp, replace);
	check_null(ret);
	unstr_free(ret);
	ret = unstr_replace(data, search, NULL);
	check_null(ret);
	unstr_free(ret);

	unstr_strcpy_char(data, "unkokkokokkokokkokokekokko");
	unstr_strcpy_char(search, "ko");
	unstr_strcpy_char(replace, "unko");

	ret = unstr_replace(data, search, emp);
	check_unstr_char(ret, "unkkkkek");
	unstr_free(ret);

	ret = unstr_replace(data, search, replace);
	check_unstr_char(ret, "ununkokunkounkokunkounkokunkounkokeunkokunko");
	unstr_free(ret);

	unstr_delete(5, ret, emp, data, search, replace);
}

static void test_unstr_strpos(void)
{
	unstr_t *emp = unstr_init_memory(1);
	unstr_t *text = unstr_init_memory(1);
	unstr_t *search = unstr_init_memory(1);
	printf("unstr_strpos\n");

	unstr_strcpy_char(text, "0123456789");
	unstr_strcpy_char(search, "34");

	test_assert(unstr_strpos(NULL, search) < 0);
	test_assert(unstr_strpos(emp, search) < 0);
	test_assert(unstr_strpos(text, NULL) < 0);
	test_assert(unstr_strpos(text, emp) < 0);

	unstr_strcpy_char(text, "0123456789");
	unstr_strcpy_char(search, "34");
	check_int(unstr_strpos(text, search), 3);

	unstr_strcpy_char(text, "000123456789");
	unstr_strcpy_char(search, "0");
	check_int(unstr_strpos(text, search), 0);

	unstr_strcpy_char(text, "0123456789");
	unstr_strcpy_char(search, "aaa");
	test_assert(unstr_strpos(text, search) < 0);

	unstr_delete(3, emp, text, search);
}

static void test_unstr_substr_count(void)
{
	unstr_t *emp = unstr_init_memory(1);
	unstr_t *text = unstr_init_memory(1);
	unstr_t *search = unstr_init_memory(1);
	printf("unstr_substr_count\n");

	unstr_strcpy_char(text, "unkokkokokkokokkokokekokko");
	unstr_strcpy_char(search, "ko");
	check_int(unstr_substr_count(NULL, search), 0);
	check_int(unstr_substr_count(emp, search), 0);
	check_int(unstr_substr_count(text, NULL), 0);
	check_int(unstr_substr_count(text, emp), 0);

	unstr_strcpy_char(text, "unkokkokokkokokkokokekokko");
	unstr_strcpy_char(search, "ko");
	check_int(unstr_substr_count(text, search), 9);

	unstr_delete(3, emp, text, search);
}

static void test_unstr_substr_count_char(void)
{
	unstr_t *emp = unstr_init_memory(1);
	unstr_t *text = unstr_init("unkokkokokkokokkokokekokko");
	char *search = "ko";
	printf("unstr_substr_count_char\n");

	check_int(unstr_substr_count_char(NULL, search), 0);
	check_int(unstr_substr_count_char(emp, search), 0);
	check_int(unstr_substr_count_char(text, NULL), 0);
	check_int(unstr_substr_count_char(text, ""), 0);
	check_int(unstr_substr_count_char(text, search), 9);

	unstr_delete(2, emp, text);
}

static void test_unstr_strtok(void)
{
	size_t index = 0;
	size_t i = 0;
	unstr_t *ret = 0;
	unstr_t *emp = unstr_init_memory(1);
	unstr_t *text = unstr_init("1<>2<>3<>4<>5<>6<>7<>8<>9<>0");
	char *search = "<>";
	char *ans[11] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ""};
	printf("unstr_strtok\n");

	ret = unstr_strtok(NULL, search, &index);
	check_null(ret);
	unstr_free(ret);
	ret = unstr_strtok(emp, search, &index);
	check_null(ret);
	unstr_free(ret);
	ret = unstr_strtok(text, NULL, &index);
	check_null(ret);
	unstr_free(ret);
	ret = unstr_strtok(text, "", &index);
	check_null(ret);
	unstr_free(ret);
	ret = unstr_strtok(text, search, NULL);
	check_null(ret);
	unstr_free(ret);

	i = 0;
	index = 0;
	while((ret = unstr_strtok(text, search, &index)) != NULL){
		check_unstr_char(ret, ans[i]);
		unstr_free(ret);
		i++;
	}
	check_int(i, 10);

	unstr_delete(3, ret, emp, text);
}

static void test_unstr_repeat(void)
{
	unstr_t *ret = 0;
	unstr_t *emp = unstr_init_memory(1);
	unstr_t *str = unstr_init("unko");
	printf("unstr_repeat\n");

	ret = unstr_repeat(NULL, 10);
	check_null(ret);
	unstr_free(ret);
	ret = unstr_repeat(emp, 10);
	check_null(ret);
	unstr_free(ret);
	ret = unstr_repeat(str, 0);
	check_null(ret);
	unstr_free(ret);

	ret = unstr_repeat(str, 5);
	check_unstr_char(ret, "unkounkounkounkounko");
	unstr_free(ret);

	unstr_delete(3, ret, emp, str);
}

static void test_unstr_repeat_char(void)
{
	unstr_t *ret = 0;
	char *str = "unko";
	printf("unstr_repeat_char\n");

	ret = unstr_repeat_char(NULL, 10);
	check_null(ret);
	unstr_free(ret);
	ret = unstr_repeat_char("", 10);
	check_null(ret);
	unstr_free(ret);
	ret = unstr_repeat_char(str, 0);
	check_null(ret);
	unstr_free(ret);

	ret = unstr_repeat_char(str, 5);
	check_unstr_char(ret, "unkounkounkounkounko");
	unstr_free(ret);

	unstr_free(ret);
}

