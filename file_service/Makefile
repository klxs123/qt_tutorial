# Makefile for systems with GNU tools
CC 	=	gcc
INSTALL	=	install
IFLAGS  = -idirafter dummyinc
#CFLAGS = -g
#CFLAGS	=	-O2 -fPIE -fstack-protector --param=ssp-buffer-size=4 \
	-Wall -W -Wshadow -Werror -Wformat-security \
	-D_FORTIFY_SOURCE=2 \
	#-pedantic -Wconversion

LIBS	=	`./vsf_findlibs.sh`
LINK	=	-Wl,-s
#LDFLAGS	=	-fPIE -pie -Wl,-z,relro -Wl,-z,now

OBJS	=	main.o utility.o prelogin.o ftpcmdio.o postlogin.o privsock.o \
		tunables.o ftpdataio.o secbuf.o ls.o \
		postprivparent.o logging.o str.o netstr.o sysstr.o strlist.o \
    banner.o filestr.o parseconf.o secutil.o \
    ascii.o oneprocess.o twoprocess.o privops.o standalone.o hash.o \
    tcpwrap.o ipaddrparse.o access.o features.o readwrite.o opts.o \
    ssl.o sslslave.o ptracesandbox.o ftppolicy.o sysutil.o sysdeputil.o \
    seccompsandbox.o


.c.o:
	$(CC) -c $*.c $(CFLAGS) $(IFLAGS)

file_service: $(OBJS) 
	$(CC) -o file_service $(OBJS) $(LINK) $(LDFLAGS) $(LIBS)

install:
	if [ -x /usr/local/sbin ]; then \
		$(INSTALL) -m 755 file_service /usr/local/sbin/file_service; \
	else \
		$(INSTALL) -m 755 file_service /usr/sbin/file_service; fi
	if [ -x /usr/local/man ]; then \
		$(INSTALL) -m 644 file_service.8 /usr/local/man/man8/file_service.8; \
		$(INSTALL) -m 644 file_service.conf.5 /usr/local/man/man5/file_service.conf.5; \
	elif [ -x /usr/share/man ]; then \
		$(INSTALL) -m 644 file_service.8 /usr/share/man/man8/file_service.8; \
		$(INSTALL) -m 644 file_service.conf.5 /usr/share/man/man5/file_service.conf.5; \
	else \
		$(INSTALL) -m 644 file_service.8 /usr/man/man8/file_service.8; \
		$(INSTALL) -m 644 file_service.conf.5 /usr/man/man5/file_service.conf.5; fi
	if [ -x /etc/xinetd.d ]; then \
		$(INSTALL) -m 644 xinetd.d/file_service /etc/xinetd.d/file_service; fi

clean:
	rm -f *.o *.swp file_service

