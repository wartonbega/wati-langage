import os, sys
import source.generator as generator
import argparse

output_name = "a"
filename = ""
rm_asm = True
optimise_asm = False
shared_lib = False
inde = False

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="Compilateur pour Wati-langage",
        description="Compile le wati-langage",
        epilog="Merci de vous référer à la documentation du Wati-langage",
    )
    parser.add_argument("filename")
    parser.add_argument("-o", "--output", required=False, help="Précise le nom du fichier de sortie (initialement le nom du fichier de départ)")
    parser.add_argument("-i", "--informations", choices=["o", "n"], required=False, help="Affiche les message d'information")
    parser.add_argument("-a", "--asm", const=True, nargs="?", help="Empêche la supression du fichier d'assembleur (pour débugger)")
    parser.add_argument("-O", "--optimiseAsm", const=True, nargs="?", help="Optimise l'assembleur")
    parser.add_argument("-d", "--definis", nargs='+', help="Ajoute un mot-clef à définir lors de la compilation")
    parser.add_argument("-s", "--shared-lib", const=True, nargs="?", help="Compile en tant que librairie partagée")
    parser.add_argument("-I", "--independant", const=True, nargs="?", help="Permet de créer une librairie partagée indépendante (pratique pour le liage)")
    parser.add_argument("--x11", const=True, nargs="?", help="Liage avec la librairie x11")
    args = parser.parse_args()
    if args.informations == None or args.informations == "n":
        generator.INFORMATIONS = False
    else:
        generator.INFORMATIONS = True
    if args.output != None:
        output_name = args.output
    else:
        PATH = []
        filename = args.filename.removesuffix(".wati").split("/")
        base = filename[:-1]
        head: str = filename[-1]
        for i in base:
            PATH.append(i)
            PATH.append("/")
        output_name = ""
        for i in PATH:
            output_name += i
        output_name += head.removesuffix(".wati")
    if args.asm != None:
        rm_asm = False
    if args.optimiseAsm:
        optimise_asm = True
    else:
        optimise_asm = False
    if args.shared_lib:
        shared_lib = True
    if args.independant:
        inde = True
    args.definis = [] if not args.definis else args.definis
        
    filename = args.filename
    shared: list[str] = generator.run_code(filename, output_name + ".asm", optimise_asm=optimise_asm, defined=args.definis, shared_lib=shared_lib, independant = inde)
    print("Compilé en assembleur !")
    print(f"nasm -f{'macho64' if sys.platform == 'darwin' else 'elf64'} -o {output_name}.o {output_name}.asm")
    os.system(f"nasm -f{'macho64' if sys.platform == 'darwin' else 'elf64'} -o {output_name}.o {output_name}.asm")
    if not shared_lib:
        print("Liage de l'assembleur !")
        if sys.platform == 'darwin':
            shared_arg = ""
            for i in shared:
                shared_arg += f" {i}.o"
            commande = f"gcc {('-e ' + generator.starting_label) if sys.platform == 'darwin' else ''} {output_name}.o {shared_arg} -lc -R/usr/X11/lib -L/usr/X11/lib -lX11 -m64 -o {output_name}.out -Wl,-no_pie"
            print(commande)
            os.system(commande)
        else:
            print(f"gcc {output_name}.o -L/usr/lib/gcc/x86_64-linux-gnu/9/ -lc -m64 -o {output_name}.out -Wl,-no_pie")
            os.system(f"gcc -e {generator.starting_label} {output_name}.o -lc -m64 -o {output_name}.out -Wl,-no_pie")
        os.system(f"rm {output_name}.o")
        print(f"Écrit : {output_name}.out")
    if rm_asm:
        os.system(f"rm {output_name}.asm")