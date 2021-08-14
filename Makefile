include nanopb/extra/nanopb.mk

CFLAGS = -ansi -Wall -Werror -g -O0
CFLAGS += "-I$(NANOPB_DIR)"
CFLAGS +=-I.

NODES_PB = JMS/electronics/src/libs/Network/Messages/JMS_Messages/nodes.pb.c
NODES_PB_HEADERS = JMS/electronics/src/libs/Network/Messages/JMS_Messages/nodes.pb.h

# Cpp Source code
CSRC = main.cpp
CSRC += $(NODES_PB)
CSRC += $(NANOPB_DIR)/pb_encode.c  # The nanopb encoder
CSRC += $(NANOPB_DIR)/pb_decode.c  # The nanopb decoder
CSRC += $(NANOPB_DIR)/pb_common.c  # The nanopb common parts


server: $(CSRC)
	$(CXX) $(CFLAGS) -oserver $(CSRC)

# nodes_pb.o: $(NODES_PB) $(NODES_PB_HEADERS)
# 	$(CXX) $(CFLAGS) -c -onodes_pb.o $(NODES_PB)

# nanopb.o: $(NANOPB) $(NANOPB_HEADERS)
# 	$(CXX) $(CFLAGS) -onanopb.o $(NANOPB)


clean:
	rm -rf server nodes_pb.o nanopb.o