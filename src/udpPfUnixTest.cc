/*
 * @Author: magiclake
 * @Date: 2020-12-11 14:53:20
 * @LastEditTime: 2020-12-12 10:21:41
 * @LastEditors: 
 * @FilePath: /src/udpPfUnixTest.cc
 * @Description: 
 */

#include "CUdpPfserver.hpp"
int main(int argc, const char* argv[])
{
    hnet::CUdpPfTest test;
    return test.test(argc, argv);
}
