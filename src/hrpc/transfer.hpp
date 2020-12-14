#ifndef ___ITransferr_HPP_hhhh__
#define ___ITransferr_HPP_hhhh__
#include <vector>
#include <string>
#include <chrono>
#include <exception>
#include <sstream>
#include "../CPfUdp.hpp"
#include "PfudpAdapter.hpp"
namespace hnet
{
namespace hrpc
{
class ITransfer
{
public:
    virtual ~ITransfer(){}
    virtual bool send(const std::string &streamIn) = 0;
    virtual bool recv(time_t timeOutMs,std::string &streamOut) = 0;
};

class PfunixClientTransfer : public ITransfer
{

    hnet::pfunix_udp::UdpData toData(const std::string &str)
    {
        PfudpAdapter adt;
        return adt.toData(str);
    }

    std::string toString(const hnet::pfunix_udp::UdpData &data)
    {
        PfudpAdapter adt;
        return adt.toString(data);
    }

public:
    hnet::pfunix_udp::CUdpPfClient client;

public:
    PfunixClientTransfer(const char* path):client(path){

    }

    virtual bool send(const std::string &streamIn) override
    {
        printf("PfunixClientTransfer send data\n");
        return client.send(toData(streamIn));
    }

    virtual bool recv(time_t timeOutMs, std::string &streamOut) override
    {
        hnet::pfunix_udp::UdpData data;
        if(!client.recv(timeOutMs, data))
        {
            printf("PfunixClientTransfer recive data error\n");
            return false;
        }
        streamOut = toString(data);
        return true;
    }
};
}}
#endif

