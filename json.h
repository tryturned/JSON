/*
 * @Author: taobo
 * @Date: 2020-11-29 14:42:22
 * @LastEditTime: 2020-11-30 14:48:27
 */
#ifndef JSON_JSON_H__
#define JSON_JSON_H__
#include <assert.h>
#include "type.h"

#define ISDIGIT(ch) ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch) ((ch) >= '1' && (ch) <= '9')

#define json_init(v) do { (v)->type = JSON_NULL; } while (0)
#define json_set_null(v) json_free(v)

// parse json string
json_parse_type json_parse(json_value* v, const char* json);

void json_free(json_value* v);

json_type json_get_type(const json_value* v);

int json_get_boolean(const json_value* v);
void json_set_boolean(json_value* v, int b);

double json_get_number(const json_value* v);
void json_set_number(json_value* v, double n);

void json_set_string(json_value* v, const char* s, size_t len);
const char* json_get_string(const json_value* v);
size_t json_get_string_length(const json_value* v);
#endif
