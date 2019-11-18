OPENSSL_DIR=/opt/openssl_mod

all : clean compile

clean:
	rm -f *.run
	rm -f *.o

compile:
	gcc -g -o test.run src/test.c -I$(OPENSSL_DIR)/include -L$(OPENSSL_DIR)/lib -lcrypto
