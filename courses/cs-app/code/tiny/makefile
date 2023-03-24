CFLAGS = -O2 -Wall ./lib/*.c -I .

build: tiny apps

tiny:
	mkdir bin -p
	gcc $(CFLAGS) -o ./bin/itiny ./src/itiny.c
	gcc $(CFLAGS) -o ./bin/ttiny ./src/ttiny.c
	gcc $(CFLAGS) -o ./bin/dtiny ./src/dtiny.c

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

dtiny:
	./bin/dtiny 2022 0 0 0 2

#============================= Run Test ====================================

ttest:
	@mkdir bin -p
	@gcc $(CFLAGS) -o ./bin/ttiny ./src/ttiny.c -D THREAD_DEBUG
	@(./bin/ttiny 2021 0) > /dev/null &
	@./test.sh ttiny 2021 0 1000
	@sleep 5s
	@pkill ttiny

dtest:
	@mkdir bin -p
	@gcc $(CFLAGS) -o ./bin/dtiny ./src/dtiny.c -D THREAD_DEBUG
	@(./bin/dtiny 2022 5 1000 6) > /dev/null &
	@echo Test that dtiny can increase threads upto maximum
	@./test.sh dtiny 2022 0 1000
	@echo Sleeping for 30 seconds...
	@sleep 30s
	@echo Test that dtiny can decrease threads to minimum when no longer needed
	@./test.sh dtiny 2022 5 5
	@pkill dtiny
