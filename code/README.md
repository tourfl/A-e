# ARM-emulator
Projet de cours en filière SICOM (Grenoble INP)

## Livrable 3 : Exécution du programme

- Jeux de tests
- Désassemblage de toutes les instructions (sauf la version T4 de LDR et STR)
- Implantation de la commande run
- Mise en place de la boucle d'exécution
- Implantation des fonctions exécutant les instructions du microprocesseur
- Gestion des erreurs
- Génération des traces

## Livrable 2 : Désassemblage

- Jeux de tests
- Lecture du dictionnaire d'instructions
- Désassemblage des instructions (disasm) [pop, push, ldr et str sont en option mais seront à faire]
- Gestion des erreurs
- Génération des traces

## 1er livrable :

- Jeux de tests
- Mise en place de l'interpréteur
- Implantation de la mémoire et des registres
- Lecture des fichiers ELF
- Implantation des commandes de la section 5.2.1 (sauf disasm (disassemble)) et 5.2.2
- Gestion des erreurs
- Génération des traces

## Description du programme

Le programme final (non encore totalement implémenté) est un émulateur d'une architecture ARMv7-M. 
Il permet de simuler l'exécution de fichiers objet ELF écrit avec la syntaxe UAL, en tout ce qui concerne la mémoire et l'utilisation des registres. 
Celui-ci se compose de 2 parties principales : le simulateur et l'interpréteur (termes peut-être à revoir).

### Simulateur

Il est composé de 3 éléments :

- une structure simulant la mémoire vive
- une structure simulant les registres
- une structure contenant le code des instructions à décoder

### Interpréteur

C'est l'intermédiaire entre le simulateur et l'utilisateur. Il a 2 modes principaux :

- mode interactif
- mode console

Il accepte une dizaine de commandes et analyse leur syntaxe.

### Commandes acceptées par l'interpréteur

{} : facultatif
"+" : l'argument doit être donné au moins une fois

- charger un fichier : 				 "load <nom_de_fichier> {<adresse>}"
- quitter : 						 "exit"
- afficher des zones de la mémoire : "disp mem <plage>+"
- afficher le map mémoire :			 "disp mem map"
- afficher des registres :			 "disp reg <registre>"
- décoder et afficher du code obj :	 "disasm <plage>+"
- modifier un registre :			 "set reg <registre> <valeur>"
- modifier un octet en mémoire :	 "set mem <type> <adresse> <valeur>"
- évaluer un registres : 			 "assert reg <registre> <valeur>"
- évaluer un octet en mémoire : 	 "assert byte <adresse> <valeur>"
- évaluer un mot en mémoire :		 "assert word <adresse> <valeur>"
- exécuter à partir d'un adresse :	 "run {<adresse>}"
- exécuter en mode pas à pas :		 "step" "step into"
- ajouter des points d'arrêt :		 "break add <adresse>+"
- supprimer des points d'arrêt :	 "break del <adresse>+|all"
- afficher les points d'arrêts :	 "break list"

commandes spécifiques à l'exécution en mode script :

- donner la main à l'utilisateur :	 "debug"
- revenir à l'exécution du script :	 "resume"

