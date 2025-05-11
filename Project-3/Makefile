CFLAGS = -g -Wall -Werror

make: wish.o wishPrints.o wishLinked.o
	gcc wish.o wishPrints.o wishLinked.o $(CFLAGS) -o wish

wish.o: wish.c wish.h
	gcc -c wish.c $(CFLAGS)

wishPrints.o : wishPrints.c wish.h
	gcc -c wishPrints.c $(CFLAGS)

wishLinked.o : wishLinked.c wish.h
	gcc -c wishLinked.c $(CFLAGS)

clean:
	rm *.o