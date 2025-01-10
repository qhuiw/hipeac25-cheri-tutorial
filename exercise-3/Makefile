normal: main.c f.c
	cc -g main.c f.c -o normal

compart: main.c f.c
	cc -g f.c -shared -o f.so
	cc -g main.c f.so -o compart -Wl,-rpath=.
	elfctl -e +cheric18n ./compart
