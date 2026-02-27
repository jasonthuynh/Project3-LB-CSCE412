CXX = g++
CXXFLAGS = -std=c++11 -Wall

OBJS = main.o Request.o WebServer.o LoadBalancer.o

all: loadbalancer

loadbalancer: $(OBJS)
	$(CXX) $(CXXFLAGS) -o loadbalancer $(OBJS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Request.o: Request.cpp
	$(CXX) $(CXXFLAGS) -c Request.cpp

WebServer.o: WebServer.cpp
	$(CXX) $(CXXFLAGS) -c WebServer.cpp

LoadBalancer.o: LoadBalancer.cpp
	$(CXX) $(CXXFLAGS) -c LoadBalancer.cpp

clean:
	rm -f *.o loadbalancer