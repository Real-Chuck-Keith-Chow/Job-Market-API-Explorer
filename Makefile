CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./src -I./src/model -I./third_party
LDFLAGS = -lcurl -lsqlite3

SRC = src/main.cpp \
      src/ApiClient.cpp \
      src/Database.cpp \
      src/JobParser.cpp

OUT = job_app

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
