SERVER_FILES := Server
override CXXFLAGS += -std=c++17 -Isrc -IuSockets/src
override LDFLAGS += uSockets/*.o -lz

.PHONY: server
server:
	cd uSockets && WITH_SSL=0 make
	$(foreach FILE,$(SERVER_FILES),$(CXX) -flto -O3 $(CXXFLAGS) server/$(FILE).cpp -o $(FILE) $(LDFLAGS);)
	
all:
	make server
