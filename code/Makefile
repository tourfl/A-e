TARGET=emul-arm
MODE=debug
LEAKFINDER=valgrind
TESTFILE=

# noms des executables utilisés durant la compilation/edition des liens
CC=`which gcc` 
LD=`which gcc`
RM=`which rm` -f

#options de compilation/edition des liens
INCLUDE=-I$(INCDIR)
CFLAGS=-Wall $(INCLUDE)
LFLAGS=-lreadline -lm -lcurses

ifeq ($(MODE),debug)
	CFLAGS+= -g -DDEBUG -DVERBOSE
endif

#options du leakfinder

LEAKFLAGS=--leak-check=full --show-leak-kinds=all

# definition des repertoires de source/destination
SRCDIR=src
INCDIR=inc
OBJDIR=obj
UNTDIR=unt

# les fichiers dont on peut se débarasser
GARBAGE=*~ $(SRCDIR)/*~ $(SRCDIR)/*/*~ $(INCDIR)/*~ $(INCDIR)/*/*~ $(TESTDIR)/*~ $(SRCDIR)/*.orig $(INCDIR)/*.orig

# ou se trouve les sources (i.e., les *.c)
SRC= $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*/*.c)

# liste des headers
INC= $(wildcard $(INCDIR)/*.h) $(wildcard $(INCDIR)/*/*.h)


# arborescence des sous-dossiers sources pour créer les sous-dossiers obj
export SRCDIRS=$(shell find $(SRCDIR) -type d)
export OBJDIRS=$(SRCDIRS:$(SRCDIR)%=$(OBJDIR)%)
export INCDIRS=$(SRCDIRS:$(SRCDIR)%=$(INCDIR)%)

# liste des fichiers objets
export OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# 1er target (celui executé par défaut quand make est lancé sans nom de cible) 
# affiche l'aide

all:
	@echo "make help for help :)"
	@($(MAKE) $(TARGET))

$(TARGET): $(OBJ)
	$(LD) $^ $(LFLAGS) -o $@


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

# utilisation d'un Makefile "esclave"

unit:
	@($(MAKE) $(TARGET))
	@(cd $(UNTDIR) && $(MAKE))

leak:
	@( $(MAKE) )
	@( $(LEAKFINDER) $(LEAKFLAGS) ./$(TARGET) $(TESTFILE) )

clean : 
	$(RM) $(TARGET) $(GARBAGE) $(OBJ)

# créé l'archive à envoyer à votre tuteur (pas de rar SVP! et interdiction absolu d'envoyer autre chose qu'une archive, il ne doit y avoir qu'un seul fichier joint dans l'e-mail !)
archive : 
	make clean 
	tar -czvf ../$(notdir $(PWD) )-ZEGGAI_DEIN-`date +%d-%m-%H-%M`.tgz .
	echo "Fichier archive ../emulARM-ZEGGAI_DEIN-`date +%d-%m-%H-%M`.tgz genere"


help:
	@echo ""
	@echo "Usage:"
	@echo ""
	@echo "make              => build DEBUG   version"
	@echo "make MODE=release => build RELEASE version"
	@echo "make unit         => build UNIT TEST target"
	@echo "make clean        => clean everything"
	@echo "make archive      => produce archive .tar.gz in ../ directory"



unit_clean:
	@(cd $(UNTDIR) && $(MAKE) clean)

unit_srcdirs:
	@(cd $(UNTDIR) && $(MAKE) srcdirs)

unit_incdirs:
	@(cd $(UNTDIR) && $(MAKE) incdirs)

unit_objdirs:
	@(cd $(UNTDIR) && $(MAKE) objdirs)

objdirs :
	mkdir -p $(OBJDIRS)
