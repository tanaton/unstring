#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "unstring.h"

static void *unstr_malloc(size_t size);
static void *unstr_realloc(void *p, size_t size, size_t len);
static unstr_bool_t unstr_check_heap_size(const unstr_t *str, size_t size);

/**
 * @brief		メモリを確保し領域をしるしで埋める。
 * @param[in]	size	確保する領域のサイズ
 * @return		確保した領域へのポインタ
 */
static void *unstr_malloc(size_t size)
{
	void *p = malloc(size);
	if(p == NULL){
		/* 領域の確保に失敗した場合、perrorを呼び出す。 */
		perror("unstr_malloc:");
	} else {
		memset(p, UNSTRING_MEMORY_STAMP, size);
	}
	return p;
}

/**
 * @brief		領域を拡張し、新しい領域をしるしで埋める。
 * @param[in]	p		領域へのポインタ
 * @param[in]	size	新しい領域のサイズ
 * @param[in]	len		元々使用していた領域のサイズ
 * @return		拡張した領域へのポインタ
 */
static void *unstr_realloc(void *p, size_t size, size_t len)
{
	p = realloc(p, size);
	if(p == NULL){
		/* 領域の確保に失敗した場合、perrorを呼び出す。 */
		perror("unstr_realloc:");
	} else {
		len++;
		if(size > len){
			memset(((char *)p) + len, UNSTRING_MEMORY_STAMP, size - len);
		}
	}
	return p;
}

/**
 * @brief		文字列を拡張する際に領域の確保が必要か計算する
 * @param[in]	str		計算を行う文字列
 * @param[in]	size	拡張する文字列の長さ
 * @return		判定結果
 * @return		UNSTRING_TRUE	必要
 * @return		UNSTRING_FALSE	不要
 */
static unstr_bool_t unstr_check_heap_size(const unstr_t *str, size_t size)
{
	return (((str->length + size) >= str->heap) ? UNSTRING_TRUE : UNSTRING_FALSE);
}

/**
 * @brief		文字列のバッファを拡張する
 * @param[in]	str		拡張対象
 * @param[in]	size	増加させる量
 * @return		無し
 * @public
 */
unstr_t *unstr_alloc(unstr_t *str, size_t size)
{
	if(str == NULL){
		str = unstr_malloc(sizeof(unstr_t));
		str->length = 0;
		str->heap = 0;
		str->data = NULL;
	}
	/* 頻繁に確保すると良くないらしいので大まかに確保して
	 * 確保する回数を減らす。
	 */
	str->heap += size + (str->heap >> 1);
	str->data = unstr_realloc(str->data, str->heap, str->length);
	return str;
}

/**
 * @brief		引数で渡された文字列をunstr_tで初期化する
 * @param[in]	str		文字列
 * @return		unstr_tに変換された文字列
 * @public
 */
unstr_t *unstr_init(const char *str)
{
	size_t size = 0;
	unstr_t *data = 0;
	if(str == NULL) return NULL;
	size = strlen(str);
	data = unstr_alloc(NULL, size + 1);
	memcpy(data->data, str, size);
	data->data[size] = '\0';
	data->length = size;
	return data;
}

/**
 * @brief		文字列の領域だけ確保して初期化する。
 * @param[in]	size	確保するunstr_t型のバッファサイズ
 * @return		空のunstr_t型
 * @public
 */
unstr_t *unstr_init_memory(size_t size)
{
	unstr_t *data = 0;
	if(size == 0) return NULL;
	data = unstr_alloc(NULL, size);
	/* 長さ0の文字列扱い */
	data->data[0] = '\0';
	data->length = 0;
	return data;
}

/**
 * @brief		文字列(unstr_t)を開放する
 * @param[in]	str		開放するunstr_t型
 * @return		無し
 * @public
 */
void unstr_free_func(unstr_t *str)
{
	if(str != NULL){
		free(str->data);
		str->data = NULL;
		str->length = 0;
		str->heap = 0;
	}
	free(str);
}

/**
 * @brief		引数で渡された文字列を開放する
 * @param[in]	size	開放する文字列(unstr_t)の数
 * @param[in]	...		開放する文字列(unstr_t)
 * @return		無し
 * @public
 */
