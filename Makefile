BIN_DIR=./bin
INCLUDE_DIR=./includes/
SRC_FILES=./src/*.cpp
BOOST_LIB_DIR=/usr/local/Cellar/boost/1.64.0_1/lib/

default:
	@(mkdir -p $(BIN_DIR))
	@(g++ -std=c++11 -o $(BIN_DIR)/scheduler -I$(INCLUDE_DIR) -L${BOOST_LIB_DIR} -lboost_system -lboost_filesystem $(SRC_FILES))

clear:
	@(rm -rf $(BIN_DIR))