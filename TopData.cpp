#include "TopData.h"

using namespace std;

TopData::TopData()
{
}

TopData::TopData(const string &_name, const string &_status)
    : name(_name), status(_status)
{
}


MiddleData::MiddleData() {
}

MiddleData::MiddleData(const string &_name, const string &_address)
    : name(_name), address(_address)
{

}

ChildData::ChildData() {

}

ChildData::ChildData(const string &_name, int _age)
    : name(_name), age(_age)
{
}
