###################################################
# Thank you so much to Fiz for his answer on topic 
# "Makefile template for c++ large projects". Source:
# http://stackoverflow.com/questions/14844268/makefile-template-for-large-c-project
###################################################

# Project Name (executable)
PROJECT = GameEngine
# Compiler
CC = g++

# Run Options       
COMMANDLINE_OPTIONS = /dev/ttyS0

# Compiler options during compilation
COMPILE_OPTIONS = -g -std=c++11 -I$(HEADERS)

#Header include directories
HEADERS = Engine
#Libraries for linking
LIBS = -lGL -lGLU -lGLEW `sdl-config --libs` -lSDL_image

# Dependency options
DEPENDENCY_OPTIONS = -MM -std=c++11 -I$(HEADERS)

#-- Do not edit below this line --

# Subdirs to search for additional source files
SUBDIRS := $(shell ls -F | grep "\/" )
DIRS := ./Engine/Core/Interfaces/ \
		./Engine/Core/Math/ \
		./Engine/Core/Random/ \
		./Engine/Core/Singleton/ \
		./Engine/Core/Console/ \
		./Engine/Core/Debug/ \
		./Engine/Core/Parsers/XML/TinyXml/ \
		./Engine/Core/ \
		./Engine/Wrappers/*/ \
		./Engine/VisualEffects/*/ \
		./Engine/Systems/*/ \
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

# Include dependencies (if there are any)
ifneq "$(strip $(DEPENDENCIES))" ""
  include $(DEPENDENCIES)
endif

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