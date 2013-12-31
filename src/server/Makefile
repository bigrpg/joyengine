SRC = \
	server.cpp \
	msg.cpp 	\
	ServerInterface.cpp \
	ServerApp.cpp \
	../lib/db_mysql/Wrapper.cpp	\
	../lib/db_mysql/MySqlException.cpp	\
	../lib/db_mysql/Query.cpp	\
	../lib/db_mysql/QueryHelper.cpp	\
	../lib/db_mysql/Transaction.cpp	\
	../lib/db_mysql/ResultSet.cpp	\
	../lib/stdinc/Concurrency.cpp \
	../lib/stdinc/BgTask.cpp	\
	../lib/db_mysql/MySqlConnectionPool.cpp \
	../lib/network/Helper.cpp 	\
	../lib/network/NetWork_Base_Type.cpp \
	../lib/stdinc/BufferStream.cpp \
	../lib/network/InterfaceHolder.cpp \
	../lib/network/InterfaceElement.cpp \
	../lib/network/Nub.cpp \
	../lib/network/Bundle.cpp \
	../lib/network/NetEventWaitor.cpp \
	../lib/network/BaseApp.cpp \
	../lib/network/NetPoint.cpp \
	../lib/network/RequestManager.cpp \
	

	
SRCCLI = \
	client.cpp \
	../lib/stdinc/BufferStream.cpp \
	../lib/network/InterfaceHolder.cpp \
	../lib/network/InterfaceElement.cpp \
	../lib/network/NetWork_Base_Type.cpp \
	../lib/network/Bundle.cpp \
	../lib/stdinc/RefCount.cpp \
	../lib/network/Nub.cpp \
	../lib/network/Helper.cpp \
	../lib/stdinc/RefCount.h \
	../lib/network/NetPoint.cpp \
	../lib/network/NetEventWaitor.cpp \
	../lib/network/RequestManager.cpp \
	

all:
	g++ -o server  -D__DISABLE__ARM__ -I/usr/local/mysql/include -I../lib  -L/usr/lib64/mysql -lmysqlclient_r   -levent_extra -levent $(SRC)

cli:
	g++ -o client -I../lib -D__DISABLE__ARM__ -levent_core -L/usr/lib64/mysql $(SRCCLI)
	
test:
	g++ -o test test.cpp
	
test1:
	g++  -DMAC_OS -E -P -<client.c> s.i -I/usr/local/mysql/include -I../lib  -levent_core -L/usr/local/mysql/lib -lmysqlclient_r 

clean:
	rm -rf server 
	rm -rf client
