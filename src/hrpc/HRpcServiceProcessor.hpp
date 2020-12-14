#ifndef ___HRpcServiceProcessor_HPP_hhhh__
#define ___HRpcServiceProcessor_HPP_hhhh__
#include <vector>
#include <string>
#include <chrono>
#include <exception>
#include <sstream>
#include "services/IRpcService.hpp"
#include "../CPfUdp.hpp"
#include "PfudpAdapter.hpp"
#include "packs.hpp"
namespace hnet
{
namespace hrpc
{
class HRpcServiceProcessor : public hnet::pfunix_udp::UdpServerEvent
{
protected:
    std::vector<services::IRpcService *>serviceList;
    bool sendBack(const  std::string &data)
    {
        PfudpAdapter adt;
        auto udata = adt.toData(data);
        return response(udata);
    }

public:
    HRpcServiceProcessor():hnet::pfunix_udp::UdpServerEvent()
    {
        printf("rsp %p 1\n", responseFunction);
    }

    virtual bool dataRxEvent(const hnet::pfunix_udp::UdpData &data)
    {
        printf("rsp %p 2\n", responseFunction);
        if(data.size() == 0)
        {
            return false;
        }
        PfudpAdapter adt;
        auto str = adt.toString(data);
        RequestPack req;
        if(!req.fromJson(str))
        {
            return false;
        }

        RspPack rsp;
        rsp.name = req.name+".rsp";
        rsp.result = false;
        for(auto &service : serviceList)
        {
            rsp.result = service->exec(req, rsp);
        }
        printf("HRpcServiceProcessor exec ret\n" );
        auto outString = rsp.toJson();
        printf("HRpcServiceProcessor tojson\n" );
        auto outData = adt.toData(outString);
        printf("HRpcServiceProcessor response:%s %d\n",outString.c_str(),outData.size() );
        return response(outData);
    }

    bool add(services::IRpcService *service)
    {
        serviceList.push_back(service);
        return true;
    }
};
}}
#endif
