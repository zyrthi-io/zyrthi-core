# zyrthi-core Makefile
# 编译器与编译选项
CC ?= gcc
AR ?= ar
CFLAGS ?= -Wall -Wextra -std=c99 -Os

# 头文件路径（Core 自身 include + HAL submodule）
INC_DIRS = -Iinclude -Ideps/zyrthi-hal/include

# 源文件
SRC_FILES = src/core.c src/helper.c

# 目标静态库
TARGET = build/libzyrthi-core.a

# 编译规则
all: $(TARGET)

$(TARGET): $(SRC_FILES)
	@mkdir -p build
	$(CC) $(CFLAGS) $(INC_DIRS) -c $(SRC_FILES)
	$(AR) rcs $(TARGET) *.o
	@mv *.o build/

# 清理编译产物
clean:
	rm -rf build

# 更新依赖
deps:
	git submodule update --init --recursive

.PHONY: all clean deps
