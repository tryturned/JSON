<!--
 * @Author: taobo
 * @Date: 2020-11-29 14:26:26
 * @LastEditTime: 2020-11-29 20:25:50
-->
# JSON 基础
* [1. JSON 数据类型](#1-json-数据类型)  
* [2. JSON 语法子集](#2-json-语法子集)
  * [2.1 null & true & false 语法子集](#2.1-null-true-false-语法子集)
  * [2.2 Numbers 语法子集](#2.2-Numbers-语法子集)
## 1. JSON 数据类型
* null: 表示为 null
* boolean: 表示为 true 或 false
* number: 一般的浮点数表示方式
* string: 表示为 "..."
* array: 表示为 [ ... ]
* object: 表示为 { ... }
## 2. JSON 语法子集
JSON 语法子集，使用 [RFC7159](https://tools.ietf.org/html/rfc7159) 中的 [ABNF](https://tools.ietf.org/html/rfc5234) 表示:
### 2.1 null true false 语法子集
```bash
JSON-text = ws value ws
ws = *(%x20 / %x09 / %x0A / %x0D)
value = null / false / true 
null  = "null"
false = "false"
true  = "true"
```
其中 `ws` 表示 `whitespace` . 第二行表示 `ws` 可能是多个  `space` 或 `tab` 或 `\r` 或 `\n`.     
下面指定了 `null` `false` `true`的字面值.
### 2.2 Numbers 语法子集
<div align="center">
    <img src="image\numeric.png">
</div>  

图片来源: [The JSON Data Interchange Syntax](https://www.ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf)  
```bash
number = [ "-" ] int [ frac ] [ exp ]
int = "0" / digit1-9 *digit
frac = "." 1*digit
exp = ("e" / "E") ["-" / "+"] 1*digit
```

