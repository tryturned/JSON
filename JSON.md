<!--
 * @Author: taobo
 * @Date: 2020-11-29 14:26:26
 * @LastEditTime: 2020-11-29 15:10:44
-->
# JSON 基础
1. [JSON 数据类型](#1-json-数据类型)
2. [JSON 语法子集](#2-json-语法子集)
## 1. JSON 数据类型
* null: 表示为 null
* boolean: 表示为 true 或 false
* number: 一般的浮点数表示方式
* string: 表示为 "..."
* array: 表示为 [ ... ]
* object: 表示为 { ... }
## 2. JSON 语法子集
JSON 语法子集，使用 [RFC7159](https://tools.ietf.org/html/rfc7159) 中的 [ABNF](https://tools.ietf.org/html/rfc5234) 表示:

```bash
JSON-text = ws value ws
ws = *(%x20 / %x09 / %x0A / %x0D)
value = null / false / true 
null  = "null"
false = "false"
true  = "true"
```
