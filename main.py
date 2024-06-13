import os
import generator2
import argparse

output_name = "a"
filename = ""

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="Compilateur pour Wati-langage",
        description="Compile le wati-langage",
        epilog="Merci de vous référer à la documentation du Wati-langage",
        
    )
    parser.add_argument("filename")
    parser.add_argument("-o", "--output", required=False)
    parser.add_argument("-i", "--informations", choices=["o", "n"], required=False)
    args = parser.parse_args()
    if args.informations == None or args.informations == "n":
        generator2.INFORMATIONS = False
    else:
        generator2.INFORMATIONS = True
    if args.output != None:
        output_name = args.output
    else:
        output_name = args.filename.removesuffix(".wati")
    filename = args.filename
    generator2.run_code(filename, output_name + ".asm")
    print("Compilé en assembleur !")
    os.system(f"nasm -f macho64 -o {output_name}.o {output_name}.asm")
    print("Liage de l'assembleur !")
    os.system(f"gcc -e _start {output_name}.o -lc -m64 -o {output_name}.out -Wl,-no_pie")
    os.system(f"rm {output_name}.asm")
    os.system(f"rm {output_name}.o")
    print(f"Écrit : {output_name}.out")