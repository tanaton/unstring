/*
 * unstring.h
 */
#ifndef UNSTRING_H_INCLUDE
#define UNSTRING_H_INCLUDE

#include <stdlib.h>

#define UNSTRING_HEAP_SIZE			(0x20)
#define UNSTRING_MEMORY_STAMP		(0x55)	/* ascii:[U] bin:01010101 */
#define unstr_free(str)				\
	do { unstr_free_func(str); (str) = NULL; } while(0)

typedef enum {
	UNSTRING_FALSE	= 0,
	UNSTRING_TRUE
} unstr_bool_t;

typedef struct unstr_st {
	char *data;
	size_t length;
	size_t heap;
} unstr_t;

extern unstr_t *unstr_alloc(unstr_t *str, size_t size);
extern unstr_t *unstr_init(const char *str);
extern unstr_t *unstr_init_memory(size_t size);
extern void unstr_free_func(unstr_t *str);
extern void unstr_delete(size_t size, ...);
extern void unstr_zero(unstr_t *str);
extern unstr_bool_t unstr_isset(const unstr_t *str);
extern unstr_bool_t unstr_empty(const unstr_t *str);
extern unstr_bool_t unstr_write(unstr_t *us, const char *bin, size_t offset, size_t len);
extern size_t unstr_strlen(const unstr_t *str);
extern unstr_t *unstr_copy(const unstr_t *str);
extern unstr_bool_t unstr_strcpy(unstr_t *s1, const unstr_t *s2);
extern unstr_bool_t unstr_strcpy_char(unstr_t *s1, const char *s2);
extern unstr_bool_t unstr_substr(unstr_t *s1, const unstr_t *s2, size_t len);
extern unstr_bool_t unstr_substr_char(unstr_t *data, const char *c, size_t len);
extern unstr_bool_t unstr_strcat(unstr_t *s1, const unstr_t *s2);
extern unstr_bool_t unstr_strcat_char(unstr_t *str, const char *c);
extern int unstr_strcmp(const unstr_t *s1, const unstr_t *s2);
extern int unstr_strcmp_char(const unstr_t *s1, const char *s2);
extern char *unstr_strstr(const unstr_t *s1, const unstr_t *s2);
extern char *unstr_strstr_char(const unstr_t *s1, const char *s2);
extern unstr_t **unstr_explode(const unstr_t *str, const char *tmp, size_t *len);
extern unstr_t *unstr_sprintf(unstr_t *str, const char *format, ...);
extern size_t unstr_sscanf(const unstr_t *data, const char *format, ...);
extern unstr_t *unstr_reverse(const unstr_t *str);
extern unstr_t *unstr_itoa(int num, size_t physics);
extern unstr_t *unstr_file_get_contents(const unstr_t *filename);
extern unstr_bool_t unstr_file_put_contents(const unstr_t *filename, const unstr_t *data, const char *mode);
extern unstr_t *unstr_replace(const unstr_t *data, const unstr_t *search, const unstr_t *replace);
extern int unstr_strpos(const unstr_t *text, const unstr_t *search);
extern size_t unstr_substr_count(const unstr_t *text, const unstr_t *search);
extern size_t unstr_substr_count_char(const unstr_t *text, const char *search);
extern unstr_t *unstr_strtok(const unstr_t *str, const char *delim, size_t *index);
extern unstr_t *unstr_repeat(const unstr_t *str, size_t count);
extern unstr_t *unstr_repeat_char(const char *str, size_t count);

#endif /* UNSTRING_H_INCLUDE */
