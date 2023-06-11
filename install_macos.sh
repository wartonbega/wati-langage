
if [ ! command -v python3.10 &> /dev/null ]; then
    echo "<python3.10> n'as pas été installé. Installez python3.10 pour pouvoir compiler les programmes"
    exit 1
fi

python3.10 depedences.py
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

sudo cp -R $LIBS /usr/local/lib/wati/
sudo cp -R $CPP_LIBS /usr/local/lib/wati/

echo ""
echo "wati-langage installé !"
echo "pour exécuter un programme : "
echo "  wati nom_du_programme.wati"