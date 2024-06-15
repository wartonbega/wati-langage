import os, sys
import generator2
import argparse

output_name = "a"
filename = ""
rm_asm = True



if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="Compilateur pour Wati-langage",
        description="Compile le wati-langage",
        epilog="Merci de vous référer à la documentation du Wati-langage",
        
    )
    parser.add_argument("filename")
    parser.add_argument("-o", "--output", required=False)
    parser.add_argument("-i", "--informations", choices=["o", "n"], required=False)
    parser.add_argument("-a", "--asm", const=True, nargs="?")
    args = parser.parse_args()
    if args.informations == None or args.informations == "n":
        generator2.INFORMATIONS = False
    else:
        generator2.INFORMATIONS = True
    if args.output != None:
        output_name = args.output
    else:
        output_name = args.filename.removesuffix(".wati")
    if args.asm != None:
        rm_asm = False
    filename = args.filename
    generator2.run_code(filename, output_name + ".asm")
    print("Compilé en assembleur !")
    print(f"nasm -f{'macho64' if sys.platform == 'darwin' else 'elf64'} -o {output_name}.o {output_name}.asm")
    os.system(f"nasm -f{'macho64' if sys.platform == 'darwin' else 'elf64'} -o {output_name}.o {output_name}.asm")
    print("Liage de l'assembleur !")
    if sys.platform == 'darwin':
        print(f"gcc {('-e ' + generator2.starting_label) if sys.platform == 'darwin' else ''} {output_name}.o -lc -m64 -o {output_name}.out -Wl,-no_pie")
        os.system(f"gcc -e {generator2.starting_label} {output_name}.o -lc -m64 -o {output_name}.out -Wl,-no_pie")
    else:
        print(f"gcc {output_name}.o -L/usr/lib/gcc/x86_64-linux-gnu/9/ -lc -m64 -o {output_name}.out -Wl,-no_pie")
        os.system(f"gcc -e {generator2.starting_label} {output_name}.o -lc -m64 -o {output_name}.out -Wl,-no_pie")
        
    if rm_asm:
        os.system(f"rm {output_name}.asm")
    os.system(f"rm {output_name}.o")
    print(f"Écrit : {output_name}.out")