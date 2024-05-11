CC         := clang++
CFLAGS     := -Wall -Wextra -O0 -ggdb -std=c++11 -D_DEBUG
LDFLAGS    := -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -lfmt

INC_FLAGS  := -I./include

SRC_FOLDER := ./src
HDR_FOLDER := ./include
OBJ_FOLDER := ./obj
SRC_FILE   := $(shell find $(SRC_FOLDER) -type f -iname "*.cpp")
OBJ_FILE   := $(patsubst $(SRC_FOLDER)/%.cpp, $(OBJ_FOLDER)/%.o, $(SRC_FILE))

BIN_FOLDER := ./bin
BIN_FILE   := $(BIN_FOLDER)/hsje

.PHONY: clean

$(BIN_FILE): $(OBJ_FILE)
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC_FLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_FOLDER)/%.o: $(SRC_FOLDER)/%.cpp $(HDR_FOLDER)/%.hpp
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(INC_FLAGS) $< -o $@

clean:
	rm -rf $(OBJ_FOLDER) $(BIN_FOLDER)
