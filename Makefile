all: tappa taprec taprand
tappa: tappa
	gcc -o tappa tappa.c -lpthread -lm -ldl -lncurses
taprec: taprec
	gcc -o taprec taprec.c -lpthread -lm -ldl -lncurses
taprand: taprand
	gcc -o taprand taprand.c
install: tappa taprec taprand
	mkdir -p ${DESTDIR}/usr/bin
	cp {tappa,taprec,taprand} ${DESTDIR}/usr/bin
