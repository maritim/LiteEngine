###################################################
# Thank you so much to Fiz for his answer on topic 
# "Makefile template for c++ large projects". Source:
# http://stackoverflow.com/questions/14844268/makefile-template-for-large-c-project
#
# Thanks to Alexandru Naiman for optimization ideas
###################################################

# Project Name (executable)
PROJECT = GameEngine.out
# Compiler
CC = g++

# Run Options       
COMMANDLINE_OPTIONS = /dev/ttyS0

# Makefile Params
# make CONFIG=DEBUG

# Compiler options during compilation
ifeq ($(CONFIG),RELEASE)
	COMPILE_OPTIONS = -g0 -Wall -Werror -march=native -mtune=native -funroll-loops -Ofast -fno-math-errno -fomit-frame-pointer -foptimize-strlen -ftree-loop-distribution -ftree-loop-distribute-patterns -ffast-math -flto -std=c++11 -I$(HEADERS)
else
	COMPILE_OPTIONS = -g2 -O0 -Wall -Werror -std=c++11 -I$(HEADERS)
endif

#Header include directories
HEADERS = Engine
#Libraries for linking
LIBS = -lGL -lGLU -lGLEW -lSDL2 -lSDL2_image -lassimp

# Dependency options
DEPENDENCY_OPTIONS = -MM -std=c++11 -I$(HEADERS)

#-- Do not edit below this line --

# Subdirs to search for additional source files
SUBDIRS := $(shell ls -F | grep "\/" )
DIRS := ./Engine/Core/Interfaces/ \
		./Engine/Core/Math/ \
		./Engine/Core/Math/glm/ \
		./Engine/Core/Random/ \
		./Engine/Core/Singleton/ \
		./Engine/Core/Console/ \
		./Engine/Core/Debug/ \
		./Engine/Core/Parsers/XML/TinyXml/ \
		./Engine/Core/Intersections/ \
		./Engine/Core/ \
		./Engine/Wrappers/*/ \
		./Engine/VisualEffects/*/ \
		./Engine/Systems/*/ \
		./Engine/Debug/*/ \
		./Engine/Utils/*/ \
		./Engine/Utils/*/*/ \
		./Engine/*/ \
		./Engine/
SOURCE_FILES := $(foreach d, $(DIRS), $(wildcard $(d)*.cpp) )

# Create an object file of every cpp file
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCE_FILES))

# Dependencies
DEPENDENCIES = $(patsubst %.cpp, %.d, $(SOURCE_FILES))

# Create .d files
%.d: %.cpp
	$(CC) $(DEPENDENCY_OPTIONS) $< -MT "$*.o $*.d" -MF $*.d

# Make $(PROJECT) the default target
all: $(DEPENDENCIES) $(PROJECT)

$(PROJECT): $(OBJECTS)
	$(CC) -o $(PROJECT) $(OBJECTS) $(LIBS)

# Compile every cpp file to an object
%.o: %.cpp
	$(CC) -c $(COMPILE_OPTIONS) -o $@ $< $(HEADERS)

# Build & Run Project
run: $(PROJECT)
	./$(PROJECT) $(COMMANDLINE_OPTIONS)

# Clean & Debug
.PHONY: makefile-debug
makefile-debug:

.PHONY: clean
clean:
	rm -f $(PROJECT) $(OBJECTS)

.PHONY: depclean
depclean:
	rm -f $(DEPENDENCIES)

clean-all: clean depclean
