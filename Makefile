# Generated Makefile, Just a template. You can modify me

Var0 = g++

.DEFAULT_GOAL := Link
run:
	
src/main.cpp.out: src/main.cpp
	@echo -e "[\e[1;32m Compiling file src/main.cpp 0.0%... \e[0m]"
	@ccache ${Var0} -c src/main.cpp -o build/main.cpp.out --std=gnu++23 -lscok -I include
Link: src/main.cpp.out
	@echo -e "[\e[1;32m Linking 100%... \e[0m]"

	@g++ build/main.cpp.out -o build/sender -largumentsea -lscok
all:	run	src/main.cpp.out	Link
install:
	sudo mv build/sender /usr/bin