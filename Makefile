default:
	gcc -DC100 -DGNOME -O2 daemon.c -o daemon_minnie -Wextra
	gcc -DKEVIN -DGNOME -O2 daemon.c -o daemon_kevin -Wextra
