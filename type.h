/*
 * @Author: taobo
 * @Date: 2020-11-29 14:59:10
 * @LastEditTime: 2020-12-03 23:00:56
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

typedef struct json_value json_value;
typedef struct json_member json_member;

// JSON 树的节点
struct json_value {
  union {
    // 仅当 json_type==JSON_NUMBER 时 n 有效
    double n;
    // 仅当 json_type==JSON_STRING 时 s,len 有效
    struct {
      char* s;
      size_t len;
    };
    // 仅当 json_type==JSON_ARRAY 时 e,size 有效
    struct {
      json_value* e;
      size_t size;
    };
    // 仅当 json_type==JSON_OBJECT 时 key,value,klen 有效
    struct {
      json_member* m;
      size_t o_size;
    };
  };
  json_type type;
};

struct json_member {
  char* key;
  size_t klen;
  json_value value;
};

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
  JSON_PARSE_MISS_QUOTATION_MARK,
  // 转义字符 \ 后出现了无效字符
  JSON_PARSE_INVALID_STRING_ESCAPE,
  // 出现了不合法的字符，即 ascii code 位于 [0x00, 0x1F] 的字符
  JSON_PARSE_INVALID_STRING_CHAR,
  // Unicode parse error
  JSON_PARSE_INVALID_UNICODE_HEX,
  // Unicode surrogate pair error
  JSON_PARSE_INVALID_UNICODE_SURROGATE,
  // Array miss comma or bracket
  JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET,
  // OBJECT miss key
  JSON_PARSE_MISS_KEY,
  // OBJECT miss colon
  JSON_PARSE_MISS_COLON,
  // OBJECT miss comma or bracket
  JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET
} json_parse_type;

typedef struct {
  const char* json;
  char* stack;
  size_t size, top;
} json_context;

#endif