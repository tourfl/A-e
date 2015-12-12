# it_state

codé sur 16 bits :

mask	sign	then	else

## Exemple

```
0011	0001	0000	0001
		TE		EQ		NE
```	

## pseudo code
```
ms = it_state >> 8						0011 	0001
te = (it_state << 8) >> 8				  		0001


state = ( (ms >> 8) & ms ) & 1					0001			Then
cond = te >> (state * 4)						0000			EQ


mise à jour de l'it_state

ms = ms >> 1 							0001	1000	

it_state = (ms << 8) + te

0001	1000	0000	0001



state = ( (ms >> 8) & ms ) & 1					0000			Else
cond = te >> (state * 4)						0001			NE


mise à jour de l'it_state

ms = ms >> 1 							0000	1100	

it_state = (ms << 8) + te

0000	1100	0000	0001

``

## Contraintes






