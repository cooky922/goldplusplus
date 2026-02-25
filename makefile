# Variables
CXX_COMPILER := g++
CXX_LINKER   := ar
CXX_VERSION  := -std=gnu++26
CXX_COMPILE_FLAGS := -ffold-simple-inlines \
                     -fconstexpr-fp-except \
                     -pedantic -Wall -m64
DEFINES :=

# Allow the path to the nonstdlib dependency to be overridden by the user
NONSTDLIB_DIR ?= ../nonstdlib

# Use relative paths for the local include and the overridable dependency path
INCLUDE_DIR  := -I$(NONSTDLIB_DIR)/include \
                -Iinclude

CXX_COMPILE_COMMON := $(CXX_COMPILER) $(CXX_COMPILE_FLAGS) $(CXX_VERSION) $(DEFINES) $(INCLUDE_DIR)

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp,lib/%.o,$(SRC))

# Default Target: build and clean
all: rebuild clean

# Build core archive from object files
lib/libgold++_core.a: $(OBJ)
	$(CXX_LINKER) rcs $@ $(OBJ)

# Merge archive
lib/libgold++.a: lib/libgold++_core.a
	cd lib && $(CXX_LINKER) -M < merge_lib_script.mri && cd ..
	strip --strip-unneeded $@

# General symbol list
lib/libgold++_sym.txt: lib/libgold++.a
	nm -C $< > $@

# Compile any src/*.cpp to lib/*.o
lib/%.o: src/%.cpp
	$(CXX_COMPILE_COMMON) -c $< -o $@

# Clean any intermediate and output files
clean:
	del testsuite\*.o
	del testsuite\*.exe
	del lib\libgold++_core.a
	del lib\*.o

# Remove the library first to rebuild
reset:
	del lib\libgold++.a
	del lib\libgold++_sym.txt

# Rebuild
rebuild: reset build

# Build
build: lib/libgold++.a lib/libgold++_sym.txt

.PHONY: all clean