TARGET=unit

# noms des executables utilisés durant la compilation/edition des liens
CC=`which gcc` 
LD=`which gcc`
RM=`which rm` -f

#options de compilation/édition des liens
INCLUDE=-I$(INCDIR)
CFLAGS=-Wall $(INCLUDE)
LFLAGS=-lreadline -lm -lcurses -lcunit

#définition des répertoires de source/destination
INCDIR=inc
SRCDIR=src
OBJDIR=obj

#liste des fichiers sources et objets
SRC=$(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*/*.c)
CUOBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

#liste des fichiers contenant des fonctions à tester
DEPOBJ_M=$(OBJ:%.o=../%.o)
DEPOBJ=$(filter-out ../$(OBJDIR)/main.o, $(DEPOBJ_M))

all:
	make $(TARGET)

$(TARGET): $(CUOBJ)
	$(LD) $^ $(DEPOBJ) $(LFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

srcdirs:
	mkdir -p $(SRCDIRS)

objdirs :
	mkdir -p $(OBJDIRS)

clean:
	$(RM) $(CUOBJ) $(TARGET)
