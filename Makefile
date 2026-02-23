# zyrthi-core/Makefile
# 纯跨平台框架：仅依赖通用 HAL 接口，无芯片相关配置
HAL_DIR ?= ../zyrthi-hal
BUILD_DIR ?= build

CC ?= clang
AR ?= ar
# 通用编译参数（无芯片专属参数）
CFLAGS ?= -std=c99 -Wall -Wextra -Iinclude -I$(HAL_DIR)/include -fPIC
# 仅链接通用 HAL 空库
LDFLAGS ?= -L$(HAL_DIR)/build -lzyrthi-hal
ARFLAGS ?= rcs

# Core 层源文件（纯框架逻辑）
SRC = src/core.c
OBJ = $(addprefix $(BUILD_DIR)/, $(SRC:.c=.o))
TARGET = $(BUILD_DIR)/libzyrthi-core.a

# 默认目标：编译通用 HAL + Core
all: mkdir_build build_hal $(TARGET)

# 编译通用 HAL（空库）
build_hal:
	$(MAKE) -C $(HAL_DIR) all

# 创建 build 目录
mkdir_build:
	@mkdir -p $(BUILD_DIR)/src

# 编译 Core 静态库
$(TARGET): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

# 编译 .c 为 .o
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理
clean:
	rm -rf $(BUILD_DIR) src/*.o
	$(MAKE) -C $(HAL_DIR) clean

.PHONY: all clean mkdir_build build_hal