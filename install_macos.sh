if ! command -v make &> /dev/null
then
    echo "<make> n'as pas été installé. Installez make pour pouvoir installer le programme"
    exit
else
    echo "compilation ..."
    make build
fi

user=$(whoami)
echo "copie dans /usr/local/bin"
sudo cp ./bin/wati /usr/local/bin