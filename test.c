/*
 * @Author: taobo
 * @Date: 2020-11-29 15:29:38
 * @LastEditTime: 2020-11-29 21:09:05
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
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g")

// 测试null
static void test_parse_null() {
  json_value v;
  v.type = JSON_TRUE;
  EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "null"));
  EXPECT_EQ_INT(JSON_NULL, json_get_type(&v));
}

// 测试true
static void test_parse_true() {
  json_value v;
  v.type = JSON_FALSE;
  EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "true"));
  EXPECT_EQ_INT(JSON_TRUE, json_get_type(&v));
}

// 测试false
static void test_parse_false() {
  json_value v;
  v.type = JSON_TRUE;
  EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "false"));
  EXPECT_EQ_INT(JSON_FALSE, json_get_type(&v));
}

// 测试number
#define TEST_NUMBER(expect, json)\
    do {\
        json_value v;\
        EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, json));\
        EXPECT_EQ_INT(JSON_NUMBER, json_get_type(&v));\
        EXPECT_EQ_DOUBLE(expect, json_get_number(&v));\
    } while(0)

static void test_parse_number() {
  TEST_NUMBER(0.0, "0");
  TEST_NUMBER(0.0, "-0");
  TEST_NUMBER(0.0, "-0.0");
  TEST_NUMBER(1.0, "1");
  TEST_NUMBER(-1.0, "-1");
  TEST_NUMBER(1.5, "1.5");
  TEST_NUMBER(-1.5, "-1.5");
  TEST_NUMBER(3.1416, "3.1416");
  TEST_NUMBER(1E10, "1E10");
  TEST_NUMBER(1e10, "1e10");
  TEST_NUMBER(1E+10, "1E+10");
  TEST_NUMBER(1E-10, "1E-10");
  TEST_NUMBER(-1E10, "-1E10");
  TEST_NUMBER(-1e10, "-1e10");
  TEST_NUMBER(-1E+10, "-1E+10");
  TEST_NUMBER(-1E-10, "-1E-10");
  TEST_NUMBER(1.234E+10, "1.234E+10");
  TEST_NUMBER(1.234E-10, "1.234E-10");
  TEST_NUMBER(0.0, "1e-10000"); /* must underflow */
}


// 测试error
#define TEST_ERROR(error, json)\
    do {\
      json_value v;\
      v.type = JSON_FALSE;\
      EXPECT_EQ_INT(error, json_parse(&v, json));\
      EXPECT_EQ_INT(JSON_NULL, json_get_type(&v));\
    } while (0)

static void test_parse_expect_value() {
  TEST_ERROR(JSON_PARSE_EXPECT_VALUE, "");
  TEST_ERROR(JSON_PARSE_EXPECT_VALUE, " ");
}

static void test_parse_invalid_value() {
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "nul");
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "?");

  /* invalid number */
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "+0");
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "+1");
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, ".123"); /* at least one digit before '.' */
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "1.");   /* at least one digit after '.' */
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "INF");
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "inf");
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "NAN");
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "nan");
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "0123");
}

static void test_parse_root_not_singular() {
  TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "null x");
}

static void test_parse() {
  test_parse_expect_value();
  test_parse_invalid_value();
  test_parse_root_not_singular();
  test_parse_null();
  test_parse_true();
  test_parse_false();
  test_parse_number();
}

int main() {
  test_parse();
  printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
  return main_ret;
}