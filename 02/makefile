objects = main.o leitura.o individuo.o genetic.o misc.o prints.o manipular_indiv.o graph.o
TxtResults = graphBest.txt graphAverage.txt

run: $(objects)
	gcc $(objects) -o Run -Wall -lm

main.o: main.c defs.h
	gcc main.c -c -Wall -lm

leitura.o: leitura.c individuo.h defs.h
	gcc leitura.c -c -Wall -lm

individuo.o: individuo.c defs.h
	gcc individuo.c -c -Wall -lm

genetic.o: genetic.c genetic.h manipular_indiv.h defs.h
	gcc genetic.c -c -Wall -lm

misc.o: misc.c defs.h
	gcc misc.c -c -Wall -lm

prints.o: prints.c defs.h
	gcc prints.c -c -Wall -lm

manipular_indiv.o: manipular_indiv.c manipular_indiv.h individuo.h defs.h
	gcc manipular_indiv.c -c -Wall -lm

graph.o: graph.c defs.h
	gcc graph.c -c -Wall -lm



reboot:
	make reset
	./run

reset:
	make clear
	make

clean:
	rm -f *.o *.gch

clear:
	rm -f Run *.o *.gch $(TxtResults)
	clear