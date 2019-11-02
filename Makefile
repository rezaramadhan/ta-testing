all : clean compile

clean:
	rm -f *.run
	rm -f *.o

compile:
	gcc -o test.run src/test.c -L/opt/openssl/lib -lcrypto
