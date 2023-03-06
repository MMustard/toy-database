#pragma once
#include "base.h"

struct MangerMethodInterface {
	virtual void method(DataBase&, const std::vector<std::string>&) = 0;
};

struct ManagerMethodSelectImpl : public MangerMethodInterface {
	void method(DataBase& db, const std::vector<std::string>& order) override
	{
		if (order[1] == "*") {
			std::string poolname = order[3];
			std::vector<std::pair<Attribute, std::string>> condtions;
			for (int i = 4;i < order.size();i += 2) {
				condtions.push_back({ order[i], order[i + 1] });
			}

			auto find_result = db.find(poolname, condtions);
			for (auto&& attr : db.bases[poolname].attributes) {
				std::cout << std::setw(10) << attr;
			}
			std::cout << std::endl;
			for (auto&& resultptr : find_result) {
				for (auto&& attr : db.bases[poolname].attributes)
					std::cout << std::setw(10) << resultptr->get(attr);
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
		else {
			std::string poolname1 = order[2];
			std::string poolname2;
			std::string new_attr;
			std::vector<std::pair<Attribute, std::string>> condtions1;
			for (int i = 3;i < order.size();i += 2) {
				condtions1.push_back({ order[i], order[i + 1] });
				if (order[i + 2] == ";") {
					poolname2 = order[i + 3];
					new_attr = order[i + 4];
					break;
				}
			}
			std::vector<Base*> find_result1 = db.find(poolname1, condtions1);//此处假设vector中有唯一值
			std::vector<std::pair<Attribute, std::string>> condtions2;
			condtions2.push_back({ "id", (find_result1[0]->attributes)["id"] });
			std::vector<Base*> find_result2 = db.find(poolname2, condtions2);//此处假设vector中有唯一值
			std::cout << new_attr << " attribute of table " << poolname1 << " element in table " << poolname2 << " is " << (find_result2[0]->attributes)[new_attr] << std::endl;
		}
	}
};

struct ManagerMethodInsertImpl : public MangerMethodInterface
{
	void method(DataBase& db, const std::vector<std::string>& order) override
	{
		std::string poolname = order[2];
		std::vector<std::pair<Attribute, std::string>> args;
		for (int i = 3;i < order.size();i += 2) {
			args.push_back({ order[i], order[i + 1] });
		}
		db.add(poolname, args);
		db.save();
	}
};

struct ManagerMethodDeleteImpl : public MangerMethodInterface
{
	void method(DataBase& db, const std::vector<std::string>& order) override
	{
		std::string poolname = order[2];
		std::vector<std::pair<Attribute, std::string>> condtions;
		for (int i = 3;i < order.size();i += 2) {
			condtions.push_back({ order[i], order[i + 1] });
		}
		db.remove(poolname, condtions);
		db.save();
	}
};

struct ManagerMethodUpdateImpl : public MangerMethodInterface
{
	void method(DataBase& db, const std::vector<std::string>& order) override
	{
		std::string poolname = order[2];
		std::vector<std::pair<Attribute, std::string>> condtions;
		Attribute attr;
		std::string val;
		for (int i = 3;i < order.size();i += 2) {
			condtions.push_back({ order[i], order[i + 1] });
			if (order[i + 2] == "NEWVALUE") {
				attr = order[i + 3];
				val = order[i + 4];
				break;
			}
		}
		db.modify(poolname, condtions, attr, val);
		db.save();
	}
};

struct ManagerMethodShowImpl : public MangerMethodInterface
{
	void method(DataBase& db, const std::vector<std::string>& order) override
	{
		if (order[1] == "CREATE") {
			std::string poolname = order[2];
			db.show_all(poolname);
		}
		else {
			for (auto&& [poolname, basepool] : db.bases) {
				db.show_all(poolname);
			}
		}
	}
};

struct ManagerMethodAddImpl : public MangerMethodInterface
{
	void method(DataBase& db, const std::vector<std::string>& order) override
	{
		Utils::attribute_adder(db.conf_name, order[2], order[3]);
		db.bases[order[2]].attributes.push_back(order[3]);
		for (auto&& base : db.bases[order[2]].pool) {
			base.attributes.insert({ order[3],"" });
		}
	}
};

struct ManagerMethodHelpImpl : public MangerMethodInterface
{
	void method(DataBase& db, const std::vector<std::string>& order) override
	{
		Utils::help();
	}
};

struct MangerMethodFactory {
	static std::unique_ptr<MangerMethodInterface> get(const std::string& method_name)
	{
		if (method_name == "SELECT") return std::make_unique<ManagerMethodSelectImpl>();
		if (method_name == "INSERT") return std::make_unique<ManagerMethodInsertImpl>();
		if (method_name == "UPDATE") return std::make_unique<ManagerMethodUpdateImpl>();
		if (method_name == "DELETE") return std::make_unique<ManagerMethodDeleteImpl>();
		if (method_name == "SHOW")   return std::make_unique<ManagerMethodShowImpl>();
		if (method_name == "ADD")    return std::make_unique<ManagerMethodAddImpl>();
		if (method_name == "HELP")   return std::make_unique<ManagerMethodHelpImpl>();
	}
};