/*
 * @Author: magiclake
 * @Date: 2020-12-11 14:53:20
 * @LastEditTime: 2020-12-12 11:22:28
 * @LastEditors: Please set LastEditors
 * @FilePath: /hnet/src/udpPfUnixTest.cc
 * @Description: 
 */

#include "CPFUdp.hpp"
int main(int argc, const char* argv[])
{
    hnet::CUdpPfTest test;
    return test.test(argc, argv);
}
