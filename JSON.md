<!--
 * @Author: taobo
 * @Date: 2020-11-29 14:26:26
 * @LastEditTime: 2020-11-30 12:02:15
-->
# JSON 基础
* [1. JSON 数据类型](#1-json-数据类型)  
* [2. JSON 语法子集](#2-json-语法子集)
  * [2.1 null & true & false 语法子集](#21-null-true-false-语法子集)
  * [2.2 Numbers 语法子集](#22-numbers-语法子集)
  * [2.3 Strings 语法子集](#23-strings-语法子集)
## 1. JSON 数据类型
* null: 表示为 null
* boolean: 表示为 true 或 false
* number: 一般的浮点数表示方式
* string: 表示为 "..."
* array: 表示为 [ ... ]
* object: 表示为 { ... }
## 2. JSON 语法子集
JSON 语法子集，使用 [RFC7159](https://tools.ietf.org/html/rfc7159) 规定表示:
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


无法使用下面语法表示的数值(例如 `Infinity` 和 `NaN` )在 `JSON` 中是不允许的：  
```matlab
      number = [ minus ] int [ frac ] [ exp ]

      decimal-point = %x2E       ; .

      digit1-9 = %x31-39         ; 1-9

      e = %x65 / %x45            ; e E

      exp = e [ minus / plus ] 1*DIGIT

      frac = decimal-point 1*DIGIT

      int = zero / ( digit1-9 *DIGIT )

      minus = %x2D               ; -

      plus = %x2B                ; +

      zero = %x30                ; 0

```
### 2.3 Strings 语法子集
```matlab
      string = quotation-mark *char quotation-mark

      char = unescaped /
          escape (
              %x22 /          ; "    quotation mark  U+0022
              %x5C /          ; \    reverse solidus U+005C
              %x2F /          ; /    solidus         U+002F
              %x62 /          ; b    backspace       U+0008
              %x66 /          ; f    form feed       U+000C
              %x6E /          ; n    line feed       U+000A
              %x72 /          ; r    carriage return U+000D
              %x74 /          ; t    tab             U+0009
              %x75 4HEXDIG )  ; uXXXX                U+XXXX

      escape = %x5C              ; \

      quotation-mark = %x22      ; "

      unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
```
上面的语法表示 `JSON` 字符串是由前后两个双引号夹着零至多个字符组成，字符分为无转义字符或转义序列。  

在转义字符列表中需要特别注意的是 `\uXXXX` ，这种情况表明在 `JSON Strings` 中若一个字符属于 `BMP` (<a href="https://encyclopedia2.thefreedictionary.com/Basic+Multilingual+Plane">Basic Multilingual Plane</a>) , 那么该字符可以使用 `\uXXXX` 这种六个字符来表示，其中 `X` 表示 `16` 进制数。例如，一个字符串包含只有一个 `\` 字符可以表示为 `"\u005C"`。

