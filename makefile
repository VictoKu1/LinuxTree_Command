all: stree
stree: stree.c
	gcc -o stree stree.c
clean:
	rm -rf stree





