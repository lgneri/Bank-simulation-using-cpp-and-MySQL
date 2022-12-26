CPP = g++
CPPFLAGS = -Wall

all:
	$(CPP) $(CPPFLAGS) Database.cpp Client.cpp Bank.cpp main.cpp -o output -L/usr/include/mysql/mysql -lmysqlclient
clean:
	rm eleicoes