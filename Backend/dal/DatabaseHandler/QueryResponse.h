#ifndef QUERYRESPONSE_H
#define QUERYRESPONSE_H

#include <map>
#include <string>
#include <vector>

typedef std::map<std::string, std::string> row;

struct QueryResponse
{
public:
    bool ok;
    std::vector<row> *rows;

    ~QueryResponse()
    {
        delete rows;
    }
};

#endif