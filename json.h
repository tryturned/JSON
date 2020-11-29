/*
 * @Author: taobo
 * @Date: 2020-11-29 14:42:22
 * @LastEditTime: 2020-11-29 21:01:28
 */
#ifndef JSON_JSON_H__
#define JSON_JSON_H__

#include "type.h"
#include <assert.h>

// parse json string
json_parse_type json_parse(json_value* v, const char* json);

json_type json_get_type(const json_value* v);

double json_get_number(const json_value* v);
#endif
