/*
 * @Author: taobo
 * @Date: 2020-11-29 15:52:19
 * @LastEditTime: 2020-11-29 17:27:58
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// static json_parse_type json_parse_null(json_context* c, json_value* v) {
//   EXPECT(c, 'n');
//   if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
//     return JSON_PARSE_INVALID_VALUE;
//   c->json += 3;
//   v->type = JSON_NULL;
//   return JSON_PARSE_OK;
// }

static json_parse_type json_parse_literal(json_context* c, json_value* v, const char* patt, json_type t) {
  if (t == JSON_NULL || t == JSON_TRUE) {
    EXPECT(c, patt[0]);
    if (c->json[0] != patt[1] || c->json[1] != patt[2] || c->json[2] != patt[3])
    return JSON_PARSE_INVALID_VALUE;
    c->json += 3;
    if (t == JSON_NULL) v->type = JSON_NULL;
    if (t == JSON_TRUE) v->type = JSON_TRUE;
  } else if (t == JSON_FALSE) {
    EXPECT(c, patt[0]);
    if (c->json[0] != patt[1] || c->json[1] != patt[2] || c->json[2] != patt[3] || c->json[3] != patt[4])
      return JSON_PARSE_INVALID_VALUE;
    c->json += 4;
    v->type = JSON_FALSE;
  }
  return JSON_PARSE_OK;
}

static json_parse_type json_parse_value(json_context* c, json_value* v) {
  switch (*c->json) {
    case 'n': return json_parse_literal(c, v, "null", JSON_NULL);
    case 't': return json_parse_literal(c, v, "true", JSON_TRUE);
    case 'f': return json_parse_literal(c, v, "false", JSON_FALSE);
    case '\0': return JSON_PARSE_EXPECT_VALUE;
    default: return JSON_PARSE_INVALID_VALUE;
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

