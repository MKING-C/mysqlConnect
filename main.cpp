#include <iostream>
#include "MysqlConn.cpp"
#include "ConnectionPool.cpp"
#include "memory"
using namespace std;  

int query(){
    MysqlConn conn;
    conn.connect("root", "123456", "cpp", "localhost", 3306);
    string sql = "insert insert person values(6,21,'female',ch)";
    bool flag = conn.update(sql);
    cout <<"flag value: "<< flag << endl;

    sql = "select * from person";
    conn.query(sql);

    while (conn.next())
    {
        cout << "ID: " << conn.value(0) << ", Age: " << conn.value(1) << ", Gender: " << conn.value(2) << ", Name: " << conn.value(3) << endl;
    }
    return 0;
}

int main(){
    std::cout << "Starting database connection test..." << std::endl;

    // // 请替换为您的实际数据库凭据
    // if (conn.connect("您的用户名", "您的密码", "您的数据库名", "localhost", 3306)) {
    //     std::cout << "Database connection successful!" << std::endl;
    // } else {
    //     std::cerr << "Failed to connect to database!" << std::endl;
    // }
    query();
    return 0;
}