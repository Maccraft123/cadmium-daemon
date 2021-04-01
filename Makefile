prefix = /usr/local

all: daemon
daemon:
	$(CC) $(CPPFLAGS) $(CFLAGS) -DC100 -O2 daemon.c -o daemon_minnie -Wextra
	$(CC) $(CPPFLAGS) $(CFLAGS) -DKEVIN -O2 daemon.c -o daemon_kevin -Wextra

install:
	install -D daemon_kevin $(DESTDIR)$(prefix)/bin/daemon_kevin
	install -D daemon_minnie $(DESTDIR)$(prefix)/bin/daemon_minnie
	install -D cadmium_daemon $(DESTDIR)$(prefix)/bin/cadmium_daemon

distclean: clean
clean:
	-rm -f daemon_kevin
	-rm -f daemon_minnie
unistall:
	-rm -f $(DESTDIR)$(prefix)/bin/daemon_kevin
	-rm -f $(DESTDIR)$(prefix)/bin/daemon_minnie
	-rm -f $(DESTDIR)$(prefix)/bin/cadmium_daemon
