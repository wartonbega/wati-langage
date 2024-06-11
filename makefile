
VAR = "test.asm"

run:
	yasm -f macho64 -o test.o $(VAR)
	ld -e _start -static test.o  

runC:
	nasm -f macho64 -o test.o $(VAR)
	gcc -e _start test.o -lc -m64 -o a.out -Wl,-no_pie
	
clear:
	rm test.o 
	rm a.out