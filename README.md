Le compilateur du wati-langage !

Bien qu'écrit en python, et sans optimisation de code, le wati langage peut être compilé : 
`$ python3 main.py sourcefile.wati -o output`

# Syntaxe
La syntaxe a bien changé, puisque le wati est maintenant complètement typé.

## Les types
Les types de base : 
 - `ent` : les entiers signés, codés sur 8 octets
 - `bool` : les boolées, codés sur 1 octet
 - `chr` : les caractères, codés sur 1 octet
 - `*type` : les pointeur vers un type, codés sur 8 octets
 - `liste[type]` : les listes d'un type, codés sur 8 octets (les listes de base sont des pointeurs vers un bloc contigue de mémoire).
 - `rien` : rien (comme du void)

Les types s'utilisent entre chevrons `<ent> x = 1;`...

Exemple :
 - `<ent>` : un entier
 - `<[_]chr>` : le type général d'une liste de caractères (peut être utilisée comme chaîne de caractères)
 - `<*rien>` : un pointeur non typé
 - `<*chr>` : un pointeur vers un caractère
 - `<*<chr>>` : même chose


Mais avec l'ajout des types vient la modification de la syntaxe d'appel des fonctions.
## Les fonctions
Pour déclarer une fonction :
```wati
fonction <ent> carre (<ent> x) fait
  renvoie x^2;
fin
```
Le type de retour est indiqué avant le nom de la fonction, et les types des arguments avec le nom de arguments.

Le type de retour de la fonction ne permet pas de la différentier, si bien que `fonction <ent> toto () ...` et `fonction <bool> toto () ...` vont se redéfinir si les arguments sont les mêmes.

Pour appeler une fonction : `!fonction(args...)`

## Les variables
Deux type d'implémentation d'une variable, si c'est une `liste` ou une variable simple que l'on définit : 

Définition d'une liste de 10 éléments : 
```
<[10] ent> liste; // Pas d'initialisation
```

Définition d'une variable simple : 
```
<ent> x = 1;
```
ou 
```
x = 1; // le type peut être implicite car le type de retour pour l'assignation est connu
```

## Les tests conditionnels
Par l'exemple : 
```
si cond1 fait
  ...
fin sinonsi cond2 fait
  ...
fin sinon fait
  ...
fin
```

## La conversion de type
On peut faire des castings en wati pour convertir une valeur d'un certain type en un autre tyoe, et en écrasant les données qui ne sont pas comprises dans la taille du type.

Par exemple :
```
<ent> x = 109349185;
<chr> y = <chr>x; // Y vaut 'A' : le premier octed de x est gardé
```
Deux syntaxes possibles : 
`y = <chr>x` ou `y = <chr>(x)`

# Erreurs fréquentes et incompréhensible si on a pas codé le wati-langage : 
`pop from empty list` : J'ai pas implémenté encore

`Not EOF and not rulled` : Y'a une erreur de syntaxe grossière dans ton code, si grossière que j'ai même pas envie de te dire où elle est.
