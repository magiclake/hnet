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
    bool doResponse(const RspPack &data)
    {
        PfudpAdapter adt;
        auto udata = adt.toData(data.toJson());
        return response(udata);
    }

    void doService(const RequestPack &req, RspPack &rsp)
    {
        bool found = false;
        for(auto &service : serviceList)
        {
            if(service->isMine(req.name))
            {
                rsp.result = service->exec(req, rsp);
                found = true;
                break;
            }
        }
        if(!found)
        {
            rsp.reason = "unfound service";
        }
    }

    void getResponse(const hnet::pfunix_udp::UdpData &data,
                 RspPack &rsp)
    {
        try {
            PfudpAdapter adt;
            std::string str = adt.toString(data);
            rsp.result = false;
            RequestPack req;
            if(!req.fromJson(str))
            {
                rsp.name = req.name+".rsp";
                rsp.reason="invalid json";
            }
            rsp.name = req.name+".rsp";
            doService(req,rsp);

        } catch (std::exception &err) {
            rsp.result = false;
            rsp.reason = err.what();
        }
    }

public:
    HRpcServiceProcessor():hnet::pfunix_udp::UdpServerEvent()
    {
    }

    virtual bool dataRxEvent(const hnet::pfunix_udp::UdpData &data)
    {
        if(data.size() == 0)
        {
            HENT_LOG("HRpcServiceProcessor handler error:data.size() = 0");
            return false;
        }
        RspPack rsp;
        getResponse(data,rsp);
        return doResponse(rsp);
    }

    bool add(services::IRpcService *service)
    {
        serviceList.push_back(service);
        return true;
    }
};
}}
#endif
