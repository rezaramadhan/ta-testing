all : clean compile

clean:
	rm -f *.run
	rm -f *.o

compile:
	gcc -o run_test.run test.c -lcrypto
