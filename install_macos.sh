echo "compilation ..."
make build

user=$(whoami)
echo "copie dans /Users/$user/Library/wati"
sudo cp ./bin/wati /Users/$user/Library/wati