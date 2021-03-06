/**
 * @Copyright 2011 GanJi Inc.
 * @file    ganji/crawler/conf_crawler/url_dedup/redis/redis.cc
 * @namespace ganji::crawler::conf_crawler::dedup::redis
 * @version 1.0
 * @author  zdn
 * @date    2014-02-17
 *
 * 改动程序后， 请使用tools/cpplint/cpplint.py 检查代码是否符合编码规范!
 * 遵循的编码规范请参考: http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
 * Change Log:
 */

#include "ganji/util/cache/redis/redis.h"
#include "ganji/util/cache/redis/redis_config.h"

#include "ganji/util/log/thread_fast_log.h"
#include "ganji/util/thread/sleep.h"
#include "ganji/util/system/system.h"
#include "ganji/util/time/time.h"

using std::string;
namespace FastLog = ::ganji::util::log::ThreadFastLog;
using FastLog::WriteLog;
using FastLog::kLogFatal;
using FastLog::kLogDebug;
using FastLog::kLogNotice;
using FastLog::kLogWarning;
using ganji::util::thread::Thread;
using ganji::util::thread::RWLock;
namespace Sleep = ::ganji::util::thread::Sleep;
namespace System = ::ganji::util::system::System;
namespace Time = ::ganji::util::time;

namespace ganji { namespace crawler { namespace conf_crawler { namespace dedup { namespace redis {

Redis::Redis():redis_ip_(""), redis_port_(0), p_config_(NULL), redis_context_(NULL), p_timer_thread_(NULL)
{
}

Redis::~Redis() {
  if (redis_context_ != NULL)
    delete redis_context_;
  if (p_config_ != NULL)
    delete p_config_;
  Thread::FreeThread(p_timer_thread_);
}

int Redis::Init()
{
  RedisConfig p_config_;
  string conf_path = "conf/conf.txt";
  p_config_.LoadConfig(conf_path);
  p_config_.PrintConfig();

  redis_ip_ = p_config_.GetRedisIP();
  redis_port_ = p_config_.GetRedisPort();
  if (redis_ip_.empty() || redis_port_ <= 0) {
    WriteLog(kLogFatal, "Redis Address[%s][%d] Invalid", redis_ip_.c_str(), redis_port_);
    return REDIS_ERROR;
  }

  // start timer thread
  p_timer_thread_ = Thread::CreateThread(TimerThread, this);
  p_timer_thread_->ResumeThread();
  return Connect();
}
int Redis::GetConnectionState()
{
  string dbsize;
  return RedisDBSize(dbsize); 
}
int Redis::Connect()
{
  if ((redis_context_ = redisConnect(redis_ip_.c_str(), redis_port_)) == NULL)
  {
    WriteLog(kLogFatal, "Redis Connection Error [%s][%d]", redis_ip_.c_str(), redis_port_);
    return REDIS_CONNECTION_ERROR;
  }
  return REDIS_SUCCESS;
}

bool Redis::IsExists(const std::string &key)
{
}

bool Redis::Insert(const std::string &key)
{
}

bool Redis::TestExistsAndInsert(const std::string &key, bool *p_exists)
{
}

void Redis::Rehash(int bucket_count)
{
}

bool Redis::Remove(const std::string &key)
{
}

int Redis::BatchRemove(const std::string &key_pattern)
{
}

int Redis::RedisSingleCmd(redisContext* redis_context, const char* cmd, string& reply_str)
{
  if (redis_context == NULL)
    return REDIS_CONNECTION_ERROR;
  if (cmd == NULL)
    return REDIS_FAIL;
  db_lock_.WrLock();
  redisReply* reply;
  reply = (redisReply*)redisCommand(redis_context, cmd);
  if (!reply)
  {
    db_lock_.Unlock();
    WriteLog(kLogFatal, ">>Redis Fail[%s]", cmd);
    return REDIS_FAIL;
  }
  if (REDIS_REPLY_ERROR == reply->type)
  {
    db_lock_.Unlock();
    WriteLog(kLogFatal, ">>Redis Single Command Fail[%s][ERROR CODE:%d:%s]", cmd, reply->type, reply->str);
    freeReplyObject(reply);
    return REDIS_COMMAND_FAIL;
  }
  else if (REDIS_REPLY_STRING == reply->type)
  {
    reply_str.append(reply->str);
  }
  else
  {
    char reply_buf[VAL_LEN];
    snprintf(reply_buf, VAL_LEN, "%lld", reply->integer);
    reply_str.append(reply_buf);
  }
  freeReplyObject(reply);
  db_lock_.Unlock();
  return REDIS_SUCCESS;
}

int Redis::RedisMutipleCmd(redisContext* redis_context, const vector<string>& vec_cmd)
{ 

  if (redis_context == NULL)
    return REDIS_CONNECTION_ERROR;
  if (vec_cmd.empty())
    return REDIS_FAIL;
  redisReply* reply;
  for (vector<string>::const_iterator it = vec_cmd.begin(); it != vec_cmd.end(); ++it)
  {
    db_lock_.WrLock();
    reply = (redisReply*)redisCommand(redis_context, it->c_str());
    if (!reply)
    {
      db_lock_.Unlock();
      WriteLog(kLogFatal, ">>Redis Fail[%s]", it->c_str());
      return REDIS_FAIL;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
      db_lock_.Unlock();
      freeReplyObject(reply);
      WriteLog(kLogFatal, ">>Redis Mutiple Command Fail[%s][ERROR CODE:%d]", it->c_str(), reply->type);
      return REDIS_COMMAND_FAIL;
    }
  }
  freeReplyObject(reply);
  db_lock_.Unlock();
  return REDIS_SUCCESS;
}
int Redis::RedisCmdWithMutipleReply(redisContext* redis_context, const char* cmd, vector<string>& vec_reply)
{
  if (redis_context == NULL)
    return REDIS_CONNECTION_ERROR;
  if (cmd == NULL)
    return REDIS_FAIL;
  redisReply* reply;
  db_lock_.WrLock();
  reply = (redisReply*)redisCommand(redis_context, cmd);
  if (!reply)
  {
    db_lock_.Unlock();
    WriteLog(kLogFatal, ">>Redis Fail[%s]", cmd);
    return REDIS_FAIL;
  }
  if (REDIS_REPLY_ERROR == reply->type)
  {
    db_lock_.Unlock();
    freeReplyObject(reply);
    WriteLog(kLogFatal, ">>Redis Mutiple Command With Reply Fail[%s][ERROR CODE:%d]", cmd, reply->type);
    return REDIS_COMMAND_FAIL;
  }
  if (REDIS_REPLY_ARRAY == reply->type)
  {
    for (int i = 0; i < reply->elements; ++i)
    {
      vec_reply.push_back(reply->element[i]->str);
      //WriteLog(kLogDebug, ">>RedisCmdWithMutipleReply[reply->elements[%d]:type=%d, integer=%lld, len=%d, str=%s, elements=%zu]", i, reply->element[i]->type, reply->element[i]->integer, reply->element[i]->len, reply->element[i]->str, reply->element[i]->elements);
    }
  }   
  freeReplyObject(reply);
  db_lock_.Unlock();
  return REDIS_SUCCESS;
}
int Redis::RedisTransaction(redisContext* redis_context, const vector<string>& vec_cmd, vector<string>& vec_reply)
{
  if (redis_context == NULL)
    return REDIS_CONNECTION_ERROR;
  if (vec_cmd.empty())
    return REDIS_FAIL;
  string reply;
  int reply_code = REDIS_SUCCESS;
  db_lock_.WrLock();
  if (REDIS_SUCCESS != (reply_code = RedisSingleCmd(redis_context, "MUTI", reply)));
  {
     db_lock_.Unlock();
     WriteLog(kLogFatal, ">>Redis Transaction Fail[ERROR CODE:%d]",reply_code);
     return reply_code;
  }
  if (REDIS_SUCCESS != (reply_code = RedisMutipleCmd(redis_context, vec_cmd)));
  {     
     db_lock_.Unlock();
     WriteLog(kLogFatal, ">>Redis Transaction Fail[ERROR CODE:%d]",reply_code);
     reply_code = RedisSingleCmd(redis_context, "DISCARD", reply);
     return reply_code;
  }
  if (REDIS_SUCCESS != (reply_code = RedisCmdWithMutipleReply(redis_context, "EXEC", vec_reply)));
  {
     db_lock_.Unlock();
     WriteLog(kLogFatal, ">>Redis Transaction Fail[ERROR CODE:%d]",reply_code);
     return reply_code;
  } 
  db_lock_.Unlock();
  return REDIS_SUCCESS;
}
int Redis::RediskeyEXISTS(const string& key)
{
  string reply;
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "EXISTS %s", key.c_str());
  RedisSingleCmd(redis_context_, cmd, reply);
  return atoi(reply.c_str());
}

int Redis::RedisKeyDEL(const string& key)
{
  string reply;
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "DEL %s", key.c_str());
  return RedisSingleCmd(redis_context_, cmd, reply);
}

int Redis::RediskeyEXPIRE(const string& key, int seconds)
{
  string reply;
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "EXPIRE %s %d", key.c_str(), seconds);
  return RedisSingleCmd(redis_context_, cmd, reply);
}

int Redis::RedisSETNX(const string& key, string& value)
{
  string reply;
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "SETNX %s %s", key.c_str(), value.c_str());
  RedisSingleCmd(redis_context_, cmd, reply);
  return atoi(reply.c_str());
}

int Redis::RedisSETEX(const string& key, int seconds, string& value)
{
  string reply;
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "SETEX %s %d %s", key.c_str(), seconds, value.c_str());
  RedisSingleCmd(redis_context_, cmd, reply);
  return REDIS_SUCCESS;
}

