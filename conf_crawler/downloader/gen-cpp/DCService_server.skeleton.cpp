// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "DCService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

class DCServiceHandler : virtual public DCServiceIf {
 public:
  DCServiceHandler() {
    // Your initialization goes here
  }

  void get_download_task(std::vector<DownloadTask> & _return, const DownloaderType::type downloader_type) {
    // Your implementation goes here
    printf("get_download_task\n");
  }

  void push_download_task(const DownloadTask& download_task) {
    // Your implementation goes here
    printf("push_download_task\n");
  }

  void upload_download_task(const DownloadedBodyItem& downloaded_body_item) {
    // Your implementation goes here
    printf("upload_download_task\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<DCServiceHandler> handler(new DCServiceHandler());
  shared_ptr<TProcessor> processor(new DCServiceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

