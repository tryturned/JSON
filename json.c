/*
 * @Author: taobo
 * @Date: 2020-11-29 15:52:19
 * @LastEditTime: 2020-12-03 19:53:06
 */
#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
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
    // if (*p && *p != '.') 
    //   return JSON_PARSE_ROOT_NOT_SINGULAR;
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

#define STRING_ERROR(ret) do { c->top = head; return ret; } while(0)

const char* json_parse_hex4(const char* p, unsigned* u) {
  unsigned ret = 0;
  for (int _ = 0; _ < 4; _++, p++) {
    ret <<= 4;
    if (ISDIGIT(*p)) {
      ret |= (*p - '0');
    } else if (ISALPHAATOF(*p)) { /* A~F or a~f */
      ret |= (tolower(*p) - 'a') + 10;
    } else {
      return NULL;
    }
  }
  *u = ret;
  return p;
}

static void json_encode_utf8(json_context* c,unsigned u) {
  if (u <= 0x7F) 
      PUTC(c, u & 0xFF);
  else if (u <= 0x7FF) {
      PUTC(c, 0xC0 | ((u >> 6) & 0xFF));
      PUTC(c, 0x80 | ( u       & 0x3F));
  }
  else if (u <= 0xFFFF) {
      PUTC(c, 0xE0 | ((u >> 12) & 0xFF));
      PUTC(c, 0x80 | ((u >>  6) & 0x3F));
      PUTC(c, 0x80 | ( u        & 0x3F));
  }
  else {
      assert(u <= 0x10FFFF);
      PUTC(c, 0xF0 | ((u >> 18) & 0xFF));
      PUTC(c, 0x80 | ((u >> 12) & 0x3F));
      PUTC(c, 0x80 | ((u >>  6) & 0x3F));
      PUTC(c, 0x80 | ( u        & 0x3F));
  }
}

static json_parse_type json_parse_string_raw(json_context* c, char** s, size_t* len) {
  EXPECT(c, '\"');
  size_t head = c->top;
  const char* p;
  p = c->json;
  while (True) {
      char ch = *p++;
      switch (ch) {
        case '\"':
          *len = c->top - head;
          *s = (char*)json_context_pop(c, *len);
          c->json = p;
          return JSON_PARSE_OK;
        case '\0':
          STRING_ERROR(JSON_PARSE_MISS_QUOTATION_MARK);
        case '\\':
          switch (*p++) {
            case '\"': PUTC(c, '\"'); break;
            case '\\': PUTC(c, '\\'); break;
            case '/':  PUTC(c, '/' ); break;
            case 'b':  PUTC(c, '\b'); break;
            case 'f':  PUTC(c, '\f'); break;
            case 'n':  PUTC(c, '\n'); break;
            case 'r':  PUTC(c, '\r'); break;
            case 't':  PUTC(c, '\t'); break;
            case 'u': ;
              unsigned u, u2;
              if (!(p = json_parse_hex4(p, &u)))
                STRING_ERROR(JSON_PARSE_INVALID_UNICODE_HEX);
              if (u >= 0xD800 && u <= 0xDBFF) {
                if (*p++ != '\\')
                  STRING_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE);
                if (*p++ != 'u')
                  STRING_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE);
                if (!(p = json_parse_hex4(p, &u2)))
                  STRING_ERROR(JSON_PARSE_INVALID_UNICODE_HEX);
                if (u2 < 0xDC00 || u2 > 0xDFFF)
                  STRING_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE);
                u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
              }
              json_encode_utf8(c, u);
              break;
          
            default:
            STRING_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE);
          }
          break;
        default:
          if ((unsigned char)ch < 0x20) {
            STRING_ERROR(JSON_PARSE_INVALID_STRING_CHAR);
          }
          PUTC(c, ch);
      }
  }  
}

static json_parse_type json_parse_string(json_context* c, json_value* v) {
  int ret;
  char* s;
  size_t len;
  if ((ret = json_parse_string_raw(c, &s, &len)) == JSON_PARSE_OK)
    json_set_string(v, s, len);
  return ret;
}

static json_parse_type json_parse_value(json_context* c, json_value* v);
static json_parse_type json_parse_array(json_context* c, json_value* v) {
  EXPECT(c, '[');
  size_t size = 0;
  json_parse_type ret;
  json_parse_whitespace(c);
  if (*c->json == ']') {
    c->json++;
    v->type = JSON_ARRAY;
    v->size = 0;
    v->e = NULL;
    return JSON_PARSE_OK;
  }
  while (True) {
    json_value e;
    json_init(&e);
    if ((ret = json_parse_value(c, &e)) != JSON_PARSE_OK)
      break;
    memcpy(json_context_push(c, sizeof(json_value)), &e, sizeof(json_value));
    size++;
    json_parse_whitespace(c);
    if (*c->json == ',') {
      c->json++;
      json_parse_whitespace(c);
    }
    else if (*c->json == ']') {
      c->json++;
      v->type = JSON_ARRAY;
      v->size = size;
      size *= sizeof(json_value);
      memcpy(v->e = (json_value*)malloc(size), json_context_pop(c, size), size);
      return JSON_PARSE_OK;
    } else {
      ret = JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
      break;
    }
  }
  for (size_t _ = 0; _ < size; _++) {
    json_free((json_value*)json_context_pop(c, sizeof(json_value)));
  }
  return ret;
}

static json_parse_type json_parse_value(json_context* c, json_value* v) {
  switch (*c->json) {
    case 'n':  return json_parse_literal(c, v, "null", JSON_NULL);
    case 't':  return json_parse_literal(c, v, "true", JSON_TRUE);
    case 'f':  return json_parse_literal(c, v, "false", JSON_FALSE);
    case '"':  return json_parse_string(c, v);
    case '[':  return json_parse_array(c, v);
    default:   return json_parse_number(c, v);
    case '\0': return JSON_PARSE_EXPECT_VALUE;
  }
  return JSON_PARSE_INVALID_VALUE;
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
  size_t index;
  switch (v->type) {
    case JSON_STRING:
      free(v->s);
      break;
    case JSON_ARRAY:
      for (index = 0; index < v->size; index++) {
        json_free(&v->e[index]);
      }
      free(v->e);
      break;
    default: break;
  }
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

size_t json_get_array_size(const json_value* v) {
  assert(v != NULL && v->type == JSON_ARRAY);
  return v->size;
}

json_value* json_get_array_element(const json_value* v, size_t index) {
  assert(v != NULL && v->type == JSON_ARRAY);
  assert(index < v->size);
  return &v->e[index];
}

void json_set_array(json_value* v, json_value* array) {
  json_free(v);
  v->e = (json_value*)malloc(sizeof(json_value));
  v->size = 1;
  memcpy(v->e, array, sizeof(json_value));
  v->type = JSON_ARRAY;
}
// ...... json objects ......
size_t json_get_object_size(const json_value* v) {
  assert(v != NULL && v->type == JSON_OBJECT);
  return v->o_size;
}
const char* json_get_object_key(const json_value* v, size_t index) {
  assert(v != NULL && v->type == JSON_OBJECT);
  assert(v->o_size > index);
  return v->m[index].key;
}
size_t json_get_object_key_length(const json_value* v, size_t index) {
  assert(v != NULL && v->type == JSON_OBJECT);
  assert(v->o_size > index);
  return v->m[index].klen;
}
json_value* json_get_object_value(const json_value* v, size_t index) {
  assert(v != NULL && v->type == JSON_OBJECT);
  assert(v->o_size > index);
  return &v->m[index].value;
}