all:
	#gcc `pkg-config --cflags gtk+-3.0` test2.c -o test2 `pkg-config --libs gtk+-3.0`
	#gcc `pkg-config --cflags gtk+-3.0` gtk3.c -o gtk3 `pkg-config --libs gtk+-3.0`
	gcc isdstd.c -o isdstd