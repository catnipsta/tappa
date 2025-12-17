all: tappa taprec
tappa: tappa
	gcc -o tappa tappa.c -lpthread -lm -lncurses
taprec: taprec
	gcc -o taprec taprec.c -lpthread -lm -lncurses
install: tappa taprec
	mkdir -p ${DESTDIR}/usr/bin
	cp {tappa,taprec} ${DESTDIR}/usr/bin
