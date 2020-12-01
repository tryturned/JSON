<!--
 * @Author: taobo
 * @Date: 2020-11-29 14:23:33
 * @LastEditTime: 2020-12-01 12:54:21
-->
# JSON 库
写出一个支持跨平台使用的 C JSON 库, 并经过了详细的单元测试。  
* [JSON 库基础](#JSON-库基础)  
* [JSON 库需求](#JSON-库需求)
* [JSON 库测试](#JSON-库测试)
  * [处理异常情况](#处理异常情况)
  * [null](#null)
  * [true](#true)
  * [false](#false)
  * [number](#number)
  * [string](#string)
  * [array](#array)
  * [object](#object)
* [JSON 库特点](#JSON-库特点)
* [JSON 库技能](#JSON-库技能)
## JSON 库基础
[JSON 库基础与语法子集介绍](./JSON.md)
## JSON 库需求
- 可以将 JSON 文本解析为一个树状数据结构（parse）
- 可以将基础数据结构转换成 JSON 文本（stringify）
- 提供接口访问 JSON 文本解析后的数据结构（access）
## JSON 库测试
### `处理异常情况`
```c
  // 测试空 JSON 文本
  TEST_ERROR(JSON_PARSE_EXPECT_VALUE, "");
  TEST_ERROR(JSON_PARSE_EXPECT_VALUE, " ");
  // 测试不符合 JSON 标准规定 6 类数据结构语法的文本串
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "+0");
  // at least one digit before '.' 
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, ".123"); 
  TEST_ERROR(JSON_PARSE_INVALID_VALUE, "NAN");
  // null 值解析后多了个字符 'x'
  TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "null x");
```
### `null` 
```c
  EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "null"));
```
### `true`
```c
  EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "true"));
```
### `false`
```c
  EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "false"));
```
### `number`
```c
  TEST_NUMBER(0.0, "-0.0");
  TEST_NUMBER(1.0, "1");
  // must underflow
  TEST_NUMBER(0.0, "1e-10000"); 
  // Max double
  TEST_NUMBER( 1.7976931348623157e+308, "1.7976931348623157e+308");
  TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
  // test_parse_number_too_big
  TEST_ERROR(JSON_PARSE_NUMBER_TOO_BIG, "1e309");
```
### `string`
```c
  // normal string
  TEST_STRING("", "\"\"");
  TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
  TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
  // test_parse_invalid_string_escape
  TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\g\"");
  TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\x12\"");
  // test_parse_invalid_string_char
  TEST_ERROR(JSON_PARSE_INVALID_STRING_CHAR, "\"\x01\"");
```
### `array`
```c
// TODO
```
### `object`
```c
// TODO
```

## JSON 库特点
- 符合标准的 JSON 解析器和生成器
- 手写的递归下降解析器（recursive descent parser）
- 使用标准 C 语言
- 跨平台／编译器（如 Windows／Linux／OS X，vc／gcc／clang）
- 仅支持 UTF-8 JSON 文本
- 仅支持以 double 存储 JSON number 类型
## JSON 库技能
- 测试驱动开发（test driven development, TDD）
- C 语言编程风格
- 数据结构
- API 设计
- 断言
- Unicode
- 浮点数
- Github、CMake、valgrind、Doxygen 等工具


