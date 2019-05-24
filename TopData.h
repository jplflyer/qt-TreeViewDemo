#ifndef TOPDATA_H
#define TOPDATA_H

#include <string>

#include "PointerVector.h"

class ChildData
{
public:
    ChildData();
    ChildData(const std::string &_name, int _age);

    std::string name;
    int age;
};

class MiddleData
{
public:
    MiddleData();
    MiddleData(const std::string &_name, const std::string &_address);

    std::string name;
    std::string address;

    PointerVector<ChildData> children;

    ChildData *createChildData(const std::string &_name, int _age) {
        ChildData *cd = new ChildData(_name, _age);
        children.push_back(cd);
        return cd;
    }
};

class TopData
{
public:
    TopData();
    TopData(const std::string &_name, const std::string &_status);

    std::string name;
    std::string status;

    PointerVector<MiddleData> middleData;

    MiddleData *createMiddleData(const std::string &_name, const std::string &_address) {
        MiddleData *md = new MiddleData(_name, _address);
        middleData.push_back(md);
        return md;
    }
};

#endif // TOPDATA_H
