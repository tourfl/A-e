TARGET=emul-arm

# noms des executables utilisés durant la compilation/edition des liens
CC=`which gcc` 
LD=`which gcc`
RM=`which rm` -f

#options de compilation/edition des liens
INCLUDE=-I$(INCDIR)
CFLAGS=-Wall $(INCLUDE)
LFLAGS=-lreadline -lm -lcurses -lcunit
CFLAGS_DBG=$(CFLAGS) -g -DDEBUG -DVERBOSE
CFLAGS_RLS=$(CFLAGS)

# definition des repertoires de source/destination
SRCDIR=src
INCDIR=inc
OBJDIR=obj

# les fichiers dont on peut se débarasser
GARBAGE=*~ $(SRCDIR)/*~ $(INCDIR)/*~ $(TESTDIR)/*~ $(SRCDIR)/*.orig $(INCDIR)/*.orig

# ou se trouve les sources (i.e., les *.c)
SRC= $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*/*.c)

# arborescence des sous-dossiers sources pour créer les sous-dossiers obj
SRCARB:=$(shell cd src && ls -d */ && cd ..)


# les objets avec l'option DEBUG s'appeleront fichier.dbg.o
# ceux de la release fichier.rls.o
#TODO
OBJ_DBG=$(SRC:src/%.c=obj/%.dbg.o)
OBJ_RLS=$(SRC:src/%.c=obj/%.rls.o)

# 1er target (celui executé par défaut quand make est lancé sans nom de cible) 
# affiche l'aide
all :
	@echo ""
	@echo "Usage:"
	@echo ""
	@echo "make objdir 	=> create obj sub directories"
	@echo "make debug   => build DEBUG   version"
	@echo "make release => build RELEASE version"
	@echo "make clean   => clean everything"
	@echo "make archive => produce archive .tar.gz in ../ directory"
	@echo $(LD_LIBRARY_PATH)


debug   : $(OBJ_DBG)
	$(LD) $^ $(LFLAGS) -o $(TARGET)

release : $(OBJ_RLS)
	$(LD) $^ $(LFLAGS) -o $(TARGET)

objdirs :
	mkdir -p $(addprefix $(OBJDIR)/, $(SRCARB))

obj/%.dbg.o: src/%.c
	$(CC) -o $@ -c $< $(CFLAGS_DBG)

obj/%.rls.o: src/%.c
	$(CC) -o $@ -c $< $(CFLAGS_RLS)

clean : 
	$(RM) $(TARGET) $(GARBAGE) $(OBJ_DBG) $(OBJ_RLS)

# créé l'archive à envoyer à votre tuteur (pas de rar SVP! et interdiction absolu d'envoyer autre chose qu'une archive, il ne doit y avoir qu'un seul fichier joint dans l'e-mail !)
archive : 
	make clean 
	tar -czvf ../$(notdir $(PWD) )-ZEGGAI_DEIN-`date +%d-%m-%H-%M`.tgz .
	echo "Fichier archive ../emulARM-ZEGGAI_DEIN-`date +%d-%m-%H-%M`.tgz genere"
