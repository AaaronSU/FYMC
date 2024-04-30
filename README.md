Pour exécuter le fichier binaire, suivez les étapes ci-dessous dans votre terminal :

```bash
# Assurez-vous d'avoir CMake installé
# Assurez-vous d'avoir le compilateur GCC (ou un autre compilateur compatible) installé

# Naviguer vers le répertoire 'vm'
cd vm

# Créer un répertoire 'build' s'il n'existe pas
mkdir -p build

# Accéder au répertoire 'build'
cd build

# Configurer le projet avec CMake
cmake ..

# Compiler le code source avec 'make'
make

# Exécuter le fichier binaire avec le script suivant
./main ../hello_world.archy

```