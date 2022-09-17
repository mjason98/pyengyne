# ------------------------------------- FOLDERS ------------------------------------
BIN_FOLDER=bin
BUILD_FOLDER=build
SRC_FOLDER=src
INCLUDE_FOLDER=include
SHADERS_FOLDER=shaders
BIN_SHADER_FOLDER=${BIN_FOLDER}/${SHADERS_FOLDER}
TEXTURE_FOLDER=data/textures
BIN_TEXTURE_FOLDER=${BIN_FOLDER}/data
TOOLS_FOLDER=tools
BIN_TOOLS_FOLDER=${TOOLS_FOLDER}/bin


# -----------------------------------  LIBRARIES -----------------------------------

INCLUDE_FLAGS=-I$(INCLUDE_FOLDER)
RENDER_FLAGS=-ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -lGL -lvulkan
TOOLS_FLAGS=-ldl -lpthread -lassimp

binmake := $(shell if ! [[ -d ${BIN_FOLDER} ]]; then mkdir ${BIN_FOLDER}; fi)
binmake_s := $(shell if ! [[ -d ${BIN_SHADER_FOLDER} ]]; then mkdir ${BIN_SHADER_FOLDER}; fi)
binmake_t := $(shell if ! [[ -d ${BIN_TEXTURE_FOLDER} ]]; then mkdir ${BIN_TEXTURE_FOLDER}; fi)
binmake_tool := $(shell if ! [[ -d ${BIN_TOOLS_FOLDER} ]]; then mkdir ${BIN_TOOLS_FOLDER}; fi)

SHADERC=bgfx/.build/linux64_gcc/bin/shadercRelease
TEXTUREC=bgfx/.build/linux64_gcc/bin/texturecRelease

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
TSOURCES= $(wildcard $(TEXTURE_FOLDER)/*.jpg)
TSOURCES+=$(wildcard $(TEXTURE_FOLDER)/*.png)
TOBJS = $(addprefix  $(BIN_TEXTURE_FOLDER)/,$(addsuffix .dds, $(basename $(notdir $(TSOURCES)))))
# -----------------------------------------------------------------------------------
ToSOURCES= $(wildcard $(TOOLS_FOLDER)/*.cpp)
ToOBJS = $(addprefix  $(BIN_TOOLS_FOLDER)/,$(basename $(notdir $(ToSOURCES))))
# -----------------------------------------------------------------------------------

.PHONY: all debug clean run newclass

all: debug

debug: CXXFLAGS+=-g3 -ggdb
debug: CXXFLAGS+=-DDEBUG
debug: shaders
debug: textures
#change this with models
debug: tools
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

# texture compilation ********************************************************
${BIN_TEXTURE_FOLDER}/%.dds:$(TEXTURE_FOLDER)/%.jpg
	${binmake}
	${binmake_t}
	@echo $@
	@$(TEXTUREC) -f $< -o $@ -q highest
${BIN_TEXTURE_FOLDER}/%.dds:$(TEXTURE_FOLDER)/%.png
	${binmake}
	${binmake_t}
	@echo $@
	@$(TEXTUREC) -f $< -o $@ -q highest
# ------------------------------------------------------------------------------

# tools compilation ********************************************************
${BIN_TOOLS_FOLDER}/%:$(TOOLS_FOLDER)/%.cpp $(INCLUDE_FOLDER)/pge_utils.h
	${binmake_tool}
	@echo $@
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(TOOLS_FLAGS) -I$(INCLUDE_FOLDER)/pge_utils.h
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
textures: $(TOBJS)
tools: $(ToOBJS)

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
	