void unstr_delete(size_t size, ...)
{
	unstr_t *str = 0;
	va_list list;
	va_start(list, size);
	while(size--){
		str = va_arg(list, unstr_t *);
		unstr_free(str);
	}
	va_end(list);
}

/**
 * @brief			空文字列で初期化する。領域は開放しない。
 * @param[in,out]	str		初期化する文字列
 * @return			無し
 * @public
 */
void unstr_zero(unstr_t *str)
{
	if(str != NULL){
		if(str->data != NULL){
			str->data[0] = '\0';
		}
		str->length = 0;
	}
}

/**
 * @brief		文字列用の領域が確保されていることを確認する。
 * @param[in]	str		対象文字列
 * @return		確認結果
 * @return		UNSTRING_TRUE	確保済み
 * @return		UNSTRING_FALSE	確保なし
 * @public
 */
unstr_bool_t unstr_isset(const unstr_t *str)
{
	unstr_bool_t ret = UNSTRING_FALSE;
	if((str != NULL) && (str->data != NULL)){
		ret = UNSTRING_TRUE;
	} else {
		ret = UNSTRING_FALSE;
	}
	return ret;
}

/**
 * @brief		文字列が空またはNULLであるか確認する。
 * @param[in]	str		対象文字列
 * @return		確認結果
 * @return		UNSTRING_TRUE	空
 * @return		UNSTRING_FALSE	空ではない
 * @public
 */
unstr_bool_t unstr_empty(const unstr_t *str)
{
	unstr_bool_t ret = UNSTRING_FALSE;
	if((str == NULL) || (str->data == NULL) || (str->length <= 0)){
		ret = UNSTRING_TRUE;
	} else {
		ret = UNSTRING_FALSE;
	}
	return ret;
}

/**
 * @brief			unstr_t型にバイナリをコピーする
 * @param[in,out]	us		コピー先
 * @param[in]		s		コピー元
 * @param[in]		offset	コピー先の開始位置
 * @param[in]		len		コピーする長さ
 * @return			コピー結果
 * @return			UNSTRING_TRUE	成功
 * @return			UNSTRING_FALSE	失敗
 * @public
 * @par				詳細:
 * lenが0だった場合は失敗にしない。
 */
unstr_bool_t unstr_write(unstr_t *us, const char *bin, size_t offset, size_t len)
{
	size_t size = len + offset;
	if(!unstr_isset(us) || (bin == NULL)){
		return UNSTRING_FALSE;
	}
	if(unstr_check_heap_size(us, size + 1)){
		unstr_alloc(us, size + 1);
	}
	memcpy(&(us->data[offset]), bin, len);
	us->length = size;
	us->data[us->length] = '\0';
	return UNSTRING_TRUE;
}

/**
 * @brief		文字列の長さを返す
 * @param[in]	str		対象文字列
 * @return		文字列の長さ
 * @public
 */
size_t unstr_strlen(const unstr_t *str)
{
	return unstr_isset(str) ? str->length : 0;
}

/**
 * @brief		文字列のコピーを返す。
 * @param[in]	str		コピー元
 * @return		コピーした文字列
 * @public
 */
unstr_t *unstr_copy(const unstr_t *str)
{
	unstr_t *data = 0;
	if(unstr_isset(str)){
		data = unstr_init_memory(str->length + 2);
		unstr_strcat(data, str);
	}
	return data;
}

/**
 * @brief			文字列をコピーする。
 * @param[in,out]	s1		コピー先
 * @param[in]		s2		コピー元
 * @return			コピー結果
 * @return			UNSTRING_TRUE	成功
 * @return			UNSTRING_FALSE	失敗
 * @public
 * @par				詳細:
 * s2に空文字列だった場合、s1が空文字列になります。
 */
unstr_bool_t unstr_strcpy(unstr_t *s1, const unstr_t *s2)
{
	if(!unstr_isset(s2)){
		return UNSTRING_FALSE;
	}
	return unstr_write(s1, s2->data, 0, s2->length);
}

/**
 * @brief			unstr_t文字列にchar文字列をコピーする。
 * @param[in,out]	s1		コピー先
 * @param[in]		s2		コピー元
 * @return			コピー結果
 * @return			UNSTRING_TRUE	成功
 * @return			UNSTRING_FALSE	失敗
 * @public
 */
