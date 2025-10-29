CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

.PHONY: test
test:
	mkdir -p build
	$(CXX) $(CXXFLAGS) src/packet.cpp test/test_packet.cpp -o build/test_packet
	./build/test_packet
