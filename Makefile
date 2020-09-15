default:
	gcc -DC100 -O2 daemon.c -o daemon_minnie -Wextra
	gcc -DKEVIN -O2 daemon.c -o daemon_kevin -Wextra