unstr_bool_t unstr_strcpy_char(unstr_t *s1, const char *s2)
{
	unstr_bool_t ret = UNSTRING_FALSE;
	unstr_t *str = 0;
	if(s2 != NULL){
		str = unstr_init(s2);
		ret = unstr_strcpy(s1, str);
		unstr_free(str);
	}
	return ret;
}

/**
 * @brief			文字列を切り出してコピーする
 * @param[in,out]	s1		コピー先
 * @param[in]		s2		コピー元
 * @param[in]		size	コピーする長さ
 * @return			コピー結果
 * @return			UNSTRING_TRUE	成功
 * @return			UNSTRING_FALSE	失敗
 * @public
 */
unstr_bool_t unstr_substr(unstr_t *s1, const unstr_t *s2, size_t len)
{
	if(!unstr_isset(s1) || unstr_empty(s2)){
		return UNSTRING_FALSE;
	}
	if(s2->length < len){
		len = s2->length;
	}
	return unstr_write(s1, s2->data, 0, len);
}

/**
 * @brief		char文字列を切り出してunstr_t文字列に変換する
 * @param[out]	str		コピー先文字列
 * @param[in]	c		対象文字列
 * @param[in]	size	コピーする長さ
 * @return		変換結果
 * @return		UNSTRING_TRUE	成功
 * @return		UNSTRING_FALSE	失敗
 * @public
 */
unstr_bool_t unstr_substr_char(unstr_t *str, const char *c, size_t len)
{
	unstr_t *data = 0;
	unstr_bool_t ret = UNSTRING_FALSE;
	if(c != NULL){
		data = unstr_init(c);
		ret = unstr_substr(str, data, len);
		unstr_free(data);
	}
	return ret;
}

/**
 * @brief			文字列を結合する
 * @param[in,out]	s1		結合先文字列
 * @param[in]		s2		結合文字列
 * @return			変換結果
 * @return			UNSTRING_TRUE	成功
 * @return			UNSTRING_FALSE	失敗
 * @public
 * @par				詳細:
 * s1に変化が無い場合、UNSTRING_FALSEを返します。
 */
unstr_bool_t unstr_strcat(unstr_t *s1, const unstr_t *s2)
{
	if(!unstr_isset(s1) || unstr_empty(s2)){
		return UNSTRING_FALSE;
	}
	return unstr_write(s1, s2->data, s1->length, s2->length);
}

/**
 * @brief			unstr_t文字列にchar文字列を結合する
 * @param[in,out]	str		結合先文字列
 * @param[in]		c		結合文字列
 * @return			結合結果
 * @return			UNSTRING_TRUE	成功
 * @return			UNSTRING_FALSE	失敗
 * @public
 */
unstr_bool_t unstr_strcat_char(unstr_t *str, const char *c)
{
	unstr_t *data = 0;
	unstr_bool_t ret = UNSTRING_FALSE;
	if(c != NULL){
		data = unstr_init(c);
		ret = unstr_strcat(str, data);
		unstr_free(data);
	}
	return ret;
}

/**
 * @brief		文字列を比較する
 * @param[in]	s1		比較文字列1
 * @param[in]	s2		比較文字列2
 * @return		比較結果
 * @return		0			同じ
 * @return		0x100		エラー
 * @return		上記以外	違う(文字コードの差分)
 * @public
 */
int unstr_strcmp(const unstr_t *s1, const unstr_t *s2)
{
	int ret = 0x100;
	if(unstr_isset(s1) && unstr_isset(s2)){
		if(s1->length == s2->length){
			ret = memcmp(s1->data, s2->data, s1->length);
		}
	}
	return ret;
}

/**
 * @brief		char文字列とunstr_t文字列を比較する
 * @param[in]	s1		比較文字列1
 * @param[in]	s2		比較文字列2
 * @return		比較結果
 * @return		0			同じ
 * @return		0x100		エラー
 * @return		上記以外	違う(文字コードの差分)
 * @public
 */
int unstr_strcmp_char(const unstr_t *s1, const char *s2)
{
	int ret = 0x100;
	unstr_t *str = 0;
	if(s2 != NULL){
		str = unstr_init(s2);
		ret = unstr_strcmp(s1, str);
		unstr_free(str);
	}
	return ret;
}

