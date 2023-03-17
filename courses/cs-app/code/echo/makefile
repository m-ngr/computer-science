DIR = ./bin
IPORT = 1111
PPORT = 1112
TPORT = 1113
LIBS = ./lib/*.c

build:
	mkdir ${DIR} -p
	gcc -O2 -o ${DIR}/echo-client client.c ${LIBS}
	gcc -O2 -o ${DIR}/echo-iserver iserver.c ${LIBS}
	gcc -O2 -o ${DIR}/echo-pserver pserver.c ${LIBS}
	gcc -O2 -o ${DIR}/echo-tserver tserver.c ${LIBS} -pthread

clean:
	rm -r -f ${DIR}

#============================ Run Client =================================

iclient:
	${DIR}/echo-client 127.0.0.1 ${IPORT}

pclient:
	${DIR}/echo-client 127.0.0.1 ${PPORT}

tclient:
	${DIR}/echo-client 127.0.0.1 ${TPORT}

#============================ Run Server =================================

iserver:
	${DIR}/echo-iserver ${IPORT}

pserver:
	${DIR}/echo-pserver ${PPORT}

tserver:
	${DIR}/echo-tserver ${TPORT}
