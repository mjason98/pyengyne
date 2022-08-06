# ------------------------------------- FOLDERS ------------------------------------
BIN_FOLDER=bin
BUILD_FOLDER=build
SRC_FOLDER=src
INCLUDE_FOLDER=include

# -----------------------------------  LIBRARIES -----------------------------------

INCLUDE_FLAGS=-I$(INCLUDE_FOLDER)
RENDER_FLAGS=-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

binmake := $(shell if ! [[ -d ${BIN_FOLDER} ]]; then mkdir ${BIN_FOLDER}; fi)

# -----------------------------------------------------------------------------------
CXX=g++
CXXFLAGS=-std=c++17 -O2
CXXFLAGS+=$(INCLUDE_FLAGS)

SOURCES= $(wildcard $(SRC_FOLDER)/*.cpp)
SOURCES+=$(wildcard $(SRC_FOLDER)/*.c)
OBJS = $(addprefix  $(BIN_FOLDER)/,$(addsuffix .o, $(basename $(notdir $(SOURCES)))))
# -----------------------------------------------------------------------------------

.PHONY: all debug clean run newclass

all: debug

debug: CXXFLAGS+=-g3 -ggdb
debug: ${BIN_FOLDER}/main


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
	