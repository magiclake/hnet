#ifndef __SERVICE____I__HPP___
#define __SERVICE____I__HPP___
#include <string>
#include "../packs.hpp"
namespace hnet {
namespace hrpc {
namespace services {
class IRpcService
{
public:
    virtual ~IRpcService(){}
    virtual bool isMine(const std::string &name) = 0;
    virtual bool exec(const RequestPack &req, RspPack &rsp) = 0;
};
}
}
}


#endif

