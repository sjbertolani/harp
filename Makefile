CC = g++
CFLAGS = -I/usr/local/include -Icommon -fPIC
LDFLAGS = -L/usr/local/lib -lgflags -lglog -lprotobuf -lzmq -g0 -O3

# See GNU Make, section "Chains of Implicit Rules"
# ftp://ftp.gnu.org/pub/pub/old-gnu/Manuals/make-3.79.1/html_chapter/make_10.html#SEC97
.SECONDARY: harp.pb.cc

all: broker client splitter

clean:
	rm -f *.o *.so *.pb.cc *.pb.h *_pb2.py *.pyc broker client splitter

# binaries
broker: broker.o
	$(CC) $(LDFLAGS) $^ -o $@

client: libharp.so client.o
	$(CC) $(LDFLAGS) $^ -o $@

splitter: splitter.o
	$(CC) $(LDFLAGS) $^ -o $@

# libraries
libharp.so : harp.pb.o
	$(CC) $(LDFLAGS) -fPIC -shared $^ -o $@

# compile
%.o : %.cc
	$(CC) $(CFLAGS) -c $< -o $@

# genproto
%.pb.cc %.pb.h : %.proto
	protoc --cpp_out=. --python_out=. $<

