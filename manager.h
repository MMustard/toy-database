#pragma once
#include<memory>
#include "utils.h"
#include "manager_method.h"

/*
* SELECT * FROM ���� ����1 ֵ1 ����2 ֵ2...����n ֵn
* INSERT INTO ���� ����1 ֵ1 ����2 ֵ2...����n ֵn
* DELETE FROM ���� ����1 ֵ1 ����2 ֵ2...����n ֵn
* UPDATE FROM ���� ����1 ֵ1 ����2 ֵ2...����n ֵn NEWVALUE ���� ֵ
* SHOW DATABASES
* SHOW CREATE DATABASES
* ADD FROM ���� ������
* SELECT FROM ���� ����1 ֵ1 ����2 ֵ2...����n ֵn ; ���� ���� ����������һ�����еĲ���ֵ��note���趨idΪ����Ψһ���
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