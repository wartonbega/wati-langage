# Note de version 
Wati-langage : _mise-à-jour listes, fonctions et thread_

## Listes intégrées
Les listes peuvent maintenant être déclarée entre des parenthèses (`l = (0, 1, 2, 3, 4, 5)`).

Elles invoquerons une nouvelle variable de type _list_, qui doit être au préalable inclue depuis la librairie standard.

### Syntaxe
On peut déclarer ouvertement des listes.
exemple 1 : 
```wati
inclue "std.wati";
l = (1, 2, 3);
!println( l[0] )
```

exemple 2 : 
```wati
inclue "std.wati";
l = (1, 2, (3, 4));
!println(l[2][0])
```

## Type-fonction
Cette version ajoute le type inbuild 'fonction' qui fait que on peut passer comme variable une fonction. On peut ensuite appeler cette variable à la manière d'une fonction normale.

### Syntaxe
exemple 1 : 
```wati
fonction test () fait
    renvoie 1;
fin

a = !test;
!a()
```

exemple 2 :
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

exemple 3 :
```wati
fonction f (x) fait
    renvoie x ^ 2; 
fin

g = !f;
!g(3)
```

## Déclaration de liste entre parenthèses
Les listes peuvent être déclarées avec des parenthèses. **Attention**, le fichier `list.wati` doit être inclue, soit séparément soit en incluant la librairie standard.

exemple : 
```wati
inclue "./lib/std.wati";
l = (0, 1, 2, 3, 4, 5);
!println(l)
```

## Les threads (tache)
Les threads sont ajoutés avec le mot-clé `tache`, qui prend en argument soit : 
- une fonction (variable de type '_fonction_')
- une liste avec le premier élément la fonction à appeler, et les éléments suivants ses arguments

La librairie standard donne accès à une fonction `!thread`, et à une classe (type) `future`.

On peut appeler la fonction `!thread`, avec comme premier argument un objet de type future et comme second argument la fonction que on veut appeler en paralèle.

La classe _future_ propose de joindre le thread où elle est utilisée et de contenir le résultat de la fonction appelée en paralèle.

Un maximum de 4 thread est pour le moment disponible par porgramme. Chaques thread lancés dans un bloc de code (fonction, fichier principal) est joint à la fin de l'execution.

### Syntaxe
exemple 1, en utilisant la librairie standard :
```wati
inclue "./lib/std.wati";

fonction test () fait
    pour i allant de 0 a 3 fait
        !dors(1)
        !println(i)

    fin
    renvoie "salut ";
fin

future1 = future();
tache (!thread, future1, !test);

!future1.join()

!print(!future1.rec_resultat())

!println("tout le monde")
```

exemple 2 : 
```wati

fonction test () fait
    pour i allant de 0 a 3 fait
        !dors(1)
        !println(i)

    fin
    renvoie "salut ";
fin

tache !test;
pour i allant de 0 a 100 fait
    !print(i, "\n")
fin
```

exemple 3 :
```wati
fonction test (x) fait
    pour i allant de 0 a x fait
        !dors(1)
        !println(i)

    fin
    renvoie "salut ";
fin


tache (!test, 5);
pour i allant de 0 a 100 fait
    !print(i, "\n")
fin
```
