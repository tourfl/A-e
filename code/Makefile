TARGET=emul-arm
MODE=debug

# noms des executables utilisés durant la compilation/edition des liens
CC=`which gcc` 
LD=`which gcc`
RM=`which rm` -f

#options de compilation/edition des liens
INCLUDE=-I$(INCDIR)
CFLAGS=-Wall $(INCLUDE)
LFLAGS=-lreadline -lm -lcurses
CFLAGS_DBG=$(CFLAGS) -g -DDEBUG -DVERBOSE
CFLAGS_RLS=$(CFLAGS)

# definition des repertoires de source/destination
SRCDIR=src
INCDIR=inc
OBJDIR=obj
UNTDIR=unt

# les fichiers dont on peut se débarasser
GARBAGE=*~ $(SRCDIR)/*~ $(SRCDIR)/*/*~ $(INCDIR)/*~ $(INCDIR)/*/*~ $(TESTDIR)/*~ $(SRCDIR)/*.orig $(INCDIR)/*.orig

# ou se trouve les sources (i.e., les *.c)
SRC= $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*/*.c)

# arborescence des sous-dossiers sources pour créer les sous-dossiers obj
export SRCDIRS=$(shell find $(SRCDIR) -type d)
export OBJDIRS=$(SRCDIRS:$(SRCDIR)%=$(OBJDIR)%)

# liste des fichiers objets
export OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# 1er target (celui executé par défaut quand make est lancé sans nom de cible) 
# affiche l'aide

all :
	@echo $(OBJDIRS)
	@echo ""
	@echo "Usage:"
	@echo ""
	@echo "make objdir 	=> create obj sub directories"
	@echo "make unit	=> build UNIT TEST target"
	@echo "make debug   => build DEBUG   version"
	@echo "make release => build RELEASE version"
	@echo "make clean   => clean everything"
	@echo "make archive => produce archive .tar.gz in ../ directory"
	@echo $(LD_LIBRARY_PATH)


# utilisation d'un Makefile "esclave"

unit:
	@(cd $(UNTDIR) && $(MAKE))


unit_clean:
	@(cd $(UNTDIR) && $(MAKE) clean)

unit_srcdirs:
	@(cd $(UNTDIR) && $(MAKE) srcdirs)

unit_objdirs:
	@(cd $(UNTDIR) && $(MAKE) objdirs)


debug   : 
	MODE=debug
	make $(TARGET)

release : 
	MODE=release
	make $(TARGET)

$(TARGET): $(OBJ)
	$(LD) $^ $(LFLAGS) -o $@

objdirs :
	mkdir -p $(OBJDIRS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
ifeq ($(MODE),debug)
	$(CC) -o $@ -c $< $(CFLAGS_DBG)
else
	$(CC) -o $@ -c $< $(CFLAGS_RLS)
endif

clean : 
	$(RM) $(TARGET) $(GARBAGE) $(OBJ)

# créé l'archive à envoyer à votre tuteur (pas de rar SVP! et interdiction absolu d'envoyer autre chose qu'une archive, il ne doit y avoir qu'un seul fichier joint dans l'e-mail !)
archive : 
	make clean 
	tar -czvf ../$(notdir $(PWD) )-ZEGGAI_DEIN-`date +%d-%m-%H-%M`.tgz .
	echo "Fichier archive ../emulARM-ZEGGAI_DEIN-`date +%d-%m-%H-%M`.tgz genere"
