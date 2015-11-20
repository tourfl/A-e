# Fonctionnement du module de mémoire virtuelle

## Adressage virtuel

- structure Mem_map contenant les Mem_seg plutôt qu'une liste de Mem_seg puisque on connaît d'avance les segments possibles

- structure Mem_seg

- problème des multiples de 0x1000

### Exemple

- On demande l'adresse 0x2000
- On réserve de la mémoire sur la machine hôte (malloc)

## Exemple général

- via l'interpréteur, l'adresse pour .text (on suppose qu'il n'y a pas de .rodata) demandée est 0x2000 (par défaut 0x1000 par défaut)
-