/**
 * @brief		文字列を検索する
 * @param[in]	s1		比較文字列1
 * @param[in]	s2		比較文字列2
 * @return		比較結果
 * @return		アドレス	発見
 * @return		NULL		エラー
 * @public
 */
char* unstr_strstr(const unstr_t *s1, const unstr_t *s2)
{
	int pos = unstr_strpos(s1, s2);
	if(pos >= 0){
		return s1->data + pos;
	}
	return NULL;
}

/**
 * @brief		unstr_t文字列をchar文字列で検索する
 * @param[in]	s1		比較文字列1
 * @param[in]	s2		比較文字列2
 * @return		比較結果
 * @return		アドレス	発見
 * @return		NULL		エラー
 * @public
 */
char* unstr_strstr_char(const unstr_t *s1, const char *s2)
{
	char *ret = 0;
	unstr_t *str = 0;
	if(s2 != NULL){
		str = unstr_init(s2);
		ret = unstr_strstr(s1, str);
		unstr_free(str);
	}
	return ret;
}

/**
 * @brief		対象文字列を区切り文字で切り、格納先に格納する。
 * @param[in]	str		対象文字列
 * @param[in]	delim	区切り文字列
 * @param[out]	len		配列の長さ
 * @return		unstr_tの配列
 * @public
 */
unstr_t **unstr_explode(const unstr_t *str, const char *delim, size_t *len)
{
	unstr_t *s = 0;
	unstr_t **ret = 0;
	size_t index = 0;
	size_t size = 0;
	size_t heap = 0;
	if(unstr_empty(str)
	|| (delim == NULL)
	|| (strlen(delim) == 0)
	|| (len == NULL)){
		return NULL;
	}
	while((s = unstr_strtok(str, delim, &index)) != NULL){
		if(size >= heap){
			heap = ((heap << 1) + 8);
			ret = unstr_realloc(ret, heap * sizeof(unstr_t *), size * sizeof(unstr_t *));
		}
		ret[size] = s;
		size++;
	}
	*len = size;
	return ret;
}

/**
 * @brief			自動拡張機能付きsprintf。細かいフォーマットには未対応。
 * @param[in,out]	str		格納先
 * @param[in]		format	フォーマット
 * @param[in]		...		可変引数
 * @return			unstr_t文字列
 * @public
 * @par			詳細:
 * formatが空文字列の場合を許容する
 */
unstr_t *unstr_sprintf(unstr_t *str, const char *format, ...)
{
	va_list list;
	unstr_t *unsp = 0;
	char *sp = 0;
	int ip = 0;
	size_t i = 0;
	if(format == NULL){
		return NULL;
	}
	va_start(list, format);
	if(unstr_isset(str)){
		unstr_zero(str);
	} else {
		unstr_free(str);
		str = unstr_init_memory(UNSTRING_HEAP_SIZE);
	}
	while((sp = strchr(format, '%')) != NULL){
		i = sp - format;
		if(i > 0){
			unsp = unstr_init_memory(i + 2);
			unstr_substr_char(unsp, format, i);
			unstr_strcat(str, unsp);
			unstr_free(unsp);
			format += i;
		}
		if((*(format + 1)) == '\0'){
			break;
		}
		format++;
		switch(*format){
		case 's':
			sp = va_arg(list, char *);
			unstr_strcat_char(str, sp);
			break;
		case '$':
			unsp = va_arg(list, unstr_t*);
			unstr_strcat(str, unsp);
			break;
		case 'd':
			ip = va_arg(list, int);
			unsp = unstr_itoa(ip, 10);
			unstr_strcat(str, unsp);
			unstr_free(unsp);
			break;
		case 'x':
			ip = va_arg(list, int);
			unsp = unstr_itoa(ip, 16);
			unstr_strcat(str, unsp);
			unstr_free(unsp);
			break;
		case 'X':
			ip = va_arg(list, int);
			unsp = unstr_itoa(ip, 16);
			for(i = 0; i < unsp->length; i++){
				unsp->data[i] = toupper(unsp->data[i]);
			}
			unstr_strcat(str, unsp);
			unstr_free(unsp);
			break;
		case '%':
			unstr_strcat_char(str, "%");
			break;
		default:
			format--;
			break;
		}
		format++;
	}
	unstr_strcat_char(str, format);
	va_end(list);
	return str;
}