int Redis::RedisGET(const string& key, string& value)
{
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "GET %s", key.c_str());
  return RedisSingleCmd(redis_context_, cmd, value);
}

int Redis::RedisHEXISTS(const string& hash, const string& key)
{
  string reply;
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "HEXISTS %s %s", hash.c_str(), key.c_str());
  RedisSingleCmd(redis_context_, cmd, reply);
  return atoi(reply.c_str());
}

int Redis::RedisRPUSH(const string& list, const string& element)
{
  string reply;
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "RPUSH %s %s", list.c_str(), element.c_str());
  RedisSingleCmd(redis_context_, cmd, reply);
  return atoi(reply.c_str());
}

int Redis::RedisLPUSH(const string& list, const string& element)
{
  string reply;
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "LPUSH %s %s", list.c_str(), element.c_str());
  RedisSingleCmd(redis_context_, cmd, reply);
  return atoi(reply.c_str());
}

int Redis::RedisLPUSH(const string& hash, const std::vector<string>& vec_cmd){
  string reply;
  string cmd;
  char key_cmd[CMD_MAX_LEN];
  snprintf(key_cmd, sizeof(key_cmd), "LPUSH %s ",  hash.c_str());
  cmd.append(key_cmd); 
  std::vector<string>::const_iterator iter = vec_cmd.begin();
  while(iter != vec_cmd.end()){
    string key = *iter;
    char pair_cmd[CMD_MAX_LEN];
  	snprintf(pair_cmd , sizeof(pair_cmd), " %s ",  key.c_str());
    cmd.append(pair_cmd); 
    iter++;
  }
  RedisSingleCmd(redis_context_, cmd.c_str(), reply);
  return atoi(reply.c_str());
}

