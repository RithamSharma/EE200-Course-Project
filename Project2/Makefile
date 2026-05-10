CC = g++
CFLAGS = -std=c++20 -Wall -Wextra -Iinclude
LDFLAGS = -lreadline

SRC_DIR = src
INC_DIR = include
OBJ_DIR = build
TARGET = novashell

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Vision module sources
VISION_DIR = src/vision
VISION_SRCS = $(wildcard $(VISION_DIR)/*.cpp)
VISION_PLATFORM_SRCS = $(wildcard $(VISION_DIR)/platform/*.cpp)
VISION_OBJS = $(VISION_SRCS:$(VISION_DIR)/%.cpp=$(OBJ_DIR)/vision/%.o) \
              $(VISION_PLATFORM_SRCS:$(VISION_DIR)/platform/%.cpp=$(OBJ_DIR)/vision/platform/%.o)

all: $(TARGET)

$(TARGET): $(OBJS) $(VISION_OBJS)
	$(CC) $(OBJS) $(VISION_OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/vision/%.o: $(VISION_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)/vision
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/vision/platform/%.o: $(VISION_DIR)/platform/%.cpp
	@mkdir -p $(OBJ_DIR)/vision/platform
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
