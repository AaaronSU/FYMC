#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 <config_file>"
    exit 1
fi

config_file="$1"

if [ ! -f "$config_file" ]; then
    echo "Fichier de configuration $config_file introuvable"
    exit 1
fi

cd vm && make compile

bin/vm "../$config_file"
make clean && cd ..
