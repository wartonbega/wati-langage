user=$(whoami)
echo "copie dans /usr/local/bin"
sudo cp ./bin/wati /usr/local/bin

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
