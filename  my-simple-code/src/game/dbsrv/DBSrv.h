
#ifndef GAME_DBSRV_H_
#define GAME_DBSRV_H_

#include <mysdk/base/Common.h>
#include <mysdk/base/BlockingQueue.h>
#include <mysdk/base/PerformanceCounter.h>
#include <mysdk/base/Timestamp.h>
#include <mysdk/base/Thread.h>

#include <mysdk/net/EventLoop.h>
#include <mysdk/net/InetAddress.h>
#include <mysdk/net/TcpServer.h>
#include <mysdk/net/TcpConnection.h>

#include <game/dbsrv/codec/codec.h>
#include <game/dbsrv/WorkerThreadPool.h>
#include <game/dbsrv/WriterThreadPool.h>

#include <google/protobuf/message.h>

#include <map>

using namespace mysdk;
using namespace mysdk::net;

typedef struct tagContext
{
	int conId;
	int threadId;
} Context;

class DBSrv
{
public:
	typedef std::map<int, mysdk::net::TcpConnection*> ConMapT;
public:
	DBSrv(EventLoop* loop, InetAddress& serverAddr);
	~DBSrv();

public:
	void start();
	void stop();

	void onConnectionCallback(mysdk::net::TcpConnection* pCon);
	void onKaBuMessage(mysdk::net::TcpConnection* pCon,
			google::protobuf::Message* msg,
			mysdk::Timestamp timestamp);

	void sendReply(int conId, google::protobuf::Message* message);
	void sendReplyEx(int conId, mysdk::net::Buffer* pBuf);
	EventLoop* getEventLoop()
	{
		return loop_;
	}

	void tickMe();
	void ping();
	void srvinfo();
private:
	KabuCodec codec_;
	ConMapT conMap_;
	EventLoop* loop_;
	TcpServer server_;
private:
	DISALLOW_COPY_AND_ASSIGN(DBSrv);
};

#endif /* DBSRV_H_ */
