/*
 * @Author: taobo
 * @Date: 2020-11-30 14:32:29
 * @LastEditTime: 2020-11-30 15:11:11
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

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
#define EXPECT_EQ_STRING(expect, actual, alength)\
    EXPECT_EQ_BASE(sizeof(expect) - 1 == alength && memcmp(expect, actual, alength) == 0, expect, actual, "%s")
#define EXPECT_TRUE(actual) EXPECT_EQ_BASE((actual) != 0, "true", "false", "%s")
#define EXPECT_FALSE(actual) EXPECT_EQ_BASE((actual) == 0, "false", "true", "%s")

#define TEST_NUMBER(expect, json)\
    do {\
        json_value v;\
        json_init(&v);\
        EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, json));\
        EXPECT_EQ_INT(JSON_NUMBER, json_get_type(&v));\
        EXPECT_EQ_DOUBLE(expect, json_get_number(&v));\
        json_free(&v);\
    } while(0)
    
#define TEST_ERROR(error, json)\
    do {\
      json_value v;\
      json_init(&v);\
      json_set_boolean(&v, 0);\
      EXPECT_EQ_INT(error, json_parse(&v, json));\
      EXPECT_EQ_INT(JSON_NULL, json_get_type(&v));\
      json_free(&v);\
    } while (0)
    