/**
 * @brief			文字列を反転させる。
 * @param[in,out]	str		対象文字列
 * @return			反転結果
 * @return			UNSTRING_TRUE	成功
 * @return			UNSTRING_FALSE	失敗
 * @public
 */
unstr_t *unstr_reverse(const unstr_t *str)
{
	size_t length = 0;
	size_t size = 0;
	size_t count = 0;
	int c = 0;
	unstr_t *ret = 0;
	if(unstr_empty(str)) return NULL;
	ret = unstr_copy(str);
	length = ret->length - 1;
	size = ret->length / 2;
	while(count < size){
		c = ret->data[count];
		ret->data[count++] = ret->data[length];
		ret->data[length--] = c;
	}
	return ret;
}

/**
 * @brief		数値から文字列を作成する。
 * @param[in]	num		対象数値
 * @param[in]	physics	基数
 * @return		unstr_t文字列
 * @public
 * @par			詳細:
 * 10進数以外のマイナス値には対応していない。
 */
unstr_t *unstr_itoa(int num, size_t physics)
{
	int value = 0;
	int c = 0;
	unstr_bool_t flag = UNSTRING_FALSE;
	size_t number = 0;
	unstr_t *str = 0;
	unstr_t *ret = 0;
	if((physics < 2) || (physics > 36)){
		return NULL;
	}
	str = unstr_init_memory(UNSTRING_HEAP_SIZE);
	if(num == 0){
		str->data[str->length++] = '0';
	} else if(num < 0){
		if(physics == 10){
			num *= -1;
		}
		flag = UNSTRING_TRUE;
	}
	number = num;
	while(number > 0){
		value = number % physics;
		number = number / physics;
		if(value >= 10){
			c = (value - 10) + 'a';
		} else {
			c = value + '0';
		}
		if(unstr_check_heap_size(str, 1)){
			unstr_alloc(str, 1);
		}
		str->data[str->length++] = c;
	}
	if(physics == 10 && flag){
		if(unstr_check_heap_size(str, 1)){
			unstr_alloc(str, 1);
		}
		str->data[str->length++] = '-';
	}
	str->data[str->length] = '\0';
	ret = unstr_reverse(str);
	unstr_free(str);
	return ret;
}

/**
 * @brief		頭の良いsubstr
 * @param[in]	data	対象文字列
 * @param[in]	format	切り出すフォーマット
 * @param[out]	...		領域が確保されたunstr_t型
 * @return		切り出した文字数
 * @public
 *
 * @par			詳細:
 * dataからformat通りに文字列を切り出し、unstr_t型に格納する。\n
 * 対応フォーマットは「$」のみ\n
 * "abcdefg"から"cd"と"fg"を抜き出したい場合、"ab$e$"と書く\n
 * 「$$」と書けば「$」を区切り文字に出来る\n
 * 取得した文字列の数を返す。上記の場合は「2」を返す
 */
size_t unstr_sscanf(const unstr_t *data, const char *format, ...)
{
	va_list list;
	size_t count = 0;
	size_t steady = 0;
	unstr_t *str = 0;
	unstr_t *search = 0;
	char *tmp = 0;
	char *index = 0;
	char *format_end = 0;

	if(!unstr_isset(data) || (format == NULL)){
		return 0;
	}
	tmp = data->data;
	/* 先頭を探索する */
	format_end = strchr(format, '$');
	if(format_end == NULL) return 0;
	search = unstr_init_memory(UNSTRING_HEAP_SIZE);
	unstr_substr_char(search, format, format_end - format);
	va_start(list, format);
	format += format_end - format;
	/* search->dataが長さ0の文字列の場合、tmpを返す */
	index = strstr(tmp, search->data);
	if(index == NULL){
		tmp += strlen(tmp);
		format += strlen(format);
	} else {
		tmp = index + search->length;
	}
	while((format_end = strchr(format, '$')) != NULL){
		format++;
		/* 検索文字列を構築 */
		format_end = strchr(format, '$');
		if(*format == '\0'){
			steady = strlen(tmp);
			format += strlen(format);
		} else {
			if(format_end == NULL){
				format_end = (char *)format + strlen(format);
			}
			unstr_substr_char(search, format, format_end - format);
			if(unstr_strlen(search) == 0){
				unstr_strcat_char(search, "$");
			}
			/* 検索 */
			index = strstr(tmp, search->data);
			if(index == NULL){
				steady = strlen(tmp);
				format += strlen(format);
			} else {
				/* 差分を数値化 */
				steady = (size_t)(index - tmp);
				format += format_end - format;
			}
		}
		str = va_arg(list, unstr_t *);
		if(unstr_substr_char(str, tmp, steady) == UNSTRING_TRUE){
			count++;
		}
		/* ポインタを進める */
		tmp += steady + search->length;
	}
	va_end(list);
	unstr_free(search);
	return count;
}

