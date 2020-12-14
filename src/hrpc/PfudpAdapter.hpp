#ifndef __PfudpAdapterPfudpAdapter__
#define __PfudpAdapterPfudpAdapter__
#include "../CPfUdp.hpp"
namespace hnet {
namespace hrpc {

class PfudpAdapter
{

public:
    std::string toString(const hnet::pfunix_udp::UdpData &data)
    {
        auto d = data;
        d.push_back(0);
        return (const char*)&d[0];
    }

    hnet::pfunix_udp::UdpData toData(const std::string &str)
    {
        hnet::pfunix_udp::UdpData ret;
        for(auto &s : str)
        {
            ret.push_back(static_cast<unsigned char>(s));
        }
        return ret;
    }
};
}
}
#endif
