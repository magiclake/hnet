#ifndef ___HRpcClient_HPP_hhhh__
#define ___HRpcClient_HPP_hhhh__
#include <vector>
#include <string>
#include <chrono>
#include <exception>
#include <sstream>
#include "transfer.hpp"
#include "packs.hpp"
namespace hnet
{
namespace hrpc
{

class HRpcClient
{
    ITransfer *transfer;
    bool doRequest(const std::string &data, time_t timeOutMs, std::string& ret)
    {
        if (data.empty())
        {
            HENT_LOG("empty data");
            return false;
        }

        if (!transfer->send(data))
        {
            return false;
        }

        return transfer->recv(timeOutMs, ret);
    }

public:
    HRpcClient(ITransfer *trans) :transfer(trans)
    {

    }

    bool call(const char* name,
              const std::vector<unsigned char> &param,
              time_t timeOutMs,
              std::vector<unsigned char> &retValue)
    {
        hnet::hrpc::RequestPack pack;
        pack.name = name;
        pack.param = param;
        std::string ret;
        if(!doRequest(pack.toJson(), timeOutMs, ret))
        {
            return false;
        }

        if(ret.empty())
        {
            HENT_LOG("rev  failed");
            return false;
        }
        RspPack response;
        if(!response.fromJson(ret))
        {
            return false;
        }

        if(!response.result)
        {
            return false;
        }
        retValue = response.data;
        return true;
    }

};
}
}
#endif

