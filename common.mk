CC=cc
CFLAGS=-g -O0

CFLAGS_BASELINE=-mabi=aapcs
CFLAGS_CHERI=

.for progbase in ${PROGBASES}
.for variant in baseline cheri
CLEANFILES+=	${progbase}-${variant}

all: ${progbase}-${variant}

${progbase}-${variant}: ${progbase}.c
	${CC} -o ${.TARGET} ${.ALLSRC} ${CFLAGS} ${CFLAGS_${variant:tu}}
.endfor
.endfor

clean:
	rm -f ${CLEANFILES}

.PHONY: all clean

