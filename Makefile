BIN_DIR=./bin
INCLUDE_DIR = ./includes/
SRC_FILES = ./src/*.cpp

default:
	@(mkdir -p $(BIN_DIR))
	@(g++ -std=c++11 -o $(BIN_DIR)/scheduler -I$(INCLUDE_DIR) $(SRC_FILES))

clear:
	@(rm -rf $(BIN_DIR))