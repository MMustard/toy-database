#pragma once
#include"base.h"
#include<vector>
#include<string>

struct Utils {
	static std::vector<std::string> split(const std::string& str, char ch)
	{
		std::vector<std::string> res;
		std::string tmp;
		for (char c : str) {
			if (c == ch) {
				res.push_back(tmp);
				tmp = "";
			}
			else {
				tmp += c;
			}
		}
		if (tmp != "") {
			res.push_back(tmp);
		}
		return res;
	}
	static void attribute_adder(const std::string& filename, const std::string& poolname, const std::string& attri) {
		std::ifstream is(filename);
		std::vector<std::string> line;
		while (!is.eof()) {
			std::string temp = "";
			std::getline(is, temp);
			if (temp == "") {
				line.push_back(temp);
				continue;
			}
			auto conf_num = split(temp, ' ');
			if (conf_num[0] == poolname) {
				temp += (" " + attri);
			}
			line.push_back(temp);
		}
		is.close();
		std::ofstream os(filename);
		os.flush();
		for (int i = 0;i < line.size() - 1;i++) {
			os << line[i] << std::endl;
		}
		os << line[line.size() - 1];
		os.close();
	}

	static void help() {
		std::cout << "------------------------------This is a help for xx&&cc toy database------------------------------\n\
* SELECT * FROM ���� ����1 ֵ1 ����2 ֵ2...����n ֵn \n\
* INSERT INTO ���� ����1 ֵ1 ����2 ֵ2...����n ֵn\n\
* DELETE FROM ���� ����1 ֵ1 ����2 ֵ2...����n ֵn\n\
* UPDATE FROM ���� ����1 ֵ1 ����2 ֵ2...����n ֵn NEWVALUE ���� ֵ\n\
* SHOW DATABASES\n\
* SHOW CREATE DATABASES\n\
* ADD FROM ���� ������\n\
* SELECT FROM ���� ����1 ֵ1 ����2 ֵ2...����n ֵn; ���� ���� ����������һ�����еĲ���ֵ��note���趨idΪ����Ψһ���\n\
----------------------------------------------------------------------------------------------------\n";
	}
};