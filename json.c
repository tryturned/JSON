/*
 * @Author: taobo
 * @Date: 2020-11-29 15:52:19
 * @LastEditTime: 2020-11-30 20:13:54
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#include "json.h"

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

static void* json_context_push(json_context* c, size_t size) {
  assert(size > 0);
  void* ret;
  if (c->top + size >= c->size) {
    if (c->size == 0)
      c->size = JSON_PARSE_STACK_INIT_SIZE;
    while (c->top + size >= c->size)
      c->size += c->size >> 1;
    c->stack = (char*)realloc(c->stack, c->size);
  }
  ret = c->stack + c->top;
  c->top += size;
  return ret;
}

static void* json_context_pop(json_context* c, size_t size) {
  assert(c->top >= size);
  return c->stack + (c->top -= size);
}

#define PUTC(c, ch) do { *(char*)json_context_push(c, sizeof(char)) = (ch); } while(0)

static json_parse_type json_parse_string(json_context* c, json_value* v) {
  EXPECT(c, '\"');
  size_t head = c->top, len;
  const char* p;
  p = c->json;
  while (True) {
      char ch = *p++;
      switch (ch) {
          case '\"':
              len = c->top - head;
              json_set_string(v, (const char*)json_context_pop(c, len), len);
              c->json = p;
              return JSON_PARSE_OK;
          case '\0':
              c->top = head;
              return LEPT_PARSE_MISS_QUOTATION_MARK;
          default:
              PUTC(c, ch);
      }
  }  
}

static json_parse_type json_parse_value(json_context* c, json_value* v) {
  switch (*c->json) {
    case 'n':  return json_parse_literal(c, v, "null", JSON_NULL);
    case 't':  return json_parse_literal(c, v, "true", JSON_TRUE);
    case 'f':  return json_parse_literal(c, v, "false", JSON_FALSE);
    case '"':  return json_parse_string(c, v);
    default:   return json_parse_number(c, v);
    case '\0': return JSON_PARSE_EXPECT_VALUE;
  }
}

json_parse_type json_parse(json_value* v, const char* json) {
  assert(v != NULL);
  json_context c;
  json_parse_type res;
  c.json = json;
  c.stack = NULL;
  c.size = c.top = 0;
  json_init(v);
  json_parse_whitespace(&c);
  if ((res = json_parse_value(&c, v)) == JSON_PARSE_OK) {
    json_parse_whitespace(&c);
    if (*c.json != '\0') {
      v->type = JSON_NULL; 
      return JSON_PARSE_ROOT_NOT_SINGULAR;
    }
  }
  assert(c.top == 0);
  free(c.stack);
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
  json_free(v);
  v->n = n;
  v->type = JSON_NUMBER;
}

int json_get_boolean(const json_value* v) {
  assert(v != NULL && (v->type == JSON_TRUE || v->type == JSON_FALSE));
  return v->type == JSON_TRUE;
}

void json_set_boolean(json_value* v, int b) {
  json_free(v);
  v->type = b ? JSON_TRUE : JSON_FALSE;
}

void json_free(json_value* v) {
  assert(v != NULL);
  if (v->type == JSON_STRING)
    free(v->s);
  v->type = JSON_NULL;
}

void json_set_string(json_value* v, const char* s, size_t len) {
  assert(v != NULL && (s != NULL || len == 0));
  json_free(v);
  v->s = (char*)malloc(len + 1);
  memmove(v->s, s, len);
  v->s[len] = '\0';
  v->len = len;
  v->type = JSON_STRING;
}

const char* json_get_string(const json_value* v) {
  assert(v != NULL && v->type == JSON_STRING);
  return v->s;
}

size_t json_get_string_length(const json_value* v) {
  assert(v != NULL && v->type == JSON_STRING);
  return v->len;
}
