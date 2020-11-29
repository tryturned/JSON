/*
 * @Author: taobo
 * @Date: 2020-11-29 14:59:10
 * @LastEditTime: 2020-11-29 21:33:29
 */
#ifndef JSON_TYPE_H__
#define JSON_TYPE_H__

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
  // 仅当 json_type==JSON_NUMBER 时 n 有效
  double n;
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
  JSON_PARSE_NUMBER_TOO_BIG
} json_parse_type;

typedef struct {
  const char* json;
} json_context;

#endif