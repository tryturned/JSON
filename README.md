<!--
 * @Author: taobo
 * @Date: 2020-11-29 14:23:33
 * @LastEditTime: 2020-11-29 14:31:23
-->
# JSON 库
写出一个支持跨平台使用的 C JSON 库
## 目标
- 符合标准的 JSON 解析器和生成器
- 手写的递归下降解析器（recursive descent parser）
- 使用标准 C 语言（C89）
- 跨平台／编译器（如 Windows／Linux／OS X，vc／gcc／clang）
- 仅支持 UTF-8 JSON 文本
- 仅支持以 double 存储 JSON number 类型
- 解析器和生成器的代码合共少于 500 行
## 需求
1. 把 JSON 文本解析为一个树状数据结构（parse）
2. 提供接口访问该数据结构（access）
3. 把数据结构转换成 JSON 文本（stringify）
## skills
- 测试驱动开发（test driven development, TDD）
- C 语言编程风格
- 数据结构
- API 设计
- 断言
- Unicode
- 浮点数
- Github、CMake、valgrind、Doxygen 等工具