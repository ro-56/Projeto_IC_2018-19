objects = leitura.o main.o individuo.o genetic.o results.o misc.o graph.o
TxtResults = graphBest.txt graphAverage.txt

run: $(objects)
	gcc $(objects) -o Run -Wall -lm

leitura.o: leitura.c defs.h
	gcc leitura.c -c -Wall -lm

main.o: main.c defs.h 
	gcc main.c -c -Wall -lm

individuo.o: individuo.c individuo.h defs.h 
	gcc individuo.c -c -Wall -lm

genetic.o: genetic.c genetic.h defs.h
	gcc genetic.c -c -Wall -lm

results.o: results.c defs.h
	gcc results.c -c -Wall -lm

misc.o: misc.c defs.h
	gcc misc.c -c -Wall -lm

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