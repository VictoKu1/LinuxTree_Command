all: ftw1 ftw2 stree
ftw1: ftw1.c
	gcc -o ftw1 ftw1.c
ftw2: ftw2.c
	gcc -o ftw2 ftw2.c
stree: stree.c
	gcc -o stree stree.c
clean:
	rm -rf  ftw1 ftw2 stree





