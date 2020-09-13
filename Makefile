default:
	gcc -DC100 -O2 daemon.c -o daemon_c100 -Wextra
	gcc -DKEVIN -O2 daemon.c -o daemon_kevin -Wextra
