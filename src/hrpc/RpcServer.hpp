#ifndef ___HRpcServer_HPP_hhhh__
#define ___HRpcServer_HPP_hhhh__
#include <functional>
#include <vector>
#include <string>
#include <chrono>
#include <exception>
#include <sstream>
#include <memory>
#include "../CPfUdp.hpp"
#include "HRpcServiceProcessor.hpp"
#include "services/IRpcService.hpp"
namespace hnet
{
namespace hrpc
{
class HRpcServer
{
    HRpcServiceProcessor *processor;
    hnet::pfunix_udp::CUdpPFUnixServer *udpServer;
public:
    HRpcServer(const char* path)
    {
        processor = new HRpcServiceProcessor();
        udpServer = new hnet::pfunix_udp::CUdpPFUnixServer(path,processor);
    }

    virtual ~HRpcServer(){
        if(udpServer != nullptr)
        {
            delete udpServer;
            udpServer = nullptr;
        }

        if(processor != nullptr)
        {
            delete  processor;
            processor = nullptr;
        }
    }

    bool add(services::IRpcService *service)
    {
        return processor->add(service);
    }

    void runForever()
    {
        udpServer->run_forever();
    }

    void stop()
    {
        udpServer->stop();
    }
};
}}
#endif
