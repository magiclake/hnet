#ifndef __EchoServiceEchoService_______
#define __EchoServiceEchoService_______
#include "IRpcService.hpp"
#include "../packs.hpp"
namespace hnet {
namespace hrpc {
namespace services {
class EchoService :public IRpcService
{
    RpcData currentData;
private:

    bool write(const RpcData &data)
    {
        HENT_LOG("EchoService write:%s %d",&data[0], data.size());
        currentData = data;
        return true;
    }

    bool read(RpcData &data)
    {
        data.clear();
        data.push_back('r');
        data.push_back('s');
        data.push_back('p');
        data.push_back(':');
        data.insert(data.end(),currentData.begin(),currentData.end());
        HENT_LOG("EchoService rsp:%s",&data[0]);
        return true;
    }
public:
    virtual bool isMine(const std::string &name) override
    {
        if(name == "echo.write" || name == "echo.read")
        {
            return true;
        }
        return false;
    }

    bool exec(const RequestPack &req, RspPack &rsp)
    {
        HENT_LOG("req.tojsonJ:%s",req.toJson().c_str());
        if(req.name ==  "echo.write")
        {
            HENT_LOG("write :%s", &req.param[0]);
            return write(req.param);
        }
        else if(req.name == "echo.read")
        {
            rsp.data.clear();
            return read(rsp.data);
        }
        else
        {
            return false;
        }
    }
};
}}}
#endif
