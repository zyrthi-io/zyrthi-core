# zyrthi/core/Makefile
# 编译器与编译选项
CC ?= gcc
AR ?= ar
CFLAGS ?= -Wall -Wextra -std=c99 -Os

# 头文件路径（Core 自身 include + HAL 层 include）
INC_DIRS = include \
           ../hal/include

# 源文件（简化层级，直接引用 src 下的 .c）
SRC_FILES = src/core.c \
            src/helper.c \

# 目标静态库
TARGET = libzyrthi-core.a

# 编译规则
all: $(TARGET)

$(TARGET): $(SRC_FILES)
	# 编译所有 .c 为 .o 文件
	$(CC) $(CFLAGS) -I$(INC_DIRS) -c $(SRC_FILES)
	# 打包为静态库
	$(AR) rcs $(TARGET) *.o

# 清理编译产物
clean:
	rm -f $(TARGET) *.o