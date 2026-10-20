CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
SRC = main.cpp Trade.cpp Holding.cpp RecordParser.cpp Portfolio.cpp TcpReceiver.cpp

portfolio: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o portfolio

clean:
	rm -f portfolio
