//
// Created by 22062 on 2025/8/18.
//
#include "MysqlConn.h"
#include <chrono>
using namespace chrono;
MysqlConn::MysqlConn() {
    // 初始化数据库连接
    m_conn = mysql_init(nullptr);
    std::cout << "MySQL connection object initialized" << std::endl;

    mysql_set_character_set(m_conn, "utf8");
}

MysqlConn::~MysqlConn() {
    if (m_conn!=nullptr) {
        mysql_close(m_conn);
    }
    freeResult();
}

bool MysqlConn::connect(string user, string passwd, string dbname, string ip, unsigned short port) {
    // 实现客户端和服务器连接
    if (!m_conn) {
        std::cerr << "Error: MySQL connection not initialized" << std::endl;
        return false;
    }
    
    MYSQL* conn = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passwd.c_str(), 
                                    dbname.c_str(), port, nullptr, 0);
    if (!conn) {
        std::cerr << "MySQL connection failed: " << mysql_error(m_conn) << std::endl;
        return false;
    }
    std::cout << "MySQL connection established" << std::endl;
    return true;
}

bool MysqlConn::update(string sql) {
    // 占位实现
    // 更新数据库
    if(mysql_query(m_conn, sql.c_str())){
        return false;
    }
    return true;
}

bool MysqlConn::query(string sql) {
    // 占位实现

    // 清空上次申请的内存
    freeResult();
    // 执行查询语句
    if(mysql_query(m_conn, sql.c_str())){
        return false;
    }
    m_result=mysql_store_result(m_conn);

    return true;
}

bool MysqlConn::next() {
    // 占位实现
    // 用于遍历结果集
    if(m_result){
        // mysql_fetch_row是返回值一个地址
        MYSQL_ROW row = mysql_fetch_row(m_result);
        if(row){
            // 处理每一行数据
            return true;
        }
    }
    return false;
}

string MysqlConn::value(int index) {
    // 占位实现
    // 得到结果集的字段值
    // 结果集的数量
    int rowCount = mysql_num_fields(m_result);
    if(index < 0 || index >= rowCount){
        return string();
    }
    // 获取当前行的字段值
    m_row = mysql_fetch_row(m_result);
    if(m_row){
        return m_row[index] ? string(m_row[index]) : string();
    }
    char* val= m_row[index];
    unsigned long len = mysql_fetch_lengths(m_result)[index];
    return string(val, len);
}

bool MysqlConn::transaction() {
    // 占位实现
    return mysql_autocommit(m_conn, false);
}

bool MysqlConn::commit() {
    // 占位实现
    return mysql_commit(m_conn);
}

bool MysqlConn::rollback() {
    // 占位实现
    return mysql_rollback(m_conn);
}

void MysqlConn::freeResult() {
    if (m_result) {
        mysql_free_result(m_result);
        m_result = nullptr;
    }
}

void MysqlConn::refreshAliveTime(){
    m_alivetime = steady_clock::now();
}
long long MysqlConn::getAliveTime(){
    nanoseconds res = steady_clock::now() - m_alivetime;
    milliseconds millisec = duration_cast<milliseconds>(steady_clock::now() - m_alivetime);
    return millisec.count();
}