#Requirements: libnotify-dev
FLAGS := "$(shell pkg-config --cflags --libs libnotify)"

all:
	echo ${FLAGS}
ifeq ("${FLAGS}", "")
	@echo Libnotify not found. Compiling without libnotify
	gcc -D DSTNOTIFY=1 -Wall isdstd.c -o isdstd
else
	@echo Libnotify found, compiling with libnotify
	gcc -Wall isdstd.c -o isdstd `pkg-config --cflags --libs libnotify`
endif
