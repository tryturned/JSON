/*
 * @Author: taobo
 * @Date: 2020-11-29 14:59:10
 * @LastEditTime: 2020-12-02 12:25:21
 */
#ifndef JSON_TYPE_H__
#define JSON_TYPE_H__

#define False 0
#define True 1
// JSON 数据类型
typedef enum {
  JSON_NULL,
  JSON_TRUE,
  JSON_FALSE,
  JSON_NUMBER,
  JSON_STRING,
  JSON_ARRAY,
  JSON_OBJECT
} json_type;

// JSON 树的节点
typedef struct {
  union {
    // 仅当 json_type==JSON_NUMBER 时 n 有效
    double n;
    // 仅当 json_type==JSON_STRING 时 s,len 有效
    struct {
      char* s;
      size_t len;
    };
  };
  json_type type;
} json_value;

// json_parse 状态返回类型
typedef enum {
  JSON_PARSE_OK = 0,
  // 若一个 JSON 只含有空白
  JSON_PARSE_EXPECT_VALUE,
  // 若值不是语法子集中的字面值
  JSON_PARSE_INVALID_VALUE,
  // 若一个值之后，在空白之后还有其他字符
  JSON_PARSE_ROOT_NOT_SINGULAR,
  // 溢出
  JSON_PARSE_NUMBER_TOO_BIG,
  // string 类型丢失了 "
  LEPT_PARSE_MISS_QUOTATION_MARK,
  // 转义字符 \ 后出现了无效字符
  JSON_PARSE_INVALID_STRING_ESCAPE,
  // 出现了不合法的字符，即 ascii code 位于 [0x00, 0x1F] 的字符
  JSON_PARSE_INVALID_STRING_CHAR,
  // Unicode parse error
  JSON_PARSE_INVALID_UNICODE_HEX
} json_parse_type;

typedef struct {
  const char* json;
  char* stack;
  size_t size, top;
} json_context;

#endif