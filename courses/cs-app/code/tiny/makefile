CFLAGS = -O2 -Wall ./lib/*.c -I .

build: tiny apps

tiny:
	mkdir bin -p
	gcc $(CFLAGS) -o ./bin/itiny ./src/itiny.c

apps:
	mkdir dist/cgi-bin -p
	gcc $(CFLAGS) -o ./dist/cgi-bin/adder ./src/apps/adder.c

clean:
	rm -r -f ./bin ./dist/cgi-bin

#============================= Run Server ====================================

itiny:
	./bin/itiny 2020 2
