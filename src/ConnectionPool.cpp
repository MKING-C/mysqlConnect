//
// Created by 22062 on 2025/8/18.
//
#include "ConnectionPool.h"
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <thread>
using namespace Json;
using namespace std;
ConnectionPool::ConnectionPool() { 
    // 加载配置文件
    if(!parseJsonFile()) {
        cerr << "Failed to load configuration file." << endl;
        return;
    }
    for(int i = 0; i < m_minSize; ++i) {
        MysqlConn* conn = new MysqlConn;
        if (conn->connect( m_user, m_password, m_dbname,m_ip, m_port)) {
            m_connectionQ.push(conn);
        } else {
            cerr << "Failed to connect to database." << endl;
            delete conn;
        }
    }
    // 创建线程
    thread producer(&ConnectionPool::produceConnection, this);
    thread recycler(&ConnectionPool::recycleConnection, this);
    // 不能够阻塞主线程
    producer.detach();
    recycler.detach();
}

ConnectionPool* ConnectionPool::getConnectionPool() {
    static ConnectionPool pool;
    return &pool;
}

bool ConnectionPool::parseJsonFile() {
    // 使用jsoncpp解析json文件
    ifstream ifs("dbconf.json");
    Reader rd;
    Value root;
    rd.parse(ifs, root);
    if (!ifs.is_open()) {
        cerr << "Failed to open dbconf.json" << endl;
        return false;
    }
    if(root.isObject()){
    m_ip = root["m_ip"].asString();
    m_user = root["m_user"].asString();
    m_password = root["m_password"].asString();
    m_dbname = root["m_dbname"].asString();
    m_port = root["m_port"].asInt();
    m_minSize = root["m_minSize"].asInt();
    m_maxSize = root["m_maxSize"].asInt();
    m_timeout = root["m_timeout"].asInt();
    m_maxIdleTime = root["m_maxIdleTime"].asInt();
        ifs.close();
    return true;
    }
    ifs.close();
    return false;
}

// 创建连接
void ConnectionPool::produceConnection() {
    while (true) {
        unique_lock<mutex> lock(m_mutex);
        while (m_connectionQ.size() >= m_minSize) {
            m_cond.wait(lock);
        }
        addConnection();
        m_cond.notify_all();
    }
}

// 销毁连接
void ConnectionPool::recycleConnection() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
        unique_lock<mutex> lock(m_mutex);
        while (m_connectionQ.size() > m_minSize) {
            MysqlConn* conn = m_connectionQ.front();
            if (conn->getAliveTime() >= m_maxIdleTime) {
                m_connectionQ.pop();
                delete conn;
            } else {
                break;
            }
        }
    }
}

// 添加连接子函数
void ConnectionPool::addConnection() {
    MysqlConn* conn = new MysqlConn;
    conn->connect(m_user, m_password, m_dbname, m_ip, m_port);
    conn->refreshAliveTime();
    m_connectionQ.push(conn);
}

shared_ptr<MysqlConn> ConnectionPool::getConnection() { 
    unique_lock<mutex> locker(m_mutex);
    while(m_connectionQ.empty())
    {
        /* code */
       if (cv_status::timeout==m_cond.wait_for(locker, chrono::milliseconds(m_timeout))){
            if (m_connectionQ.empty())
            {
                // return nullptr;
                continue;
                /* code */
            }
            
       }
    }
    shared_ptr<MysqlConn> connptr(m_connectionQ.front(),[this](MysqlConn* conn){
        lock_guard<mutex> locker(m_mutex);
        conn->refreshAliveTime();
        m_connectionQ.push(conn);
    });
    m_connectionQ.pop();
    m_cond.notify_all();
    return connptr;
}


ConnectionPool::~ConnectionPool() {
    lock_guard<mutex> locker(m_mutex);
    while (!m_connectionQ.empty()) {
        MysqlConn* conn = m_connectionQ.front();
        m_connectionQ.pop();
        delete conn;
    }
}