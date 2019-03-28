include ../Make.defines

PROCS = server client

server: server.o
	${CC} ${CFLAGS} -o $@ $^ ${LIBS}

client: client.o
	${CC} ${CFLAGS} -o $@ $^ ${LIBS}

all: ${PROCS}

clean:
	${RM} ${PROCS} ${CLEANFILES}

