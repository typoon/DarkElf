main: darkelfWrite.o darkelfRead.o darkelf.o main.o
	gcc -Wall darkelfWrite.o darkelfRead.o darkelf.o main.o -o main  

main.o: main.c
	gcc -Wall -c main.c

darkelf.o: darkelf.c
	gcc -Wall -c darkelf.c
    
darkelfRead.o: darkelfRead.c
	gcc -Wall -c darkelfRead.c

darkelfWrite.o: darkelfWrite.c
	gcc -Wall -c darkelfWrite.c

clean:
	rm *.o
	rm main
