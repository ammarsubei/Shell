all: myShell.c
	gcc -o myShell myShell.c
clean:
	rm myShell 