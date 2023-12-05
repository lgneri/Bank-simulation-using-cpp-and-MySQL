CPP = g++
CPPFLAGS = -Wall

all:
	$(CPP) $(CPPFLAGS) Database.cpp Client.cpp Bank.cpp main.cpp -o bank -L/usr/include/mysql/mysql -lmysqlclient
clean:
	rm bank
