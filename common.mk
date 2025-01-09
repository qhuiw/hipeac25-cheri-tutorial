CC=cc
CFLAGS=-g -O2 -Wall -Wcheri

CFLAGS_BASELINE=-march=morello+noa64c -mabi=aapcs
CFLAGS_CHERI=-march=morello -mabi=purecap

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

