Le compilateur du wati-langage !

Un langage de programmation tout en français.

Bien qu'écrit en python, et sans optimisation de code, le wati langage peut être compilé : 
`$ python3 main.py sourcefile.wati -o output`

Puis : `$ ./output.out`.

Pour compiler une bibliothèque wati : `$ python3 main.py sourcefile.wati -s -I`.
L'argument `-s` permet de créer uniquement un fichier `.o` liable avec gcc, et `-I` permet de rendre le fichier indépendant, ce qui évite les redéfinitions pour le linker.

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

Le  type général d'une liste est `<[_]type>`. Par exemple :
```
<[10] ent> liste;
<[_]ent> dup = liste;
```

Définition d'une variable simple : 
```
<ent> x = 1;
```

ou : 

```
x = 1; // le type peut être implicite car le type de retour pour l'assignation est connu
```

Pour définir un caractère : utiliser la syntaxe avec des guillemets simples : `'a'`.

La syntaxe avec des guillemets doubles invoque une chaine de caractère de type `<[_]chr>` : 
```
<[_]chr> a = "chaine";
```

## La conversion de type

On peut faire des castings en wati pour convertir une valeur d'un certain type en un autre tyoe, et en écrasant les données qui ne sont pas comprises dans la taille du type.

Par exemple :
```
<ent> x = 109349185;
<chr> y = <chr>x; // Y vaut 'A' : le premier octet de x est gardé
```
Deux syntaxes possibles : 
`y = <chr>x` ou `y = <chr>(x)`

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

On peut aussi avoir une valeur conditionnelle : 
```
<bool> x = Vrai;
!println(? "Vrai" si x sinon "Faux");
```

Avec la syntaxe `? val2 si cond sinon val2`

## Boucles

Deux types de boucles, les boucles `tant que ... fait ... fin` et `pour . dans ... fait ... fin`.

Boucles `tant que` sont des classiques boucles conditionnelles. Exemple : 
```wati
<ent> i = 0;
tant que i < 10 fait
 !println(i);
 i = i + 1;
fin
```

Les boucles `pour . dans` prennent après le 'dans' un objet de type itérateur.

Ces derniers sont définis en rajoutant `IT` après le nom de classe. Ils doivent avoir au minimum 3 méthodes : `.debut()`, `.suivant()`, `.fin()`.
 - `.debut()` initialise l'itérateur et ne doit renvoyer rien
 - `.suivant()` doit renvoyer la valeur suivant prise par la variable de boucle
 - `.fin()` doit renvoyer un booléen indiquant soit la boucle doit se stopper.
Exemple de création d'un itérateur :
```wati
classe plage IT contient
   <ent> debut;
   <ent> fin;
   <ent> courant;

   methode <*plage> constructeur (<ent> deb, <ent> fin) fait
       soit = <*plage>!mallom(24); // Fait 24 octets de long, à raison de 8 octets par entiers
       soit.debut = deb;
       soit.fin = fin;
       soit.courant = deb;
   fin

   methode <rien> debut () fait
       soit.courant = soit.debut;
   fin

   methode <ent> suivant () fait
       s = soit.courant;
       soit.courant = soit.courant + 1;
       renvoie s;
   fin

   methode <bool> fin () fait
       renvoie soit.courant == soit.fin;
   fin
fin
```
Enfin on peut utiliser cet itérateur dans une boucle : 
```wati
pour i dans plage(0, 10) fait
    !println(i);
fin
```

# Erreurs fréquentes et incompréhensible si on a pas codé le wati-langage : 

`pop from empty list` : J'ai pas implémenté encore

`Not EOF and not rulled` : Y'a une erreur de syntaxe grossière dans ton code, si grossière que j'ai même pas envie de te dire où elle est.

# Bibliothèque standard

On peut importer la bibiliothèque standard :
```
inclue "std.wati";
```
Attention, les chemins relatifs ne sont pas implémentés.

La bibliothèque standard possède des fonctions de bases comme :
 - `<rien>print(<...>)` : Affiche ce qui est demandé. (Attention les types exotiques ne sont pas implémentés).
 - `<rien>println(<...>)` : Affiche ce qui est demandé avec un retour à la ligne.
 - `<rien>exit(<ent> code)` : Sort de l'execution du programme.
 - `<*rien>mallom(<ent> taille)` : Alloue un bloc de mémoire de taille `taille` (en octet).
 - `<rien>libere(<*rien> ptr)` : Libère un bloc de mémoire alloué par `mallom`.
 - `<rien>erreur(<[_]chr> message)` : Affiche le message d'erreur et sort du programme.


La bibliothèque standard définit des types de bases comme :
 - `<ent32>` : des entiers sur 32 bits (4 octets)
 - `<ent16>` : des entiers sur 16 bits (2 octets)
 - `<ent8>`  : des entiers sur 8 bits (1 octets)

Les 3 types précédents n'acceptent des opérations qu'avec `<ent>`. Ainsi il faut caster le type du second opérateur vers `ent` pour effectuer le calcul.

Exemple : 
```
<ent8> x = <ent8> 12;
<ent8> y = <ent8> 10;
z = x + <ent> y; // Le type de z est donc <ent8>
```

La bibliothèque standard définit des classes comme :
 - `liste<type>` : une liste simplement chaînée, de taille dynamique. Ses méthodes : 
   - `<rien>ajoute(<type> x)` : ajoute l'élément `x` à la fin de la liste.
   - `<rien>print()>` : affiche la liste, si `print(<type>)` existe.
   - `<rien>println()>` : affiche la liste et un retour à la ligne, si `print(<type>)` existe.
   - `<type>index(<ent> i)` : renvoie la valeur à l'index `i`.
   - `<type>pop(<ent> i)` : enlève l'élément à l'index `i` et renvoie son contenu
     On notera que '`liste<type>` est un itérateur.
   - Exemple :
     ```
     <liste<ent>>l = <ent>liste(); // Définit une liste d'entiers
     !l.ajoute(1);
     !l.ajoute(2);
     !l.ajoute(3);
     !l.println();
     !println(!l.index(1)); // affiche '2'
     ```
 - `pile<type>` : une pile classique, de taille dynamique. Ses méthodes : 
   - `<rien>ajoute(<type> x)` : ajoute l'élément `x` sur le haut de la pile.
   - `<type>pop()` : enlève et renvoie l'élément sur le haut de la pile.
     Contrairement à la liste chainée, la pile n'est pas un itérateur
