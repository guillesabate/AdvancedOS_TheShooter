CC=gcc
CFLAGS= -g -Wall -Wextra -lpthread
DEPS = libraries.h utils.h ext.h fat.h
OBJ = main.o utils.o ext.o fat.o

%.o: %.c $(DEPS)
	@echo "Compiling project"
	$(CC) -c -o $@ $< $(CFLAGS)

shooter: $(OBJ)
	@echo "Working on main"
	$(CC) -o $@ $^ $(CFLAGS)
	rm *.o
	@echo "Done!"

.PHONY: clean

clean:
	@echo "Cleaning up"
	rm -f *.o