/**
 * @brief		ファイルを丸ごと読み込む
 * @param[in]	filename	ファイルパス
 * @return		読み込んだファイルの中身
 * @public
 */
unstr_t *unstr_file_get_contents(const unstr_t *filename)
{
	FILE *fp = fopen(filename->data, "r");
	unstr_t *str = 0;
	long size = 0;
	size_t getsize = 0;

	if(fp == NULL) return NULL;
	/* ファイルサイズを求める */
	/* ファイルポインタを最後まで移動 */
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	/* ファイルポインタを先頭に戻す */
	rewind(fp);

	/* ファイルサイズ分の領域を確保 */
	str = unstr_init_memory((size_t)size + 1);

	/* 読み込み */
	getsize = fread(str->data, 1, (size_t)size, fp);
	str->data[getsize] = '\0';
	str->length = getsize;

	/* ファイルポインタをクローズ */
	fclose(fp);
	return str;
}

/**
 * @brief		ファイルに書き出す
 * @param[in]	filename	ファイルパス
 * @param[in]	data		書き込む内容
 * @param[in]	mode		書き込みモード
 * @return		出力結果
 * @return		UNSTRING_TRUE	成功
 * @return		UNSTRING_FALSE	失敗
 * @public
 */
unstr_bool_t unstr_file_put_contents(const unstr_t *filename, const unstr_t *data, const char *mode)
{
	FILE *fp = 0;
	if(unstr_empty(data)) return UNSTRING_FALSE;
	fp = fopen(filename->data, mode);
	if(fp == NULL) return UNSTRING_FALSE;
	/* ftell(fp); */
	fwrite(data->data, 1, data->length, fp);
	fclose(fp);
	return UNSTRING_TRUE;
}

/**
 * @brief		文字列を置換する。非破壊。
 * @param[in]	data	対象文字列
 * @param[in]	search	置換対象文字列
 * @param[in]	replace	置換文字列
 * @return		対象文字列から置換対象文字列を置換文字列に置換した文字列
 * @public
 */
unstr_t *unstr_replace(const unstr_t *data, const unstr_t *search, const unstr_t *replace)
{
	unstr_t *str = 0;
	size_t size = 0;
	char *pt = 0;
	char *index = 0;

	if(unstr_empty(data) || unstr_empty(search) || !unstr_isset(replace)){
		return NULL;
	}
	pt = data->data;
	str = unstr_init_memory(data->length);
	while((index = strstr(pt, search->data)) != NULL){
		size = (size_t)(index - pt);
		if(unstr_check_heap_size(str, size + replace->length)){
			unstr_alloc(str, size + replace->length);
		}
		memcpy(&(str->data[str->length]), pt, size);
		str->length += size;
		memcpy(&(str->data[str->length]), replace->data, replace->length);
		str->length += replace->length;
		pt = index + search->length;
	}
	if(*pt != '\0'){
		size = strlen(pt);
		if(unstr_check_heap_size(str, size)){
			unstr_alloc(str, size);
		}
		memcpy(&(str->data[str->length]), pt, size);
		str->length += size;
	}
	str->data[str->length] = '\0';
	return str;
}

/**
 * @brief		文字列を検索し、発見した文字位置を返す
 * @param[in]	text	対象文字列
 * @param[in]	search	検索文字列
 * @return		文字位置
 * @public
 */
