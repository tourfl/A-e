TARGET=unit

# noms des executables utilisés durant la compilation/edition des liens
CC=`which gcc` 
LD=`which gcc`
RM=`which rm` -f
MAKEHD=makeheaders

#options de compilation/édition des liens
INCLUDE=-I$(INCDIR) -I$(DEPINCDIR)
CFLAGS=-Wall $(INCLUDE)
LFLAGS=-lreadline -lm -lcurses -lcunit

#définition des répertoires de source/destination
INCDIR=inc
DEPINCDIR=../inc
DEPOBJDIR=../obj
DEPSRCDIR=../src
SRCDIR=src
OBJDIR=obj

#liste des fichiers sources, objets et headers
SRC=$(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*/*.c)
CUOBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
CUINC=$(SRC:$(SRCDIR)/%.c=$(INCDIR)/%.h)

#liste des fichiers contenant des fonctions à tester
DEPOBJ_M=$(OBJ:%.o=../%.o)
DEPOBJ=$(filter-out $(DEPOBJDIR)/main.o, $(DEPOBJ_M))

all: $(CUINC)
	@echo $(SRC)
	make $(TARGET)

# doit être fait avant la compilation pour que les headers soient à jour
$(INCDIR)/%.h: $(SRCDIR)/%.c
	$(MAKEHD) $<:$@

$(TARGET): $(CUOBJ) $(DEPOBJ)
	$(LD) $^ $(LFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)


srcdirs:
	mkdir -p $(SRCDIRS)

incdirs:
	mkdir -p $(INCDIRS)

objdirs:
	mkdir -p $(OBJDIRS)




clean:
	$(RM) $(CUOBJ) $(TARGET)
