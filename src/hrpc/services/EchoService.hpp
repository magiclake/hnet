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
        printf("EchoService write:%s %d\n",&data[0], data.size());
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
        printf("EchoService rsp:%s\n",&data[0]);
        return true;
    }
public:
    bool exec(const RequestPack &req, RspPack &rsp)
    {
        printf("req.tojsonJ:%s\n",req.toJson().c_str());
        if(req.name ==  "echo.write")
        {
            printf("write :%s\n", &req.param[0]);
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
