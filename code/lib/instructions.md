# Dictionnaire d'instructions

## Syntaxe

### commande/encoding nom_inside_IT_block/nom_outside masque_opcode/valeur_opcode nb_params_reg/masque_reg+ nb_imm/masque_imm+ nb_extra_params/masque_extra+

avec :

### masque_reg+ = (masque_rd)/(masque_rn)/(masque_rm)
### masque_imm+ = (imm)/(imm)/ ...
### extra_params = S, cond, N, J1, J2, ... ils sont classé dans le sens croissant de numérotation LITTLE ENDIAN (voir ci-dessous)
### masque_extra+ = (ep_1)/(ep_2)/ ... ou plus complexe (à définir en fonction des besoins)

### Les masques sont de la forme i:j ou juste i

- Les (imm) sont rangés dans le bon ordre, qui n'est pas celui de l'instruction (!)
- Le premier registre sera toujours le registre de destination
- Les instructions en 16 bits vont de 0 à 15, celle en 32 bits vont de 0 à 31

### Spécifier le nombre permet de faire une vérification

- On suppose qu'il y a toujours 2 noms, s'il n'y a pas 2 noms, une chaîne vide est mise dans nom_out

## Endianness (!) :

- Nous avons commencé le dictionnaire en little endian, tout ce qui concerne l'OPCODE est donc en LITTLE ENDIAN ALIGNE (alors que les fichiers .o sont codés en big endian)
- On va coder les masques des PARAMETRES en BIG ENDIAN ALIGNE (plus simple, on peut utiliser la numérotation de la doc)

## Equivalence numérotations doc pour les instructions 32 bits (Little Endian Aligné) :

15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16

15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 15 14 13 12 11 10 "9 "8 "7 "6 "5 "4 "3 "2 "1 "0

## Exemples :

```
mov_imm/T3 MOV/MOVW FBF08000/F2408000 1/24:27 4/0:3/10/28:30/16:23 0

Il n'y a pas de paramètres extra, Rd est récupéré de 24 à 27, imm4 de 0 à 3, i en 10, imm3 de 28 à 30 et imm8 de 16 à 23
```