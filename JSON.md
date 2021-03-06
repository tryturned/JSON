<!--
 * @Author: taobo
 * @Date: 2020-11-29 14:26:26
 * @LastEditTime: 2020-12-03 19:06:06
-->
# JSON 基础
* [1. JSON 数据类型](#1-json-数据类型)  
* [2. JSON 语法子集](#2-json-语法子集)
  * [2.1 null & true & false 语法子集](#21-null-true-false-语法子集)
  * [2.2 Numbers 语法子集](#22-numbers-语法子集)
  * [2.3 Strings 语法子集](#23-strings-语法子集)
  * [2.4 Unicode --> UTF-8 编码](#24-UTF-8-编码)
  * [2.5 Arrays 语法子集](#25-Arrays-语法子集)
  * [2.6 OBJECTS 语法子集](#26-OBJECTS-语法子集)
* [3. SKILLS 整合](#3-SKILLS-整合)

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
    <img src="image\numeric.png" alt="The JSON Data Interchange Syntax" width="550" height="400">
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
<div align="center">
    <img src="image\strings.png" width="550" height="400">
</div>    

下面是 `json string` 具体语法的详细介绍：
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

### 2.4 UTF-8 编码
该 `JSON` 库只支持 `UTF-8` 编码，所以需要把 `Unicode` 码点编码成 `UTF-8`。  

`UTF-8` 的编码单元为 `8 位（1 字节）`，每个码点编码成 `1 至 4 个字节`。它的编码方式很简单，按照码点的范围，把码点的二进位分拆成 1 至最多 4 个字节, 具体规则如下所示：  


| 码点范围            | 码点位数  | 字节1     | 字节2    | 字节3    | 字节4     |
|:------------------:|:--------:|:--------:|:--------:|:--------:|:--------:|
| U+0000 ~ U+007F    | 7        | 0xxxxxxx |
| U+0080 ~ U+07FF    | 11       | 110xxxxx | 10xxxxxx |
| U+0800 ~ U+FFFF    | 16       | 1110xxxx | 10xxxxxx | 10xxxxxx |
| U+10000 ~ U+10FFFF | 21       | 11110xxx | 10xxxxxx | 10xxxxxx | 10xxxxxx |

一个例子解析多字节的情况，欧元符号 `€` → U+20AC：

1. U+20AC 在 U+0800 ~ U+FFFF 的范围内，应编码成 3 个字节。
2. U+20AC 的二进位为 10000010101100
3. 3 个字节的情况我们要 16 位的码点，所以在前面补两个 0，成为 0010000010101100
4. 按上表把二进位分成 3 组：0010, 000010, 101100
5. 加上每个字节的前缀：11100010, 10000010, 10101100
6. 用十六进位表示即：0xE2, 0x82, 0xAC

**注意:**   
需要注意的是 `4` 位的 `16` 进制数字只能表示 `0 ~ 0xFFFF`，但 `Unicode的码点`是从 `0 ~ 0x10FFFF`。所以提出了一种解决方案是在 `0 ~ 0xFFFF` 的基本多文种平面（basic multilingual plane, BMP）中预留了 `2048 个代理码点` 用于处理 `0x10000 ~ 0x10FFFF` 之间的码点。   

即如果第一个码点是 `U+D800 ~ U+DBFF`，我们便知道它的代码对的高代理项（high surrogate），之后应该伴随一个 `U+DC00 ~ U+DFFF` 的低代理项（low surrogate）。然后，我们用下列公式把代理对 (H, L) 变换成真实的码点：
```bash
codepoint = 0x10000 + (H − 0xD800) × 0x400 + (L − 0xDC00)
```
如当 `H = 0xD834` , `L = 0xDD1E`时：
```bash
H = 0xD834, L = 0xDD1E
codepoint = 0x10000 + (H − 0xD800) × 0x400 + (L − 0xDC00)
          = 0x10000 + (0xD834 - 0xD800) × 0x400 + (0xDD1E − 0xDC00)
          = 0x10000 + 0x34 × 0x400 + 0x11E
          = 0x10000 + 0xD000 + 0x11E
          = 0x1D11E
```
通过这种手段就可以得到所有 `Unicode` 编码的字符的码点了， 这样就可以基于上面的描述方式将其编码为 `utf-8` 格式了。

### 2.5 Arrays 语法子集
```bash
array = begin-array [ value *( value-separator value ) ] end-array
```
<div align="center">
    <img src="image\array.png" width="550" height="200">
</div>    
更细节的来说，其具体语法如下所示：   

```bash
array = %x5B ws [ value *( ws %x2C ws value ) ] ws %x5D
``` 
当中，`%x5B` 是左中括号 `[`，`%x2C` 是逗号 ,`%x5D` 是右中括号 `]` ，`ws` 是空白字符。一个数组可以包含零至多个值，以逗号分隔，例如 `[]`、`[1,2,true]`、`[[1,2],[3,4],"abc"]` 都是合法的数组。但注意 `JSON` 不接受末端额外的逗号，例如 `[1,2,]` 是不合法的。
### 2.6 OBJECTS 语法子集
```bash
member = string ws %x3A ws value
object = %x7B ws [ member *( ws %x2C ws member ) ] ws %x7D
```
如上所示， JSON 对象以花括号 `{}` 包裹表示，另外 JSON 对象由对象成员（member）组成，而 JSON 数组由 JSON 值组成。所谓对象成员，就是键值对，键必须为 JSON 字符串，然后值是任何 JSON 值，中间以冒号 `:`（U+003A）分隔。

## 3. SKILLS 整合  
[cppreference.com:中](https://zh.cppreference.com/w/%E9%A6%96%E9%A1%B5)  

[Cpp中匿名 Union 和匿名 Struct 用法](https://hajimeru.github.io/2019/04/03/anonymous-Union-and-struct-of-cpp/)