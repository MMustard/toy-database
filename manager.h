#pragma once
#include<memory>
#include "utils.h"
#include "manager_method.h"

/*
* SELECT * FROM 表名 列名1 值1 列名2 值2...列名n 值n
* INSERT INTO 表名 列名1 值1 列名2 值2...列名n 值n
* DELETE FROM 表名 列名1 值1 列名2 值2...列名n 值n
* UPDATE FROM 表名 列名1 值1 列名2 值2...列名n 值n NEWVALUE 列名 值
* SHOW DATABASES
* SHOW CREATE DATABASES
* ADD FROM 表名 新属性
* SELECT FROM 表名 列名1 值1 列名2 值2...列名n 值n ; 表名 列名 （返回在另一个表中的查找值（note：设定id为索引唯一项））
*/

struct Manager
{
private: 
	DataBase db;
	Manager(const std::string& conf): db(conf) { }

public:
	static Manager& getInstance(const std::string& conf) {
		static Manager manager(conf);
		return manager;
	}
	void execute(const std::string& cmd) {
		std::vector<std::string> order = Utils::split(cmd, ' ');
		MangerMethodFactory::get(order[0])->method(db, order);
	}
};