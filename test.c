/*
 * @Author: taobo
 * @Date: 2020-11-29 15:29:38
 * @LastEditTime: 2020-11-29 17:08:37
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format)\
    do {\
        test_count++;\
        if (equality)\
          test_pass++;\
        else {\
          fprintf(stderr, "%d lines: expect: " format " actual: " format "\n", __LINE__, expect, actual);\
          main_ret = 1;\
        }\
    } while (0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

static void test_parse_null() {
  json_value v;
  v.type = JSON_TRUE;
  EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "null"));
  EXPECT_EQ_INT(JSON_PARSE_ROOT_NOT_SINGULAR, json_parse(&v, "  null   is"));
  EXPECT_EQ_INT(JSON_NULL, json_get_type(&v));
}

static void test_parse_true() {
  json_value v;
  v.type = JSON_FALSE;
  EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "true"));
  EXPECT_EQ_INT(JSON_TRUE, json_get_type(&v));
}

static void test_parse_false() {
  json_value v;
  v.type = JSON_TRUE;
  EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "false"));
  EXPECT_EQ_INT(JSON_FALSE, json_get_type(&v));
}

static void test_parse() {
  test_parse_null();
  test_parse_true();
  test_parse_false();
}

int main() {
  test_parse();
  printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
  return main_ret;
}