all:
	ftw1 ftw2 stree
ftw1:
	gcc -o ftw1 ftw1.c
ftw2:
	gcc -o ftw2 ftw2.c
stree: 
	gcc -o stree stree.c
clean:
	rm -rf  ftw1 ftw2 stree
