# ------------------------------------- FOLDERS ------------------------------------
BIN_FOLDER=bin
BUILD_FOLDER=build
SRC_FOLDER=src
INCLUDE_FOLDER=include
SHADERS_FOLDER=shaders
BIN_SHADER_FOLDER=${BIN_FOLDER}/${SHADERS_FOLDER}

# -----------------------------------  LIBRARIES -----------------------------------

INCLUDE_FLAGS=-I$(INCLUDE_FOLDER)
RENDER_FLAGS=-ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -lGL -lvulkan

binmake := $(shell if ! [[ -d ${BIN_FOLDER} ]]; then mkdir ${BIN_FOLDER}; fi)
binmake_s := $(shell if ! [[ -d ${BIN_SHADER_FOLDER} ]]; then mkdir ${BIN_SHADER_FOLDER}; fi)

SHADERC=bgfx/.build/linux64_gcc/bin/shadercRelease

# LIBRARIES : ------------------------------------------------------------------
INCLUDE_FLAGS+= -Iglfw/include
RENDER_FLAGS+= glfw/build/src/libglfw3.a

INCLUDE_FLAGS+= -Ibgfx/include -Ibx/include -Ibimg/include
RENDER_FLAGS+= bgfx/.build/linux64_gcc/bin/libbgfx-shared-libRelease.so
#RENDER_FLAGS+= bgfx/.build/linux64_gcc/bin/libbgfxRelease.a bgfx/.build/linux64_gcc/bin/libbimgRelease.a bgfx/.build/linux64_gcc/bin/libbxRelease.a 
# -----------------------------------------------------------------------------------

CXX=g++
CXXFLAGS=-std=c++14 -O2
CXXFLAGS+=$(INCLUDE_FLAGS)

SOURCES= $(wildcard $(SRC_FOLDER)/*.cpp)
SOURCES+=$(wildcard $(SRC_FOLDER)/*.c)
OBJS = $(addprefix  $(BIN_FOLDER)/,$(addsuffix .o, $(basename $(notdir $(SOURCES)))))
# -----------------------------------------------------------------------------------
SSOURCES= $(wildcard $(SHADERS_FOLDER)/*.vert)
SSOURCES+=$(wildcard $(SHADERS_FOLDER)/*.frag)
SOBJS = $(addprefix  $(BIN_SHADER_FOLDER)/,$(addsuffix .bin, $(basename $(notdir $(SSOURCES)))))
# -----------------------------------------------------------------------------------

.PHONY: all debug clean run newclass

all: debug

debug: CXXFLAGS+=-g3 -ggdb
debug: CXXFLAGS+=-DDEBUG
debug: ${BIN_FOLDER}/main

# shader compilation ********************************************************
${BIN_SHADER_FOLDER}/%.bin:$(SHADERS_FOLDER)/%.vert
	${binmake}
	${binmake_s}
	@echo $@
	@$(SHADERC) -f $< -o $@ --platform linux --type vertex --verbose -i bgfx/src

${BIN_SHADER_FOLDER}/%.bin:$(SHADERS_FOLDER)/%.frag
	${binmake}
	${binmake_s}
	@echo $@
	@$(SHADERC) -f $< -o $@ --platform linux --type fragment --verbose -i bgfx/src
# ------------------------------------------------------------------------------

${BIN_FOLDER}/%.o:$(SRC_FOLDER)/%.cpp $(INCLUDE_FOLDER)/%.h
	${binmake}
	@echo $@
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

${BIN_FOLDER}/%.o:$(SRC_FOLDER)/%.c
	${binmake}
	@echo $@
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

${BIN_FOLDER}/main: main.cpp $(OBJS)
	${binmake}
	@echo $@
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(RENDER_FLAGS)

shaders: $(SOBJS)

clean:
	@rm -r bin 
	$(shell if [[ -x main ]]; then rm main; fi)

newclass:
	@touch $(INCLUDE_FOLDER)/$(N).h
	$(shell echo -n "#include \"$(N).h\" " > $(SRC_FOLDER)/$(N).cpp)
	@echo "Created"



run: all
	@echo "---------------------- RUNING -----------------------"
	@${BIN_FOLDER}/main 
	@echo "------------------- END OF RUNING -------------------"
	