int Redis::RedisRPUSH(const string& hash, const std::vector<string>& vec_cmd){
  string reply;
  string cmd;
  char key_cmd[CMD_MAX_LEN];
  snprintf(key_cmd, sizeof(key_cmd), "RPUSH %s ",  hash.c_str());
  cmd.append(key_cmd); 
  std::vector<string>::const_iterator iter = vec_cmd.begin();
  while(iter != vec_cmd.end()){
    string key = *iter;
    char pair_cmd[CMD_MAX_LEN];
  	snprintf(pair_cmd , sizeof(pair_cmd), " %s ",  key.c_str());
    cmd.append(pair_cmd); 
    iter++;
  }
  RedisSingleCmd(redis_context_, cmd.c_str(), reply);
  return atoi(reply.c_str());
}
  
int Redis::RedisLPOP(const string& key, string& value)
{
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "LPOP %s", key.c_str());
  return RedisSingleCmd(redis_context_, cmd, value);
}

int Redis::RedisRPOP(const string& key, string& value)
{
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "RPOP %s", key.c_str());
  return RedisSingleCmd(redis_context_, cmd, value);
}

int Redis::RedisHSETNX(const string& hash, const string& key, string& value)
{
  string reply;
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "HSETNX %s %s %s", hash.c_str(), key.c_str(), value.c_str());
  RedisSingleCmd(redis_context_, cmd, reply);
  return atoi(reply.c_str());
}

int Redis::RedisHINCRBY(const string& hash, const string& key, string& value)
{
  string reply;
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "HINCRBY %s %s %s", hash.c_str(), key.c_str(), value.c_str());
  RedisSingleCmd(redis_context_, cmd, reply);
  return atoi(reply.c_str());
}

int Redis::RedisHDEL(const string& hash, const string& key)
{
  string reply;
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "HDEL %s %s", hash.c_str(), key.c_str());
  return RedisSingleCmd(redis_context_, cmd, reply);
}

int Redis::RedisHGET(const string& hash, const string& key, string& value)
{
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "HGET %s %s", hash.c_str(), key.c_str());
  return RedisSingleCmd(redis_context_, cmd, value);
}

int Redis::RedisHSET(const string& hash, const string& key, string& value)
{
  string reply;
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "HSET %s %s %s", hash.c_str(), key.c_str(), value.c_str());
  return RedisSingleCmd(redis_context_, cmd, reply);
}

int Redis::RedisLTRIM(const string& list_key, const string& start, string& end)
{
  string reply;
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "LTRIM %s %s %s", list_key.c_str(), start.c_str(), end.c_str());
  return RedisSingleCmd(redis_context_, cmd, reply);
}

