DIR = ./bin
IPORT = 1111
PPORT = 1112
TPORT = 1113
PTPORT = 1114
DTPORT = 1115
LIBS = ./lib/*.c

build:
	mkdir ${DIR} -p
	gcc -O2 -o ${DIR}/echo-client client.c ${LIBS}
	gcc -O2 -o ${DIR}/echo-iserver iserver.c ${LIBS}
	gcc -O2 -o ${DIR}/echo-pserver pserver.c ${LIBS}
	gcc -O2 -o ${DIR}/echo-tserver tserver.c ${LIBS} -pthread
	gcc -O2 -o ${DIR}/echo-ptserver ptserver.c ${LIBS} -pthread
	gcc -O2 -o ${DIR}/echo-dtserver dtserver.c ${LIBS} -pthread

clean:
	rm -r -f ${DIR}

#============================ Run Client =================================

iclient:
	${DIR}/echo-client 127.0.0.1 ${IPORT}

pclient:
	${DIR}/echo-client 127.0.0.1 ${PPORT}

tclient:
	${DIR}/echo-client 127.0.0.1 ${TPORT}

ptclient:
	${DIR}/echo-client 127.0.0.1 ${PTPORT}

dtclient:
	${DIR}/echo-client 127.0.0.1 ${DTPORT}

#============================ Run Server =================================

iserver:
	${DIR}/echo-iserver ${IPORT}

pserver:
	${DIR}/echo-pserver ${PPORT}

tserver:
	${DIR}/echo-tserver ${TPORT}

ptserver:
	${DIR}/echo-ptserver ${PTPORT} 10

dtserver:
	${DIR}/echo-dtserver ${DTPORT} 2 10 2
