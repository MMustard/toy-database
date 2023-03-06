#pragma once
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>
#include <iomanip>
#include <fstream>

using Attribute = std::string;

struct Base {
    std::map<Attribute, std::string> attributes;
    std::string get(Attribute attr) {
        if (attributes.count(attr) == 0) { 
            auto msg = "no attrribute named " + attr + "!";
            throw std::exception(msg.c_str()); 
        }
        return attributes[attr];
    }
    void set(Attribute attr, std::string value) {
        if (attributes.count(attr) == 0) {
            auto msg = "no attrribute named " + attr + "!";
            throw std::exception(msg.c_str());
        }
        attributes[attr] = value; 
    }
    Base(const std::vector<Attribute>& attrs) {
        for (auto&& attr : attrs) {
            attributes.insert({ attr, "" });
        }
    }
    Base(){}
};

struct BasePool {
    std::vector<Attribute> attributes;
    std::vector<Base> pool;
    BasePool() {
        std::cout << "default" << std::endl;
    }
    BasePool(const std::vector<Attribute>& attrs)
        : attributes(attrs) {}
    Base& newBase()
    {
        pool.push_back(Base(attributes));
        return pool.back();
    }
    std::vector<Base*> find(std::vector<std::pair<Attribute, std::string>> condtions)
    {
        std::vector<Base*> res;
        for (auto&& base : pool) {
            if(satisfied(condtions, base))
                res.push_back(&base);
        }
        return res;
    }

    void modify(std::vector<std::pair<Attribute, std::string>> condtions, Attribute attr, std::string val)
    {
        for (auto iter = pool.begin(); iter != pool.end();) {
            if (satisfied(condtions, *iter)) {
                iter->set(attr, val);
                iter++;
            }
            else {
                iter++;
            }
        }
    }

    void remove(std::vector<std::pair<Attribute, std::string>> condtions)
    {
        for (auto iter = pool.begin(); iter != pool.end();) {
            if (satisfied(condtions, *iter)) {
                iter = pool.erase(iter);
            }
            else {
                iter++;
            }
        }
    }

    void add(std::vector<std::pair<Attribute, std::string>> args)
    {
        auto& new_base = newBase();
        for (auto&& [attr, val] : args) {
            new_base.set(attr, val);
        }
    }

    void show_all()
    {
        for (auto&& attr : attributes) {
            std::cout << std::setw(10) << attr ;
        }
        std::cout << std::endl;
        for (auto&& base : pool) {
            for (auto&& attr : attributes) {
                std::cout << std::setw(10) << base.get(attr) ;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void save(std::string filename)
    {
        std::ofstream os(filename);
        for (auto&& base : pool) {
            for (auto&& attr : attributes) {
                if(base.get(attr)!="") 
                    os << " " << base.get(attr);
            }
            os << std::endl;
        }
        os.close();
    }
    private:
        auto satisfied(auto&& condtions, auto&& base) -> bool {
            for (auto&& [attr, val] : condtions) {
                if (base.get(attr) != val) {
                    return false;
                }
            }
            return true;
        }
};

struct DataBase {
    std::map<std::string, BasePool> bases;
    std::string conf_name;
    BasePool& getBasePool(std::string base_name) { return bases[base_name]; }
    void show_table()
    {
        for (auto&& [table_name, pool] : bases) {
            std::cout << table_name << std::endl;
        }
    }
    void createBasePool(
        std::string pool_name,
        std::initializer_list<Attribute> attrs
    )
    {
        bases.insert(
            {
                pool_name,
                BasePool(attrs)
            }
        );
    }
    DataBase(const std::string& conf): conf_name(conf)
    {
        std::ifstream is(conf);
        char tmp = 0;
        while (!is.eof()) {
            std::string key;
            std::vector<std::string> attrs;
            is >> key;
            if (key == "") break;
            is.get(tmp);
            while (tmp != '\n') {
                if (is.eof()) { break; }
                std::string attr;
                is >> attr;
                attrs.push_back(attr);
                is.get(tmp);
            }
            bases.insert(
                {
                    key,
                    attrs
                }
            );
        }
        for (auto& [name, pool] : bases) {
            std::ifstream is(name);

            while (!is.eof()) {
                char tmp = 0;
                std::vector<std::string> vals;
                while (tmp != '\n') {
                    if (is.eof()) { break; }
                    std::string val;
                    is >> val;
                    vals.push_back(val);
                    is.get(tmp);
                }
                if (vals.empty() || vals[0] == "") { break; }
                std::vector<std::pair<Attribute, std::string>> args;
                for (int i = 0; i < vals.size(); i++) {
                    args.push_back(
                        { pool.attributes[i], vals[i] }
                    );
                }
                
                for (int i = vals.size(); i < pool.attributes.size(); i++) {
                    args.push_back(
                        { pool.attributes[i], "" }
                    );
                }
                pool.add(args);
            }
            
            is.close();
        }
    }
    std::vector<Base*> find(std::string pool_name, const std::vector<std::pair<Attribute, std::string>>& condtions)
    {
        return bases[pool_name].find(condtions);
    }
    void add(std::string pool_name, std::vector<std::pair<Attribute, std::string>> args)
    {
        bases[pool_name].add(args);
    }
    void modify(std::string pool_name, std::vector<std::pair<Attribute, std::string>> condtions, Attribute attr, std::string val)
    {
        bases[pool_name].modify(condtions, attr, val);
    }
    void remove(std::string pool_name, std::vector<std::pair<Attribute, std::string>> condtions)
    {
        bases[pool_name].remove(condtions);
    }
    void save()
    {
        for (auto&& [name, pool] : bases)
        {
            bases[name].save(name);
        }
    }
    void show_all(std::string pool_name) {
        bases[pool_name].show_all();
    }
};