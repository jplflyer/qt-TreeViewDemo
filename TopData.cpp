#include "TopData.h"

using namespace std;

BaseData::BaseData(Type _type)
    : type(_type), parent(nullptr)
{
}

std::string
BaseData::typeAsString() const {
    switch(type) {
        case Type::TOP: return "TopData";
        case Type::MIDDLE: return "MiddleData";
        case Type::CHILD: return "ChildData";
    }

    return "Unknown";
}

TopData::TopData()
    : BaseData(Type::TOP)
{
}

TopData::TopData(const string &_name, const string &_status)
    : BaseData(Type::TOP), name(_name), status(_status)
{
}

MiddleData::MiddleData()
    : BaseData(Type::MIDDLE)
{
}

MiddleData::MiddleData(const string &_name, const string &_address)
    : BaseData(Type::MIDDLE), name(_name), address(_address)
{
}

ChildData::ChildData()
    : BaseData(Type::CHILD)
{
    type = Type::CHILD;
}

ChildData::ChildData(const string &_name, int _age)
    : BaseData(Type::CHILD), name(_name), age(_age)
{
}
