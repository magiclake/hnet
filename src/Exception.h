/*
 * @Author: magiclake 
 * @Date: 2020-12-12 10:05:37
 * @LastEditTime: 2020-12-12 10:21:59
 * @LastEditors: huhai
 * @FilePath: /src/Exception.h
 * @Description: 
 */
#ifndef __EXCEPTION_H_INCL__
#define __EXCEPTION_H_INCL__

#include <stdexcept>

namespace hnet
{
    /**
    * the class is to thrown when network error occured
    * author IVY (2009-05-21)
    */
    class network_error : public std::runtime_error
    {
    public:
        network_error(const std::string& msg):std::runtime_error(msg)
        {

        };
    };
}

#endif
