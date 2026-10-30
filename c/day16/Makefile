# ============================================================
# day16 计算器  --  Windows + MinGW-w64 Makefile
#
# 用法：
#   make            # 编译（等价 mingw32-make，本目录的 make.bat 提供 make 命令）
#   make run        # 编译并运行
#   make clean      # 删除 .o 和 .exe
#
# 两条铁律：
#   1) 每条命令前必须是【Tab】，不能用空格 —— 否则报 missing separator
#   2) 目标名要和实际产物一致（Windows 上 gcc -o calculator 生成的是 calculator.exe）
# ============================================================

CC      = gcc
CFLAGS  = -Wall -g
TARGET  = calculator.exe
OBJS    = main.o math.ultis.o

# Windows 的 cmd 里没有 rm，所以按平台选择删除命令
ifeq ($(OS),Windows_NT)
RM_F = del /Q
else
RM_F = rm -f
endif

all: $(TARGET)

# 链接：把两个 .o 合成一个可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# 编译 main.c（依赖头文件，头文件改动会自动重编）
main.o: main.c math.ultis.h
	$(CC) $(CFLAGS) -c main.c -o main.o

# 编译 math.ultis.c
math.ultis.o: math.ultis.c math.ultis.h
	$(CC) $(CFLAGS) -c math.ultis.c -o math.ultis.o

run: $(TARGET)
	.\$(TARGET)

clean:
	-$(RM_F) $(OBJS) $(TARGET)

.PHONY: all run clean
