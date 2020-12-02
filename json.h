/*
 * @Author: taobo
 * @Date: 2020-11-29 14:42:22
 * @LastEditTime: 2020-12-02 23:22:23
 */
#ifndef JSON_JSON_H__
#define JSON_JSON_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include "type.h"

#ifndef JSON_PARSE_STACK_INIT_SIZE
#define JSON_PARSE_STACK_INIT_SIZE 256
#endif

#define ISALPHAATOF(ch)  tolower((ch)) >= 'a' && tolower((ch)) <= 'f'
#define ISDIGIT(ch) ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch) ((ch) >= '1' && (ch) <= '9')

#define json_init(v) do { (v)->type = JSON_NULL; } while (0)
#define json_set_null(v) json_free(v)

#define EXPECT(c, ch) do {\
      assert(*c->json == (ch));\
      c->json++;\
    }\
    while (0)
// parse json string
json_parse_type json_parse(json_value* v, const char* json);

void json_free(json_value* v);

json_type json_get_type(const json_value* v);

int json_get_boolean(const json_value* v);
void json_set_boolean(json_value* v, int b);

double json_get_number(const json_value* v);
void json_set_number(json_value* v, double n);

#define PUTC(c, ch) do { *(char*)json_context_push(c, sizeof(char)) = (ch); } while(0)
void json_set_string(json_value* v, const char* s, size_t len);
const char* json_get_string(const json_value* v);
size_t json_get_string_length(const json_value* v);

size_t json_get_array_size(const json_value* v);
json_value* json_get_array_element(const json_value* v, size_t index);
void json_set_array(json_value* v, json_value* array);
#endif
