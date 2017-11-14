include config.mk

SRC = setfan.c
OBJ = ${SRC:.c=.o}

all: options setfan

options:
	@echo setfan build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

config.h:
	@echo creating $@ from config.def.h
	@cp config.def.h $@

setfan: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f setfan ${OBJ}

install: all
	@echo installing executable to ${DESTDIR}${PREFIX}/sbin
	@mkdir -p ${DESTDIR}${PREFIX}/sbin
	@cp -f setfan ${DESTDIR}${PREFIX}/sbin
	@chmod 755 ${DESTDIR}${PREFIX}/sbin/setfan

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/sbin
	@rm -f ${DESTDIR}${PREFIX}/sbin/setfan

.PHONY: all options clean install uninstall
