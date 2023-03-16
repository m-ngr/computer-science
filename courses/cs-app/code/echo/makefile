DIR = ./bin
IPORT = 1111
LIBS = ./lib/mcsapp.c

build:
	mkdir ${DIR} -p
	gcc -O2 -o ${DIR}/echo-client client.c ${LIBS}
	gcc -O2 -o ${DIR}/echo-iserver iserver.c ${LIBS}

clean:
	rm -r -f ${DIR}

#============================ Run Client =================================

iclient:
	${DIR}/echo-client 127.0.0.1 ${IPORT}

#============================ Run Server =================================

iserver:
	${DIR}/echo-iserver ${IPORT}