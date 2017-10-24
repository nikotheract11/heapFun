hp:
	@echo " Compile hp_main ...";
	gcc -g3 -lm -I ./include/ -L ./lib/ -Wl,-rpath=./lib/ ./examples/hp_main.c ./src/heap_file.c -lbf -o ./build/runner -O2

bf:
	@echo " Compile bf_main ...";
	gcc -I ./include/ -L ./lib/ -Wl,-rpath=./lib/ ./examples/bf_main.c -lbf -o ./build/runner -O2
