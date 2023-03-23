CFLAGS = -O2 -Wall ./lib/*.c -I .

build: tiny apps

tiny:
	mkdir bin -p
	gcc $(CFLAGS) -o ./bin/itiny ./src/itiny.c
	gcc $(CFLAGS) -o ./bin/ttiny ./src/ttiny.c

apps:
	mkdir dist/cgi-bin -p
	gcc $(CFLAGS) -o ./dist/cgi-bin/adder ./src/apps/adder.c

clean:
	rm -r -f ./bin ./dist/cgi-bin

#============================= Run Server ====================================

itiny:
	./bin/itiny 2020 2

ttiny:
	./bin/ttiny 2021 2

#============================= Run Test ====================================

ttest:
	@mkdir bin -p
	@gcc $(CFLAGS) -o ./bin/ttiny ./src/ttiny.c -D THREAD_DEBUG
	@(./bin/ttiny 2021 0) > /dev/null &
	@./test.sh ttiny 2021 0 1000
	@sleep 5s 
	@pkill ttiny
