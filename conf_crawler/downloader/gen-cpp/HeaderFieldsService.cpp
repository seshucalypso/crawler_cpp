/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "HeaderFieldsService.h"



uint32_t HeaderFieldsService_get_header_fields_args::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->url);
          this->__isset.url = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->depth);
          this->__isset.depth = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t HeaderFieldsService_get_header_fields_args::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("HeaderFieldsService_get_header_fields_args");

  xfer += oprot->writeFieldBegin("url", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->url);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("depth", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->depth);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t HeaderFieldsService_get_header_fields_pargs::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("HeaderFieldsService_get_header_fields_pargs");

  xfer += oprot->writeFieldBegin("url", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString((*(this->url)));
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("depth", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32((*(this->depth)));
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t HeaderFieldsService_get_header_fields_result::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 0:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->success.clear();
            uint32_t _size93;
            ::apache::thrift::protocol::TType _etype96;
            xfer += iprot->readListBegin(_etype96, _size93);
            this->success.resize(_size93);
            uint32_t _i97;
            for (_i97 = 0; _i97 < _size93; ++_i97)
            {
              xfer += iprot->readString(this->success[_i97]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.success = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t HeaderFieldsService_get_header_fields_result::write(::apache::thrift::protocol::TProtocol* oprot) const {

  uint32_t xfer = 0;

  xfer += oprot->writeStructBegin("HeaderFieldsService_get_header_fields_result");

  if (this->__isset.success) {
    xfer += oprot->writeFieldBegin("success", ::apache::thrift::protocol::T_LIST, 0);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->success.size()));
      std::vector<std::string> ::const_iterator _iter98;
      for (_iter98 = this->success.begin(); _iter98 != this->success.end(); ++_iter98)
      {
        xfer += oprot->writeString((*_iter98));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

uint32_t HeaderFieldsService_get_header_fields_presult::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 0:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            (*(this->success)).clear();
            uint32_t _size99;
            ::apache::thrift::protocol::TType _etype102;
            xfer += iprot->readListBegin(_etype102, _size99);
            (*(this->success)).resize(_size99);
            uint32_t _i103;
            for (_i103 = 0; _i103 < _size99; ++_i103)
            {
              xfer += iprot->readString((*(this->success))[_i103]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.success = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

void HeaderFieldsServiceClient::get_header_fields(std::vector<std::string> & _return, const std::string& url, const int32_t depth)
{
  send_get_header_fields(url, depth);
  recv_get_header_fields(_return);
}

void HeaderFieldsServiceClient::send_get_header_fields(const std::string& url, const int32_t depth)
{
  int32_t cseqid = 0;
  oprot_->writeMessageBegin("get_header_fields", ::apache::thrift::protocol::T_CALL, cseqid);

  HeaderFieldsService_get_header_fields_pargs args;
  args.url = &url;
  args.depth = &depth;
  args.write(oprot_);

  oprot_->writeMessageEnd();
  oprot_->getTransport()->writeEnd();
  oprot_->getTransport()->flush();
}

void HeaderFieldsServiceClient::recv_get_header_fields(std::vector<std::string> & _return)
{

  int32_t rseqid = 0;
  std::string fname;
  ::apache::thrift::protocol::TMessageType mtype;

  iprot_->readMessageBegin(fname, mtype, rseqid);
  if (mtype == ::apache::thrift::protocol::T_EXCEPTION) {
    ::apache::thrift::TApplicationException x;
    x.read(iprot_);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
    throw x;
  }
  if (mtype != ::apache::thrift::protocol::T_REPLY) {
    iprot_->skip(::apache::thrift::protocol::T_STRUCT);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
  }
  if (fname.compare("get_header_fields") != 0) {
    iprot_->skip(::apache::thrift::protocol::T_STRUCT);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
  }
  HeaderFieldsService_get_header_fields_presult result;
  result.success = &_return;
  result.read(iprot_);
  iprot_->readMessageEnd();
  iprot_->getTransport()->readEnd();

  if (result.__isset.success) {
    // _return pointer has now been filled
    return;
  }
  throw ::apache::thrift::TApplicationException(::apache::thrift::TApplicationException::MISSING_RESULT, "get_header_fields failed: unknown result");
}

bool HeaderFieldsServiceProcessor::dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext) {
  ProcessMap::iterator pfn;
  pfn = processMap_.find(fname);
  if (pfn == processMap_.end()) {
    iprot->skip(::apache::thrift::protocol::T_STRUCT);
    iprot->readMessageEnd();
    iprot->getTransport()->readEnd();
    ::apache::thrift::TApplicationException x(::apache::thrift::TApplicationException::UNKNOWN_METHOD, "Invalid method name: '"+fname+"'");
    oprot->writeMessageBegin(fname, ::apache::thrift::protocol::T_EXCEPTION, seqid);
    x.write(oprot);
    oprot->writeMessageEnd();
    oprot->getTransport()->writeEnd();
    oprot->getTransport()->flush();
    return true;
  }
  (this->*(pfn->second))(seqid, iprot, oprot, callContext);
  return true;
}

void HeaderFieldsServiceProcessor::process_get_header_fields(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext)
{
  void* ctx = NULL;
  if (this->eventHandler_.get() != NULL) {
    ctx = this->eventHandler_->getContext("HeaderFieldsService.get_header_fields", callContext);
  }
  ::apache::thrift::TProcessorContextFreer freer(this->eventHandler_.get(), ctx, "HeaderFieldsService.get_header_fields");

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->preRead(ctx, "HeaderFieldsService.get_header_fields");
  }

  HeaderFieldsService_get_header_fields_args args;
  args.read(iprot);
  iprot->readMessageEnd();
  uint32_t bytes = iprot->getTransport()->readEnd();

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->postRead(ctx, "HeaderFieldsService.get_header_fields", bytes);
  }

  HeaderFieldsService_get_header_fields_result result;
  try {
    iface_->get_header_fields(result.success, args.url, args.depth);
    result.__isset.success = true;
  } catch (const std::exception& e) {
    if (this->eventHandler_.get() != NULL) {
      this->eventHandler_->handlerError(ctx, "HeaderFieldsService.get_header_fields");
    }

    ::apache::thrift::TApplicationException x(e.what());
    oprot->writeMessageBegin("get_header_fields", ::apache::thrift::protocol::T_EXCEPTION, seqid);
    x.write(oprot);
    oprot->writeMessageEnd();
    oprot->getTransport()->writeEnd();
    oprot->getTransport()->flush();
    return;
  }

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->preWrite(ctx, "HeaderFieldsService.get_header_fields");
  }

  oprot->writeMessageBegin("get_header_fields", ::apache::thrift::protocol::T_REPLY, seqid);
  result.write(oprot);
  oprot->writeMessageEnd();
  bytes = oprot->getTransport()->writeEnd();
  oprot->getTransport()->flush();

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->postWrite(ctx, "HeaderFieldsService.get_header_fields", bytes);
  }
}

::boost::shared_ptr< ::apache::thrift::TProcessor > HeaderFieldsServiceProcessorFactory::getProcessor(const ::apache::thrift::TConnectionInfo& connInfo) {
  ::apache::thrift::ReleaseHandler< HeaderFieldsServiceIfFactory > cleanup(handlerFactory_);
  ::boost::shared_ptr< HeaderFieldsServiceIf > handler(handlerFactory_->getHandler(connInfo), cleanup);
  ::boost::shared_ptr< ::apache::thrift::TProcessor > processor(new HeaderFieldsServiceProcessor(handler));
  return processor;
}


