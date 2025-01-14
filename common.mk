CC=		cc
CFLAGS=		-g -O0
CFLAGS_BASELINE=-mabi=aapcs
CFLAGS_CHERI=

SRCS+=		${PROG}.c

.for variant in baseline cheri
CLEANFILES+=	${PROG}-${variant}

all: ${PROG}-${variant}

${PROG}-${variant}: ${SRCS}
	${CC} -o ${.TARGET} ${.ALLSRC} ${CFLAGS} ${CFLAGS_${variant:tu}}
.endfor

clean:
	rm -f ${CLEANFILES}

.PHONY: all clean
