G = g++-4.9
CFLAGS = -I/usr/local/lib/Oracle/instantclient_11_2/sdk/include
LFLAGS = -L/usr/local/lib/Oracle/instantclient_11_2 -locci -lociei

project: main.cpp
	$(G) $(CFLAGS) $(LFLAGS) main.cpp -o project
