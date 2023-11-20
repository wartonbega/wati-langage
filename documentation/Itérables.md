Les itérables sont un type d'objets qui vérifient un certain nombre de demandes.

1) Ils doivent avoir un attribut `.iterable = 1`. C'est ce qui distingue les itérables du reste des objets. L'attribut peut éventuellement être définis comme une constante.
2) Ils doivent avoir un attribut `.taille` qui tient compte de leur taille (dynamiquement).
3) Ils doivent avoir un attribut `.conflits` de type _list_ ou _tableau_ qui permet de résoudre les conflits d'itération voir [[Itérables#Résoudre les conflits d'itération|résoudre les conflits]].
4) Ils ont des méthodes d'accession et de remplacement et d'itération: 
	- `!.index(i)`
	- `!.modifie(index, valeur)`
	- `!.suivant()`
	- `!.a_la_fin()`
	- `!.debut()`
	- `!.dernier_etat()`
## Les tableaux en wati
Il existe un type d'objet (implémenté en c++) semblables aux listes : les tableaux.
Ce sont des tableaux dynamiques à complexité amortie

## Itérable de préférence pour la définition de listes
[NOTE]: Disponible avec la librairie standard en c++ : _'basic.cpp'_

Lors de la création de listes implicite (`l = (1, 2, 3);`), on peut choisir si l'itérable créé sera une liste ou un tableau : 
`!classe_defaut_liste("tableau")` pour que ce soit les _tableau_ ou encore `!classe_defaut_liste("list")` pour que ce soit les _list_.

## Itérer

### Résoudre les conflits d'itération
Dans certains cas de figure, on peut avoir un code comme ceci : 
```wati
l = (1, 2, 3, 4);
pour i dans l fait
	pour j dans l fait
		!print(i, " fois ", j " fait ", i*j, "\n")
	fin
fin
```
Un tel code renvoie ceci :
```
>1 fois 1 fait 1
>1 fois 2 fait 2
>1 fois 3 fait 3
>1 fois 4 fait 4
```
puis s'arrête.

Ici, l'itérateur s'arrête au bout de 4 itérations sur la liste car il y a un conflit entre les deux boucles. La première lance l'itération, la deuxième la recommence depuis le début, et a la fin de la première boucle, l'itérateur considère avoir finis d'itérer, ainsi s'arrêtent les deux boucles.

Il faut donc gérer le conflit sur l'itérateur, l'algorithme est simple, lors de l'initialisation de l'itération, on rajoute au bout de la liste de résolution le dernier élément pris, et quand l'itération est terminée le dernier élément de la liste est remis comme premier.
C'est traité internement par le wati, par les fonctions `!initialise_boucle`et `!termine_boucle`.
À la fin de la boucle, `!termine_boucle` appel la méthode `!iter.dernier_etat` qui remet l'itérateur dans son dernier état connu.
