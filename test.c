/*
 * @Author: taobo
 * @Date: 2020-11-29 15:29:38
 * @LastEditTime: 2020-12-01 13:38:58
 */
#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
# include "test.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

// 测试null
static void test_parse_null() {
  json_value v;
  json_init(&v);
  json_set_boolean(&v, 0);
  EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "null"));
  EXPECT_EQ_INT(JSON_NULL, json_get_type(&v));
  json_free(&v);
}

// 测试true
static void test_parse_true() {
  json_value v;
  json_init(&v);
  json_set_boolean(&v, 0);
  EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "true"));
  EXPECT_EQ_INT(JSON_TRUE, json_get_type(&v));
  json_free(&v);
}

// 测试false
static void test_parse_false() {
  json_value v;
  json_init(&v);
  json_set_boolean(&v, 1);
  EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "false"));
  EXPECT_EQ_INT(JSON_FALSE, json_get_type(&v));
  json_free(&v);
}

// 测试number
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

  TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
  TEST_NUMBER( 4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
  TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
  TEST_NUMBER( 2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
  TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
  TEST_NUMBER( 2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
  TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
  TEST_NUMBER( 1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
  TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}

// 测试error
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
}

static void test_parse_root_not_singular() {
  TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "null x");

  TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0123");
  TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0x123");
  TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0x0");
}

static void test_parse_number_too_big() {
  TEST_ERROR(JSON_PARSE_NUMBER_TOO_BIG, "1e309");
  TEST_ERROR(JSON_PARSE_NUMBER_TOO_BIG, "-1e309");
}

static void test_access_string() {
  json_value v;
  json_init(&v);
  json_set_string(&v, "", 0);
  EXPECT_EQ_STRING("", json_get_string(&v), json_get_string_length(&v));
  json_set_string(&v, "Hello", 5);
  EXPECT_EQ_STRING("Hello", json_get_string(&v), json_get_string_length(&v));
  json_free(&v);
}
static void test_access_null() {
  json_value v;
  json_init(&v);
  json_set_string(&v, "a", 1);
  json_set_null(&v);
  EXPECT_EQ_INT(JSON_NULL, json_get_type(&v));
  json_free(&v);
}
static void test_access_boolean() {
  json_value v;
  json_init(&v);
  json_set_string(&v, "a", 1);
  json_set_boolean(&v, 1);
  EXPECT_TRUE(json_get_boolean(&v));
  json_set_boolean(&v, 0);
  EXPECT_FALSE(json_get_boolean(&v));
  json_free(&v);
}
static void test_access_number() {
  json_value v;
  json_init(&v);
  json_set_string(&v, "a", 1);
  json_set_number(&v, 1234.5);
  EXPECT_EQ_DOUBLE(1234.5, json_get_number(&v));
  json_free(&v);
}

static void test_parse_string() {
    TEST_STRING("", "\"\"");
    TEST_STRING("Hello", "\"Hello\"");
    TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
    TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
}

static void test_parse_invalid_string_escape() {
  TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\g\"");
  TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\'\"");
  TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\0\"");
  TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\x12\"");
}

static void test_parse_invalid_string_char() {
    TEST_ERROR(JSON_PARSE_INVALID_STRING_CHAR, "\"\x01\"");
    TEST_ERROR(JSON_PARSE_INVALID_STRING_CHAR, "\"\x1F\"");
}
// 𝄞
static void test_parse() {
  test_parse_expect_value();
  test_parse_invalid_value();
  test_parse_root_not_singular();
  test_parse_null();
  test_parse_true();
  test_parse_false();
  test_parse_number();
  test_parse_number_too_big();
  test_parse_string();
  test_parse_invalid_string_escape();
  test_parse_invalid_string_char();


  // naive unit test
  test_access_null();
  test_access_boolean();
  test_access_number();
  test_access_string();
}

int main() {
  #ifdef _WINDOWS
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  #endif
  test_parse();
  printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
  return main_ret;
}