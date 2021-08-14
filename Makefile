include nanopb/extra/nanopb.mk

CFLAGS = -ansi -Wall -Werror -g -O0
CFLAGS += "-I$(NANOPB_DIR)"

# Cpp Source code
CSRC = main.cpp

server: $(CSRC)
	$(CXX) $(CFLAGS) -oserver $(CSRC)