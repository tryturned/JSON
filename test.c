/*
 * @Author: taobo
 * @Date: 2020-11-29 15:29:38
 * @LastEditTime: 2020-12-03 23:26:57
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

  // invalid number 
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "+0");
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "+1");
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, ".123"); /* at least one digit before '.' */
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "1.");   /* at least one digit after '.' */
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "INF");
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "inf");
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "NAN");
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "nan");

  // invalid array
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "[1,]");
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "[0, nul]");
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
    // \uXXXX test
    TEST_STRING("Hello\0World", "\"Hello\\u0000World\"");
    TEST_STRING("\x24", "\"\\u0024\"");
    TEST_STRING("\xC2\xA2", "\"\\u00A2\""); 
    TEST_STRING("\xE2\x82\xAC", "\"\\u20AC\"");
    TEST_STRING("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\"");
    TEST_STRING("\xF0\x9D\x84\x9E", "\"\\ud834\\udd1e\"");
}

static void test_parse_invalid_string_escape() {
  TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\'\"");
  TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\0\"");
  TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\x12\"");
}

static void test_parse_invalid_string_char() {
  TEST_ERROR(JSON_PARSE_INVALID_STRING_CHAR, "\"\x01\"");
  TEST_ERROR(JSON_PARSE_INVALID_STRING_CHAR, "\"\x1F\"");
}

static void test_parse_missing_quotation_mark() {
  TEST_ERROR(JSON_PARSE_MISS_QUOTATION_MARK, "\"fd");
}

static void test_parse_invalid_unicode_hex() {
  TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"ab\\u91gg\"");
  TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"ab\\u\"");
  TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"ab\\u02\"");
  TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"ab\\u0/02\"");
  TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"ab\\u0  2\"");
}

static void test_parse_invalid_unicode_surrogate() {
  TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\"");
  TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uDBFF\"");
  TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\\\\"");
  TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD811\\u0032\"");
}

static void test_parse_array() {
    json_value v;
    json_init(&v);
    EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "[ ]"));
    EXPECT_EQ_INT(JSON_ARRAY, json_get_type(&v));
    EXPECT_EQ_SIZE_T(0, json_get_array_size(&v));
    json_free(&v);
    json_init(&v);
    EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "[ null , false , true , 123 , \"abc\" ]"));
    EXPECT_EQ_INT(JSON_NULL,   json_get_type(json_get_array_element(&v, 0)));
    EXPECT_EQ_INT(JSON_FALSE,  json_get_type(json_get_array_element(&v, 1)));
    EXPECT_EQ_INT(JSON_TRUE,   json_get_type(json_get_array_element(&v, 2)));
    EXPECT_EQ_INT(JSON_NUMBER, json_get_type(json_get_array_element(&v, 3)));
    EXPECT_EQ_INT(JSON_STRING, json_get_type(json_get_array_element(&v, 4)));
    EXPECT_EQ_DOUBLE(123.0, json_get_number(json_get_array_element(&v, 3)));
    EXPECT_EQ_STRING("abc", json_get_string(json_get_array_element(&v, 4)), json_get_string_length(json_get_array_element(&v, 4)));
    json_free(&v);
    json_init(&v);
    EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ]]"));
    json_free(&v);
}
static void test_access_array() {
  json_value v, v2;
  json_init(&v);
  json_init(&v2);
  json_set_string(&v, "a", 1);
  json_set_array(&v2, &v);
  EXPECT_EQ_INT(JSON_ARRAY, json_get_type(&v2));
  json_free(&v2);
}

static void test_parse_miss_comma_or_square_bracket() {
  TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1");
  TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1}");
  TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1 2");
  TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[[]");
  TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1, 2 4]");
}

static void test_parse_miss_key() {
  TEST_ERROR(JSON_PARSE_MISS_KEY, "{:1,");
  TEST_ERROR(JSON_PARSE_MISS_KEY, "{1:1,");
  TEST_ERROR(JSON_PARSE_MISS_KEY, "{true:1,");
  TEST_ERROR(JSON_PARSE_MISS_KEY, "{false:1,");
  TEST_ERROR(JSON_PARSE_MISS_KEY, "{null:1,");
  TEST_ERROR(JSON_PARSE_MISS_KEY, "{[]:1,");
  TEST_ERROR(JSON_PARSE_MISS_KEY, "{{}:1,");
  TEST_ERROR(JSON_PARSE_MISS_KEY, "{\"a\":1,");
}

static void test_parse_miss_colon() {
  TEST_ERROR(JSON_PARSE_MISS_COLON, "{\"a\"}");
  TEST_ERROR(JSON_PARSE_MISS_COLON, "{\"a\",\"b\"}");
}

static void test_parse_miss_comma_or_curly_bracket() {
  TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":1");
  TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":1]");
  TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":1 \"b\"");
  TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":{}");
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
  test_parse_missing_quotation_mark();
  test_parse_invalid_unicode_hex();
  test_parse_invalid_unicode_surrogate();
  test_parse_array();
  test_parse_miss_comma_or_square_bracket();
  test_parse_miss_key();
  test_parse_miss_colon();
  test_parse_miss_comma_or_curly_bracket();

  // naive unit test
  test_access_null();
  test_access_boolean();
  test_access_number();
  test_access_string();
  test_access_array();
}

int main() {
  #ifdef _WINDOWS
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  #endif
  test_parse();
  printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
  return main_ret;
}