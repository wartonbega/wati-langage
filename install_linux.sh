echo "compilation ..."
make build

user=$(whoami)
echo "copie dans /usr/bin/wati"
sudo cp ./bin/wati /usr/bin/wati