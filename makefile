prog0: prog0.c
	gcc -g -Wall -Wextra -Wwrite-strings -oprog0 prog0.c

clean:
	rm -f prog0
	rm -f tests.out
