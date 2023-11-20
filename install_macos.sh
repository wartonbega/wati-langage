if [ $? -eq 1 ]; then
    exit 1
fi

if [ ! command -v make &> /dev/null ]; then
    echo "<make> n'as pas été installé. Installez make pour pouvoir compiler les programmes"
    exit 1
else
    echo "compilation ..."
    make build

    if [ $? -eq 1 ]; then
        exit 1
    fi

    echo "compilation des librairies ..."
    make cpp_lib_macos

    if [ $? -eq 1 ]; then
        exit 1
    fi
fi

#user=$(whoami)
echo "copie dans /usr/local/bin"
sudo cp ./bin/wati1.2 /usr/local/bin

echo "création des répertoires à librairies"

FILE=/usr/local/lib/wati1.2
if [ ! -d "$FILE" ]; then
    sudo mkdir /usr/local/lib/wati1.2
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

sudo cp -R $LIBS /usr/local/lib/wati1.2/
sudo cp -R $CPP_LIBS /usr/local/lib/wati1.2/

echo ""
echo "wati-langage installé !"
echo "pour exécuter un programme : "
echo "  wati1.2 nom_du_programme.wati"