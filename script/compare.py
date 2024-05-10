import sys
from colorama import init, Fore

init(autoreset=True)

def compare_binary_files(file1_path, file2_path):
    try:
        with open(file1_path, "rb") as file1, open(file2_path, "rb") as file2:
            data1 = file1.read()
            data2 = file2.read()

        if data1 == data2:
            print(Fore.GREEN + "Les fichiers binaires sont identiques.")
        else:
            raise ValueError("Les fichiers binaires sont différents.")
    except FileNotFoundError:
        print(Fore.RED + "Erreur : Fichier non trouvé.")
    except ValueError as e:
        print(Fore.RED + f"Erreur : {str(e)}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <chemin_vers_fichier1> <chemin_vers_fichier2>")
        sys.exit(1)

    file1_path = sys.argv[1]
    file2_path = sys.argv[2]

    compare_binary_files(file1_path, file2_path)