int Redis::RedisHMSET(const string& hash, const std::map<string, string>& map_cmd){
  string reply;
  string cmd;
  char key_cmd[CMD_MAX_LEN];
  snprintf(key_cmd, sizeof(key_cmd), "HMSET %s ",  hash.c_str());
  cmd.append(key_cmd); 
  std::map<string, string>::const_iterator iter = map_cmd.begin();
  while(iter != map_cmd.end()){
    string key = iter->first;
    string value = iter->second;
    char pair_cmd[CMD_MAX_LEN];
  	snprintf(pair_cmd , sizeof(pair_cmd), " %s %s ",  key.c_str(), value.c_str());
    cmd.append(pair_cmd); 
    iter++;
  }
  
  return RedisSingleCmd(redis_context_, cmd.c_str(), reply);
}

int Redis::RedisHMGET(const string& hash, const std::vector<string>& key_vec, std::vector<string>& value_vec){
  //vector<string> reply;
  string cmd;
  char key_cmd[CMD_MAX_LEN];
  char pair_cmd[CMD_MAX_LEN];
  snprintf(key_cmd, sizeof(key_cmd), "HMGET %s ",  hash.c_str());
  cmd.append(key_cmd); 
  std::vector<string>::const_iterator iter = key_vec.begin();
  iter = key_vec.begin();
  while(iter != key_vec.end()){
  	snprintf(pair_cmd , sizeof(pair_cmd), " %s ",  iter->c_str());
    cmd.append(pair_cmd); 
    iter++;
  }
  return RedisCmdWithMutipleReply(redis_context_, cmd.c_str(), value_vec);
}

int Redis::RedisHLRANGE(const string& hash, const std::vector<string>& key_vec, std::vector<string>& value_vec){
  //vector<string> reply;
  string cmd;
  char key_cmd[CMD_MAX_LEN];
  char pair_cmd[CMD_MAX_LEN];
  snprintf(key_cmd, sizeof(key_cmd), "LRANGE %s ",  hash.c_str());
  cmd.append(key_cmd); 
  std::vector<string>::const_iterator iter = key_vec.begin();
  iter = key_vec.begin();
  while(iter != key_vec.end()){
  	snprintf(pair_cmd , sizeof(pair_cmd), " %s ",  iter->c_str());
    cmd.append(pair_cmd); 
    iter++;
  }
  return RedisCmdWithMutipleReply(redis_context_, cmd.c_str(), value_vec);
}

int Redis::RedisDBSize(string& dbsize)
{
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "DBSIZE");
  int reply = REDIS_SUCCESS;
  reply = RedisSingleCmd(redis_context_, cmd, dbsize);
  WriteLog(kLogDebug, ">>Redis DBSIZE[%s]", dbsize.c_str());
  return reply;
}

int Redis::RedisLLEN(string& key, string& length)
{
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "LLEN %s", key.c_str());
  int reply = REDIS_SUCCESS;
  reply = RedisSingleCmd(redis_context_, cmd, length);
  return reply;
}

int Redis::RedisFlushDB()
{
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "flushdb");
  string reply;
  WriteLog(kLogDebug, ">>Redis flushdb");
  return RedisSingleCmd(redis_context_, cmd, reply);
}

int Redis::RedisFlushAll()
{
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "flushall");
  string reply;
  WriteLog(kLogDebug, ">>Redis flushall");
  return RedisSingleCmd(redis_context_, cmd, reply);
}

int Redis::RedisSelectDB(int index)
{
  char cmd[CMD_MAX_LEN];
  snprintf(cmd, sizeof(cmd), "select %d", index);
  string reply;
  RedisSingleCmd(redis_context_, cmd, reply);
  //WriteLog(kLogDebug, ">>Redis select[%d]", index);
  return atoi(reply.c_str());
}

void* Redis::TimerThread(void* arg) {
  Redis *p_redis = reinterpret_cast<Redis *>(arg);
  char cmd[CMD_MAX_LEN];
  while (true) {
    Sleep::DoSleep(60 * 1000);
    int mb = 0;
    if (System::GetMem(&mb) < 0)
      continue;
    string dbsize;
    if (REDIS_SUCCESS != p_redis->GetConnectionState())
    {
      p_redis->Connect();
    }
    else 
    {
      int reply = REDIS_SUCCESS;
      if ((reply = p_redis->RedisDBSize(dbsize)) != REDIS_SUCCESS)
        WriteLog(kLogFatal, "REDIS ERROR");
    }
    WriteLog(kLogNotice, "#redis db size:%s system memory usage:%dMB", dbsize.c_str(),mb);
  }
  return NULL;
}

}}}}};  ///< end of namespace ganji::crawler::conf_crawler::dedup::redis

