# 编译器设置
CXX = g++
CXXFLAGS = -Wall -std=c++11

# 目标文件
TARGET = algraph_program

# 源文件
SRCS = main.cpp

# 目标文件
OBJS = $(SRCS:.cpp=.o)

# 默认目标
all: $(TARGET)

# 链接
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# 编译
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理
clean:
	rm -f $(OBJS) $(TARGET)

# 运行程序
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run