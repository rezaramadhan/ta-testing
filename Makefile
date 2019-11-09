all : clean compile

clean:
	rm -f *.run
	rm -f *.o

compile:
	gcc -o test.run src/test.c -I/opt/openssl/include -L/opt/openssl/lib -lcrypto
