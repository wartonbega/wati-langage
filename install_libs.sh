# On copie l'ensemble des fichiers dans /usr/local/lib/wati/lib

echo "installation des librairies"

FILE=/usr/local/lib/wati
if [ ! -d "$FILE" ]; then
    sudo mkdir /usr/local/lib/wati
fi

LIBS=./lib
if [ ! -d "$LIBS" ]; then
    echo "répertoire non-trouvé : $LIBS, verifiez les dépendances"
    exit 1
fi

CPP_LIBS=./cpp_lib
if [ ! -d "$CPP_LIBS" ]; then
    echo "répertoire non-trouvé : $CPP_LIBS, verifiez les dépendances"
    exit 1
fi

sudo cp -R $LIBS /usr/local/lib/wati/
sudo cp -R $CPP_LIBS /usr/local/lib/wati/