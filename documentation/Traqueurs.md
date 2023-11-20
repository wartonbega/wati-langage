Les traqueurs sont des variables de type 'traqueur', qui gardent en mémoire un nom de variable et une table de variable dans un bout de code local.

# Définition d'un traqueur
On le définit avec l'opérateur de définition de traqueur '&'.

Exemple : 
```wati
a = "Test !";
t = &a; // On définit ici le traqueur 't' qui traque le nom 'a' dans ce bloque 
		// de code, ou un bloc de code parent
```

Un traqueur peut aussi traqueur un index dans un itérable : 
```wati
l = (1, 2, 3, 4, 5);
t = &l[1];
```
Ici, 't' vas traquer le premier élément de cette liste

# Modification du contenu 
On peut modifier le contenu d'un traqueur avec la syntaxe : 
```wati
*t = content;
```
où `t` est un traqueur et `content` la valeur qu'on veut assigner.

Exemple : 
```wati
var = "Bleu";
t = &var;
!println(t); // Vas écrire '<traqeur &=var *="Bleu">'
*t = "Rouge";
!println(t) // Vas écrire '<traqeur &=var *="Rouge">'
```

Idem, on peut modifier la valeur dans un liste avec cette syntaxe 
```wati
l = (1, 2, 3, 4, 5);
t = &l[1];
*t = 5;
!println(l) // '[1, 5, 3, 4, 5]'
```
