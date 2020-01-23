integ:
	gcc *.c -o gengar -lws2_32

prod:
	gcc *.c -o gengar -lws2_32 -mwindows
