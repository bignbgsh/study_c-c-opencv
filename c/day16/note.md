# day16 用 make 编译

## 为什么敲 `make` 提示找不到命令
MinGW-w64 里 GNU make 的程序名是 `mingw32-make.exe`，**没有 `make.exe`**。
本目录放了个 `make.bat` 转发给它，所以在这个文件夹里可以直接敲 `make`。
想全局可用：把 `D:\c-translation\mingw64\bin\mingw32-make.exe` 复制一份改名为 `make.exe`。

## 三个命令
```
make          # 编译，生成 calculator.exe
make run      # 编译并运行
make clean    # 删除 .o 和 .exe
```
不用 make.bat 时用全名 `mingw32-make`。

## Makefile 必须守的两条规矩
1. **每条命令前必须是 Tab，不是空格。**
   原来报 `Makefile:4: *** missing separator. Stop.` 就是这个原因。
   VS Code 里容易把 Tab 自动换成空格，注意状态栏的 "Spaces: 4" 要改成 Tab。
2. **目标名要等于真实产物。** Windows 上 `gcc -o calculator` 生成的其实是 `calculator.exe`；
   目标若写 `calculator`，make 每次都认为没生成、每次都全量重编。

## 原 Makefile 的其他错
- 依赖写 `math.ultis.o`（点），规则名写 `math_utils.o`（下划线）→ `No rule to make target`。
- 第 9 行混进一个孤立的 `30`，既不是规则也不是命令 → 语法错。
- `clean` 里用 `rm -f`，Windows cmd 没有 rm → 已按平台换成 `del /Q`。

## 源码侧的坑（会让链接失败，但不是 make 的错）
原来 `main.c` 写的是 `#include "math.ultis.c"`，而 Makefile 又把 `math.ultis.c`
单独编成 `math.ultis.o` 一起链接 → 四个函数各被定义两次：
```
multiple definition of `add'
```
改法：`main.c` 只 include 头文件 `math.ultis.h`，函数体由 `math.ultis.o` 提供。

## 顺手修掉的 bug
- 头文件原型拼错：`subsract` / `multiple`，和实现里的 `subtract` / `multiply` 对不上。
- `divide()` 里 `return (a / b);` 是整数除法，7/2 只得 3；已改为 `(float)a / b`。

## 源文件是 GBK 编码
`main.c` 的中文是 GBK。VS Code 打开乱码时，右下角选 "GB2312/GBK" 重开。
别随手另存为 UTF-8，否则 exe 在 cmd 里输出会乱码。

## 原始文件备份
`bak\` 目录里存了改动前的 Makefile / main.c / math.ultis.h / math.ultis.c。