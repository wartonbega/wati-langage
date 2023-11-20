# Première utilisation
Après avoir installé avec succès le _wati-langage_, on vas d'écrire notre premier programme : un simple "Hello World!" pour commencer.

Dans un fichier ('test.wati' par exemple), écrire cette simple ligne de code : 
```wati
!imprimeln("Hello World!");
```
Le retour devrait donner 'Hello World!'.
Que s'est-il passé ?

Pour appeler une fonction en wati, on utilise l'opérateur _'!'_ suivis du nom de la fonction et finalement de ses arguments entre parenthèses, séparés par des virgules.
La fonction !imprimeln (imprime ligne) est inclue dans la librairie standard qui est normalement automatique importée. Elle est aussi disponible en tant que !println. Elle affiche l'unique argument avec un retour à la ligne.
On peut obtenir un résultat similaire en utilisant la fonction !imprime ou !print mais en rajoutant en argument un retour à la ligne ("\\n") : 
`!imprime("Hello World!", "\n")`