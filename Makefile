CC=gcc
#OUTPUTDIR =./bin/ 
#MKDIR = mkdir -p $(OUTPUTDIR)

EXEC=Shell
SRCDIR = src

SRC_C =$(wildcard $(SRCDIR)/*.c)

INCLUDES += -I inc 


OBJECTS_C= $(SRC_C:.c=.o)

OBJECTS =$(OBJECTS_C)

.PHONY: depend clean
all:$(EXEC)
	@echo "Let us make it"

$(EXEC):$(OBJECTS)
	$(CC) $(INCLUDES) -o $(EXEC) $(OBJECTS) 

.c.o:
	$(CC) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXEC)

