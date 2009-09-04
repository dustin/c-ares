AUTOMAKE_OPTIONS = foreign

SHELL=/bin/sh

bin_PROGRAMS = adig ahost
lib_LIBRARIES = libares.a

ahost_sources = ahost.c
ahost_DEPENDENCIES = libares.a
ahost_LDADD = -L. -lares

adig_SOURCES = adig.c
adig_DEPENDENCIES = libares.a
adig_LDADD = -L. -lares

libares_a_SOURCES = \
	ares_fds.c ares_parse_ptr_reply.c \
	ares_free_errmem.c ares_private.h ares.h ares_free_hostent.c \
	ares_process.c ares__close_sockets.c ares_free_string.c ares_query.c \
	ares__get_hostent.c ares_gethostbyaddr.c ares_search.c \
	ares__read_line.c ares_gethostbyname.c ares_send.c ares_destroy.c \
	ares_init.c ares_strerror.c ares_dns.h ares_mkquery.c ares_timeout.c \
	ares_expand_name.c ares_parse_a_reply.c
