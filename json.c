/*
 * @Author: taobo
 * @Date: 2020-11-29 15:52:19
 * @LastEditTime: 2020-11-30 13:39:40
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#include "json.h"

#define EXPECT(c, ch) do {\
      assert(*c->json == (ch));\
      c->json++;\
    }\
    while (0)

static void json_parse_whitespace(json_context* c) {
  const char *p = c->json;
  while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
    p++;
  c->json = p;
}

static json_parse_type json_parse_literal(json_context* c, json_value* v, const char* patt, json_type t) {
  size_t index;
  EXPECT(c, patt[0]);
  for (index = 0; patt[index + 1]; index++) {
    if (c->json[index] != patt[index + 1])
      return JSON_PARSE_INVALID_VALUE;
  }
  c->json += index;
  v->type = t;
  return JSON_PARSE_OK;
}

static json_parse_type json_parse_number(json_context* c, json_value* v) {
  // TODO validate number
  const char* p = c->json;
  if (*p == '-') p++;
  if (*p == '0') {
    p++;
    if (*p && *p != '.') 
      return JSON_PARSE_ROOT_NOT_SINGULAR;
  } else {
      if (!ISDIGIT1TO9(*p)) return JSON_PARSE_INVALID_VALUE;
      for (p++; ISDIGIT(*p); p++);
  }
  if (*p == '.') {
      p++;
      if (!ISDIGIT(*p)) return JSON_PARSE_INVALID_VALUE;
      for (p++; ISDIGIT(*p); p++);
  }
  if (*p == 'e' || *p == 'E') {
      p++;
      if (*p == '+' || *p == '-') p++;
      if (!ISDIGIT(*p)) return JSON_PARSE_INVALID_VALUE;
      for (p++; ISDIGIT(*p); p++);
  }
  // translate
  errno = 0;
  v->n = strtod(c->json, NULL);
  if (errno == ERANGE && (v->n == HUGE_VAL || v->n == -HUGE_VAL))
    return JSON_PARSE_NUMBER_TOO_BIG;
  c->json = p;
  v->type = JSON_NUMBER;
  return JSON_PARSE_OK;
}

static json_parse_type json_parse_value(json_context* c, json_value* v) {
  switch (*c->json) {
    case 'n': return json_parse_literal(c, v, "null", JSON_NULL);
    case 't': return json_parse_literal(c, v, "true", JSON_TRUE);
    case 'f': return json_parse_literal(c, v, "false", JSON_FALSE);
    default:  return json_parse_number(c, v);
    case '\0': return JSON_PARSE_EXPECT_VALUE;
  }
}

json_parse_type json_parse(json_value* v, const char* json) {
  assert(v != NULL);
  json_context c;
  c.json = json;
  v->type = JSON_NULL;
  json_parse_whitespace(&c);
  json_parse_type res;
  if ((res = json_parse_value(&c, v)) == JSON_PARSE_OK) {
    json_parse_whitespace(&c);
    if (*c.json != '\0') 
      return JSON_PARSE_ROOT_NOT_SINGULAR;
  }
  return res;
}

json_type json_get_type(const json_value* v) {
  assert(v != NULL);
  return v->type;
}

double json_get_number(const json_value* v) {
  assert(v != NULL && v->type == JSON_NUMBER);
  return v->n;
}

void json_set_number(json_value* v, double n) {
  assert(v != NULL && v->type == JSON_NUMBER);
  v->n = n;
}

int json_get_boolean(const json_value* v) {
  assert(v != NULL && (v->type == JSON_TRUE || v->type == JSON_FALSE));
  return v->type;
}

void json_set_boolean(json_value* v, int b) {
  assert(v != NULL && (b == JSON_TRUE || b == JSON_FALSE));
  v->type = b;
}

void json_set_string(json_value* v, const char* s, size_t len);
void json_free(json_value* v);
