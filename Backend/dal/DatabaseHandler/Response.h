#ifndef RESPONSE_H
#define RESPONSE_H

#include <vector>

enum responseCode
{
    SUCCESS,
    NOT_FOUND,
    INTERNAL_ERROR
};

template <typename T>
struct Response
{
    int status;
    T *result;

    Response()
    {
        result = nullptr;
    }

    ~Response()
    {
        if (result != nullptr)
        {
            delete result;
        }
    }
};

#endif