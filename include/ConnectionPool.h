#pragma once
#include <vector>
#include <mutex>
#include <mysql.h>
#include <queue>
#include "iostream"
#include "MysqlConn.h"
#include "condition_variable"
using namespace std;
class ConnectionPool {
public:
    static ConnectionPool* getConnectionPool();    
    ConnectionPool(const ConnectionPool&) = delete;
    ConnectionPool& operator=(const ConnectionPool&) = delete;
    ~ConnectionPool();

    // 获取数据库连接函数
    shared_ptr<MysqlConn> getConnection();
    // 释放数据库连接
    void releaseConnection(MysqlConn* conn);
private:
    ConnectionPool();
    bool parseJsonFile();

    void produceConnection();
    void recycleConnection();
    void addConnection();

    string m_ip;
    string m_user;
    string m_password;
    string m_dbname;
    unsigned int m_port;
    int m_minSize;
    int m_maxSize;
    int m_timeout;
    int m_maxIdleTime;

    // 连接池
    queue<MysqlConn*> m_connectionQ;
    // 互斥锁
    mutex m_mutex;
    // 条件变量
    condition_variable m_cond;
};