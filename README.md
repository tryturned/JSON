<!--
 * @Author: taobo
 * @Date: 2020-11-29 14:23:33
 * @LastEditTime: 2020-12-01 12:23:42
-->
# JSON 库
写出一个支持跨平台使用的 C JSON 库, 并经过了详细的单元测试。  
* [JSON 库基础](./JSON.md)  
* [JSON 库需求](#JSON-库需求)
* [JSON 库测试](#JSON-库测试)
  * [null](#\`null\`)
  * [true](#\`true\`)
  * [false](#`false`)
  * [number](#`number`)
  * [string](#`string`)
  * [array](#`array`)
  * [object](#`object`)
* [JSON 库特点](#JSON-库特点)
* [JSON 库技能](#JSON-库技能)
## JSON 库需求
- 可以将 JSON 文本解析为一个树状数据结构（parse）
- 可以将基础数据结构转换成 JSON 文本（stringify）
- 提供接口访问 JSON 文本解析后的数据结构（access）
## JSON 库测试
### `null` 
### `true`
### `false`
### `number`
### `string`
### `array`
### `object`


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


