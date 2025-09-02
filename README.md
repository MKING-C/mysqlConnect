# MySQL Connection Pool

一个基于C++和MySQL的数据库连接池实现，提供了高效的数据库连接管理和复用机制。

## 功能特性

- **连接池管理**: 预先创建并维护一定数量的数据库连接，避免频繁创建和销毁连接的开销
- **线程安全**: 使用互斥锁和条件变量保证多线程环境下的连接访问安全
- **动态扩容**: 根据需求动态创建新的数据库连接
- **自动回收**: 定时回收空闲时间过长的连接，释放资源
- **智能指针管理**: 使用`shared_ptr`自动管理连接生命周期，防止内存泄漏
- **JSON配置**: 通过JSON文件配置数据库连接参数
- **超时控制**: 支持连接获取超时机制

## 项目结构

```
├── include/           # 头文件目录
│   ├── ConnectionPool.h    # 连接池类定义
│   └── MysqlConn.h         # MySQL连接类定义
├── src/               # 源文件目录
│   ├── ConnectionPool.cpp  # 连接池类实现
│   └── MysqlConn.cpp       # MySQL连接类实现
├── dbconf.json        # 数据库配置文件
├── main.cpp           # 主程序入口
└── test.cpp           # 测试程序
```

## 依赖项

- MySQL C Connector (libmysqlclient)
- JsonCpp
- C++11 或更高版本

## 配置文件

项目使用`dbconf.json`文件进行数据库配置，配置项包括：

```json
{
    "m_ip": "数据库IP地址",
    "m_user": "用户名",
    "m_password": "密码",
    "m_dbname": "数据库名",
    "m_port": 3306,
    "m_minSize": 5,
    "m_maxSize": 10,
    "m_timeout": 500,
    "m_maxIdleTime": 600
}
```

## 使用方法

1. 配置`dbconf.json`文件中的数据库连接信息
2. 编译项目：
   ```bash
   g++ -std=c++11 main.cpp src/*.cpp -lmysqlclient -ljsoncpp -Iinclude -o mysql_pool
   ```
3. 运行程序：
   ```bash
   ./mysql_pool
   ```

## 核心类介绍

### MysqlConn
MySQL数据库连接类，提供以下功能：
- 数据库连接和断开
- SQL语句执行（查询和更新）
- 结果集遍历和字段值获取
- 事务操作（开始、提交、回滚）
- 连接存活时间管理

### ConnectionPool
数据库连接池类，提供以下功能：
- 单例模式管理唯一连接池实例
- 连接的获取和释放
- 动态连接创建
- 空闲连接回收
- 线程安全的连接访问

## 许可证

MIT License

## 联系方式

如有问题，请提交Issue或联系项目维护者。