default:
	gcc -DC100 -O2 daemon.c -o daemon_minnie -Wextra
	gcc -DKEVIN -O2 daemon.c -o daemon_kevin -Wextra

install:
	mkdir -p /opt/bin/
	cp daemon daemon_minnie daemon_kevin /opt/bin/
	cp c100pa-daemon.service /etc/systemd/system/
