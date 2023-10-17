server:
	g++ -g -o server -std=c++20 \
	main.cpp error.cpp Socket.cpp Epoll.cpp Event.cpp EventDriven.cpp Server.cpp Acceptor.cpp Connection.cpp ThreadPool.cpp

clean:
	rm server
