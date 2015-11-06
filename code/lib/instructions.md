# Dictionnaire d'instructions

## Syntaxe

- Définir la syntaxe est sans doute le problème le plus important

### commande/encoding nom_inside_IT_block/nom_outside masque_opcode valeur_opcode nb_params_reg masque_reg+ masque_imm+

avec :

### masque_reg+ = (masque_rd) (masque_rn) (masque_rm)
### masque_imm = (imm) (imm) ...

- Les (imm) sont rangés dans le bon ordre, qui n'est pas celui de l'instruction (!)
- Le premier registre sera toujours le registre de destination
- Les instructions en 16 bits vont de 0 à 15, celle en 32 bits vont de 0 à 31

## Endianness (!) :

- Nous avons commencé le dictionnaire en little endian, tout ce qui concerne l'OPCODE est donc en LITTLE ENDIAN ALIGNE (alors que les fichiers .o sont codés en big endian)
- On va coder les masques des PARAMETRES en BIG ENDIAN ALIGNE (plus simple, on peut utiliser la numérotation de la doc)

## Equivalence numérotations doc pour les instructions 32 bits (Little Endian Aligné) :

15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0

## Exemples :

```
mov_imm/T3 MOV/MOVW FBF08000 F2408000 1 
add_imm/T1 ADD/ADDS 1111111000000000 0001110000000000
add_imm/T2 ADD/ADDS 1111111000000000 0001110000000000
add_imm/T3 ADD 11111011111000001000000000000000 11110001000000000000000000000000
add_imm/T4 ADDW 11111011111100001000000000000000 11110010000000000000000000000000
```