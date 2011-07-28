main: darkelfRead.o darkelf.o main.o
	gcc -Wall darkelfRead.o darkelf.o main.o -o main  

main.o: main.c
	gcc -Wall -c main.c

darkelf.o: darkelf.c
	gcc -Wall -c darkelf.c
    
darkelfRead.o: darkelfRead.c
	gcc -Wall -c darkelfRead.c

clean:
	rm *.o
	rm main
