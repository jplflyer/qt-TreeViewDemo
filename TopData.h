#ifndef TOPDATA_H
#define TOPDATA_H

#include <string>

#include "PointerVector.h"

class BaseData {
public:
    enum class Type { TOP, MIDDLE, CHILD };
    Type type;
    BaseData *parent;

    BaseData(Type _type);

    std::string typeAsString() const;
};

class ChildData: public BaseData
{
public:
    ChildData();
    ChildData(const std::string &_name, int _age);

    std::string name;
    int age;
};

class MiddleData: public BaseData
{
public:
    MiddleData();
    MiddleData(const std::string &_name, const std::string &_address);

    std::string name;
    std::string address;

    PointerVector<ChildData> children;

    ChildData *createChildData(const std::string &_name, int _age) {
        ChildData *cd = new ChildData(_name, _age);
        cd->parent = this;
        children.push_back(cd);
        return cd;
    }
};

class TopData: public BaseData
{
public:
    TopData();
    TopData(const std::string &_name, const std::string &_status);

    std::string name;
    std::string status;

    PointerVector<MiddleData> middleData;

    MiddleData *createMiddleData(const std::string &_name, const std::string &_address) {
        MiddleData *md = new MiddleData(_name, _address);
        md->parent = this;
        middleData.push_back(md);
        return md;
    }
};

#endif // TOPDATA_H
