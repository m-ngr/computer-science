DIR = ./bin
IPORT = 1111

build:
	mkdir ${DIR} -p
	gcc -O2 -o ${DIR}/echoclient client.c mcsapp.c
	gcc -O2 -o ${DIR}/echoiserver iserver.c mcsapp.c

iclient:
	${DIR}/echoclient 127.0.0.1 ${IPORT}

iserver:
	${DIR}/echoiserver ${IPORT}

clean:
	rm -r -f ${DIR}