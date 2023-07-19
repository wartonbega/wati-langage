# le Wati langage

Le wati langage est un langage de programmation entièrement (presque, et lachez-moi les basques) en français.

## Installation

### Macos
Désipez le fichier téléchargé dans un répertoire.
Acceder à ce fichier dans un terminal avec la commande :
`cd chemin/au/dossier`.

Prérequis : 
 - home brew : [installation](https://docs.brew.sh/Installation)
 - gcc : dans un terminal : `brew install gcc`
 - SDL2 : dans un terminal
    - `brew install sld2`
    - `brew install sld2_image`
    - `brew install sld2_ttf`
 - Makefile : `brew install make`

Installation : 
 - utiliser la commande `sudo chmod u+x install_macos.sh` pour rendre le fichier 'install_macos.sh' executable
 - executer `./install_macos.sh`

Finalement, exécuter les fichier en wati avec la commande
`wati nom_du_fichier.wati`.
