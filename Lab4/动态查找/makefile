# 编译器
CXX = g++

# 编译选项
CXXFLAGS = -std=c++11 -Wall -O2

# 目标文件
TARGET = search_table

# 源文件
SRCS = main.cpp static_table.cpp dynamic_table.cpp

# 目标文件
OBJS = $(SRCS:.cpp=.o)

# 默认目标
all: $(TARGET)

# 链接
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 编译
%.o: %.cpp search_table.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理
clean:
	rm -f $(OBJS) $(TARGET)

# 伪目标
.PHONY: all clean