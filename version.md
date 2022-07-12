# Note de version 
Wati-langage : _mise-à-jour listes et fonctions_

## Listes intégrées
Les listes peuvent maintenant être déclarée entre des parenthèses (`l = (0, 1, 2, 3, 4, 5)`).

Elles invoquerons une nouvelle variable de type _list_, qui doit être au préalable inclue depuis la librairie standard.

### Syntaxe
On peut déclarer ouvertement des listes.
Example 1 : 
```wati
inclue "std.wati";
l = (1, 2, 3);
!println( l[0] )
```

Example 2 : 
```wati
inclue "std.wati";
l = (1, 2, (3, 4));
!println(l[2][0])
```

## Type-fonction
Cette version ajoute le type inbuild 'fonction' qui fait que on peut passer comme variable une fonction. On peut ensuite appeler cette variable à la manière d'une fonction normale.

### Syntaxe
Example 1 : 
```wati
fonction test () fait
    renvoie 1;
fin

a = !test;
!a()
```

Example 2 :
```wati
fonction coucou () fait
    !print("Coucou !")
fin

classe test contient
    methode constructeur (objectif) fait
        self.objectif = objectif;
    fin

    methode lancement () fait
        !self.objectif()
    fin
fin

t = test(!coucou);
!t.lancement()
```

Example 3 :
```wati
fonction f (x) fait
    renvoie x ^ 2; 
fin

g = !f;
!g(3)
```