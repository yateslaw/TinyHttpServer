#ifndef WORKER_H
#define WORKER_H
#include "listener.h"

#include <string>
#include <map>
#include <vector>

#include "event2/event.h"
#include "event2/util.h"

#include "util.h"
class Connection;
class Listener;
class Watcher;
class Plugin;
class Worker
{
	public:
		Worker();
		~Worker();
		/*子进程函数*/
		bool Init(Watcher *wc,const char* host,const char* serv);
		void Run();
		static void WorkerExitSignal(evutil_socket_t signo,short event,void *arg);
		
		/*连接池函数*/
		Connection* NewCon();
		static void CloseCon(Connection *con);
	public:
		Watcher *wk_wc;
		struct event_base *wk_ebase;
		
		typedef std::map<evutil_socket_t, Connection *> ConnectionMap;
		ConnectionMap wk_con_map;
		
		Plugin*			*w_plugins;//插件链接结构
		int			 w_plugin_cnt;//插件数量
		
	private:
		/*连接池函数*/
		void InitConPool();
		Connection* GetFreeCon();
		bool AddConToFreePool(Connection *con);
		static void FreeCon(Connection *con);
		
		/*插件操作函数*/
		bool SetupPlugins();		
		bool LoadPlugins(); 
		void RemovePlugins();
		void UnloadPlugins();
	private:
		struct event *wk_exitEvent;
		Listener wk_listener;

		/*连接池参数*/
		typedef std::vector<Connection*> con_pool_t;
		con_pool_t con_pool;
		int con_pool_size;//连接池大小
		int con_pool_cur;//可用连接位置
		
};

#endif
