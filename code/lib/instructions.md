# Dictionnaire d'instructions

## Syntaxe

- Définir la syntaxe est sans doute le problème le plus important

### commande/encoding nom_inside_IT_block/nom_outside masque_opcode valeurs_opcode nb_params_reg masque_reg+ masque_imm

- Tout est codé en big endian (!)

- Le premier registre sera toujours le registre de destination

Exemples :

```
mov_imm/T1 MOV/MOVS 
add_imm/T1 ADD/ADDS 1111111000000000 0001110000000000
add_imm/T2 ADD/ADDS 1111111000000000 0001110000000000
add_imm/T3 ADD 11111011111000001000000000000000 11110001000000000000000000000000
add_imm/T4 ADDW 11111011111100001000000000000000 11110010000000000000000000000000
```