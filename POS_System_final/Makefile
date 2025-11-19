
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
OBJ = main.o inventory.o billing.o report.o utils.o

pos: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o pos

main.o: main.c inventory.h billing.h report.h utils.h
	$(CC) $(CFLAGS) -c main.c

inventory.o: inventory.c inventory.h utils.h
	$(CC) $(CFLAGS) -c inventory.c

billing.o: billing.c billing.h inventory.h utils.h
	$(CC) $(CFLAGS) -c billing.c

report.o: report.c report.h billing.h
	$(CC) $(CFLAGS) -c report.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm -f *.o pos
