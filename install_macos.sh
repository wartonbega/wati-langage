if [ ! command -v make &> /dev/null ]; then
    echo "<make> n'as pas été installé. Installez make pour pouvoir compiler le programme"
    exit 1
else
    echo "compilation ..."
    make build

    echo "compilation des librairies ..."
    make cpp_lib_macos
fi

user=$(whoami)
echo "copie dans /usr/local/bin"
sudo cp ./bin/wati /usr/local/bin

echo "création des répertoires à librairies"

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

sudo cp -R $LIBS /usr/local/lib/wati/lib
sudo cp -R $CPP_LIBS /usr/local/lib/wati/cpp_lib


echo "wati-langage installé !"
echo "pour exécuter un programme : "
echo "\twati nom_du_programme.wati"