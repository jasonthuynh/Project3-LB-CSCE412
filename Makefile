CXX = g++
CXXFLAGS = -std=c++11 -Wall

OBJS = main.o Request.o WebServer.o LoadBalancer.o Switch.o

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

Switch.o: Switch.cpp
	$(CXX) $(CXXFLAGS) -c Switch.cpp

docs:
	doxygen Doxyfile
	@echo "Documentation generated in docs/html/index.html"
	@echo "Open with: open docs/html/index.html"

clean:
	rm -f *.o loadbalancer
	rm -rf docs