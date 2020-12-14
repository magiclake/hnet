/*
 * @Author: magiclake
 * @Date: 2020-12-11 14:53:20
 * @LastEditTime: 2020-12-14 15:53:42
 * @LastEditors: Please set LastEditors
 * @FilePath: /hnet/src/udpPfUnixTest.cc
 * @Description: 
 */

#include "CPFUdp.hpp"
int main(int argc, const char* argv[])
{
    hnet::pfunix_udp::CUdpPfTest test;
    return test.test(argc, argv);
}
