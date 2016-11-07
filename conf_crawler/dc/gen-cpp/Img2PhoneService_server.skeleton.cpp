// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "Img2PhoneService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

class Img2PhoneServiceHandler : virtual public Img2PhoneServiceIf {
 public:
  Img2PhoneServiceHandler() {
    // Your initialization goes here
  }

  void get_phone_by_img(std::string& _return, const std::string& url, const std::string& img_str) {
    // Your implementation goes here
    printf("get_phone_by_img\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<Img2PhoneServiceHandler> handler(new Img2PhoneServiceHandler());
  shared_ptr<TProcessor> processor(new Img2PhoneServiceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

