# Méthodes 
Un certain nombre de méthodes peuvent être appelées par le langage, et un certain nombre d'attributs peuvent être regardés durant l'execution du programme

| Méthodes    | !.en_string | !.constructeur | !.modifie       | !.index   | !.copie | !.argument                                                                                          |
| ----------- | ----------- | -------------- | --------------- |:--------- | ------- | --------------------------------------------------------------------------------------------------- |
| Spécifique  | tout        | tout           | itérables       | itérables | tout    | tout                                                                                                | 
| Arguments   | non         | (...)          | (index, valeur) | (index)   | non        | (nom_fonction)                                                                                      |
| Spécificité |             |                |                 |           |         | N'est pas appelée pour la construction de nouvel objet (c'est à dire pas pour !classe.constructeur) |

- `!.en_string`
	Renvoie la représentation en _char_ de l'objet.
- `!.constructeur` 
	Permet de construire l'objet. Est appelée lors de `classe(args)`.
- `!.modifie` 
	Permet de modifier en un emplacement _index_ l'itérable, avec _valeur_.
- `!.index` 
	Renvoie la valeur indiquée a l'_index_ de l'itérable.
- `!.copie`
	Effectue une copie de l'objet lors d'une assignation de variable (par exemple).  Si la méthode n'existe pas, l'objet n'est pas copié et est passé tel quel. (Il peut donc être modifié à la suite).

# Itérables
Voir [[Itérables]].
Un certain nombre d'actions sont possibles avec des itérables,  comme la recherche par indices.
Les itérables sont définis avec l'attribut `.iterable` qui est spécifié comme un _booléen_, c'est à dire **_(0, 1)_**;