int unstr_strpos(const unstr_t *text, const unstr_t *search)
{
	unsigned char *x = 0;
	size_t m = 0;
	unsigned char *y = 0;
	size_t n = 0;
	size_t i = 0;
	size_t table[256] = {0};

	if(unstr_empty(text) || unstr_empty(search)){
		return -1;
	}
	x = (unsigned char *)search->data;
	m = unstr_strlen(search);
	y = (unsigned char *)text->data;
	n = unstr_strlen(text);

	// クイックサーチ
	for(i = 0; i < 256; i++){
		table[i] = m + 1;
	}
	for(i = 0; i < m; i++){
		table[x[i]] = m - i;
	}

	for(i = 0; i <= n - m;){
		if(memcmp(x, y + i, m) == 0){
			return (int)i;
		}
		i += table[y[i + m]];
	}
	return -1;
}

/**
 * @brief		出現数をカウント
 * @param[in]	text	対象文字列
 * @param[in]	search	検索文字列
 * @return		検索文字列の出現数
 * @public
 */
size_t unstr_substr_count(const unstr_t *text, const unstr_t *search)
{
	unsigned char *x = 0;
	size_t m = 0;
	unsigned char *y = 0;
	size_t n = 0;
	size_t count = 0;
	size_t i = 0;
	size_t table[256] = {0};

	if(unstr_empty(text) || unstr_empty(search)){
		return 0;
	}
	x = (unsigned char *)search->data;
	m = unstr_strlen(search);
	y = (unsigned char *)text->data;
	n = unstr_strlen(text);

	// クイックサーチ
	for(i = 0; i < 256; i++){
		table[i] = m + 1;
	}
	for(i = 0; i < m; i++){
		table[x[i]] = m - i;
	}

	for(i = 0; i <= n - m;){
		if(memcmp(x, y + i, m) == 0){
			count++;
		}
		i += table[y[i + m]];
	}
	return count;
}

/**
 * @brief		出現数をカウント
 * @param[in]	text	対象文字列
 * @param[in]	search	検索文字列
 * @return		検索文字列の出現数
 * @public
 */
size_t unstr_substr_count_char(const unstr_t *text, const char *search)
{
	size_t count = 0;
	unstr_t *str = 0;
	if(search != NULL){
		str = unstr_init(search);
		count = unstr_substr_count(text, str);
		unstr_free(str);
	}
	return count;
}

/**
 * @brief			文字列をトークンで切り分ける
 * @param[in,out]	str		対象文字列。
 * @param[in]		delim	トークン(文字列可)
 * @param[in,out]	index	インデックス値。次回呼び出し時に必要
 * @return			切り出した文字列
 * @public
 *
 * @par				詳細:
 * delimで指定された「文字列」でstrを切り分けます。
 */
unstr_t *unstr_strtok(const unstr_t *str, const char *delim, size_t *index)
{
	unstr_t *data = 0;
	char *p = 0;
	char *ptr = 0;
	size_t len = 0;
	size_t slen = unstr_strlen(str);
	if(unstr_empty(str)
	|| (delim == NULL)
	|| (strlen(delim) == 0)
	|| (index == NULL)
	|| (*index > slen)){
		return NULL;
	}
	data = unstr_init_memory(UNSTRING_HEAP_SIZE);
	ptr = str->data + (*index);
	p = strstr(ptr, delim);
	if(p != NULL){
		len = (size_t)(p - ptr);
		unstr_substr_char(data, ptr, len);
		*index += len + strlen(delim);
	} else {
		unstr_substr_char(data, ptr, slen - (*index));
		*index = slen + 1;
	}
	return data;
}

/**
 * @brief		繰り返し文字列を生成する
 * @param[in]	str		繰り返す文字列
 * @param[in]	count	繰り返す回数
 * @return		生成した文字列
 * @public
 */
unstr_t *unstr_repeat(const unstr_t *str, size_t count)
{
	unstr_t *data = 0;
	size_t i = 0;
	if(unstr_empty(str) || (count == 0)){
		return NULL;
	}
	data = unstr_init_memory((unstr_strlen(str) * count) + 2);
	for(i = 0; i < count; i++){
		unstr_strcat(data, str);
	}
	return data;
}

/**
 * @brief		繰り返し文字列を生成する
 * @param[in]	str		繰り返す文字列
 * @param[in]	count	繰り返す回数
 * @return		生成した文字列
 * @public
 */
unstr_t *unstr_repeat_char(const char *str, size_t count)
{
	unstr_t *data = 0;
	unstr_t *ret = 0;
	if(str != NULL){
		data = unstr_init(str);
		ret = unstr_repeat(data, count);
		unstr_free(data);
	}
	return ret;
}

