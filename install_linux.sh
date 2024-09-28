#!/bin/bash
./compile_std.sh

python3 -m pip install typing_extensions

python3 ./config/config.py

echo "pour compiler : python3 main.py [nom_du_fichier.wati]"
echo "Le premier test à faire est test3.wati (test1 et test2 sont pas très intéressants)"