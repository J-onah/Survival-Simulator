CFLAGS = -Wall -Werror -ansi -pedantic
OFILES = helper.o random.o inventory.o item.o animals.o actions.o game.o

main: $(OFILES)
	gcc $(OFILES) -o SurvivalSim.exe $(CFLAGS)
	gcc $(OFILES) -o SurvivalSim.o $(CFLAGS)

helper.o : helper.c
	gcc -c helper.c -o helper.o $(CFLAGS)

random.o: random.c
	gcc -c random.c -o random.o $(CFLAGS)

inventory.o: inventory.c
	gcc -c inventory.c -o inventory.o $(CFLAGS)

item.o: item.c
	gcc -c item.c -o item.o $(CFLAGS)

game.o: game.c
	gcc -c game.c -o game.o $(CFLAGS)

actions.o: actions.c
	gcc -c actions.c -o actions.o $(CFLAGS)

animals.o: animals.c
	gcc -c animals.c -o animals.o $(CFLAGS)

clean:
	rm $(OFILES)
	rm SurvivalSim.*
	