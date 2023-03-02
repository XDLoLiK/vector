SRC_DIRS := . stdlike
VPATH += $(SRC_DIRS)

INC_DIRS := include
BIN_DIR := bin
BUILD_DIR := build

APPLICATION := $(BUILD_DIR)/vector.out

SRC := $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJ := $(addprefix $(BIN_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(SRC))))

CXX := g++
CXX_FLAGS := $(addprefix -I, $(INC_DIRS)) $(addprefix -I, $(SRC_DIRS))

CXX_FLAGS += -D _DEBUG -ggdb3 -std=c++20 -O0 -pthread -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations\
-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported\
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security\
-Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual\
-Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-null-sentinel\
-Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override\
-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros\
-Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector\
-fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer\
-Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -fno-elide-constructors -fsanitize=address

all: prepare $(APPLICATION)

$(APPLICATION): $(OBJ)
	@$(CXX) $(LD_FLAGS) $^ -o $@ $(CXX_FLAGS) $(SDL_FLAGS)

$(BIN_DIR)/%.o: %.cpp
	@$(CXX) $< -c -MD -o $@ $(CXX_FLAGS)

-include $(wildcard $(BIN_DIR)/*.d)

.PHONY: prepare clean info run gdb valgrind

run: all
	@$(APPLICATION)

gdb: all
	@gdb $(APPLICATION)

valgrind: all
	@valgrind --leak-check=full $(APPLICATION)

prepare:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(BUILD_DIR)

info:
	@echo [*] OBJ: $(OBJ)
	@echo [*] SRC: $(SRC)
	@echo [*] DLL: $(DLL_FILES)
