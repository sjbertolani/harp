#include <gflags/gflags.h>
#include <glog/logging.h>
#include <zmq.hpp>

#include <exception>
#include <iostream>

DEFINE_string(incoming, "tcp://*:9000", "Incoming socket");
DEFINE_string(outgoing1, "tcp://localhost:7000", "Outgoing socket #1");
DEFINE_string(outgoing2, "tcp://localhost:8000", "Outgoing socket #2");

int main (int argc, char *argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  zmq::context_t context(1);
  zmq::socket_t in(context, ZMQ_PULL);
  zmq::socket_t out1(context, ZMQ_PUSH);
  zmq::socket_t out2(context, ZMQ_PUSH);

  try {
    in.bind(FLAGS_incoming.c_str());
  } catch (std::exception& e) {
    LOG(FATAL) << "Failed to bind incoming socket: " << FLAGS_incoming << std::endl;
  }

  try {
    out1.connect(FLAGS_outgoing1.c_str());
  } catch (std::exception& e) {
    LOG(FATAL) << "Failed to connect first outgoing socket: " << FLAGS_outgoing1 << std::endl;
  }

  try {
    out2.connect(FLAGS_outgoing2.c_str());
  } catch (std::exception& e) {
    LOG(FATAL) << "Failed to connect second outgoing socket: " << FLAGS_outgoing2 << std::endl;
  }

  while (true) {
    zmq::message_t msg;
    CHECK(in.recv(&msg));
    CHECK(out1.send(msg));
    CHECK(out2.send(msg));
  }
}
