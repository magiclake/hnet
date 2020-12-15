#ifndef ___PACKS_HPP_hhhh__
#define ___PACKS_HPP_hhhh__
#include <functional>
#include <vector>
#include <string>
#include <chrono>
#include <exception>
#include <sstream>
#include "../json.hpp"
#include "../log.h"
namespace hnet
{
    namespace hrpc
    {
        using RpcData = std::vector<unsigned char>;

        struct RequestPack
        {
            std::string name;
            RpcData param;
            std::string toJson() const
            {
                using namespace nlohmann;
                    nlohmann::json j ;
                    j["name"] = name;
                    j["param"] = param;                    
                    return j.dump();
            }

            bool fromJson(const std::string &jString)
            {
                using namespace nlohmann;
                try
                {
                    json j = json::parse(jString);
                    j.at("name").get_to(name);
                    j.at("param").get_to(param);
                    return true;
                }
                catch(nlohmann::json::exception &e )
                {
                    HENT_LOG("fromJson error %s",e.what());
                    return false;
                }
            }
        };


        struct RspPack
        {
            std::string name;
            std::string reason;
            bool result;
            RpcData data;
        public:
            std::string toJson() const
            {
                using namespace nlohmann;
                    nlohmann::json j ;
                    j["name"] = name;
                    j["data"] = data;
                    j["result"] = result;
                    j["reason"] = reason;
                    return j.dump();
            }

            bool fromJson(const std::string &jString)
            {
                using namespace nlohmann;
                try
                {
                    json j = json::parse(jString);
                    j.at("name").get_to(name);
                    j.at("data").get_to(data);
                    j.at("result").get_to(result);
                    j.at("reason").get_to(reason);
                    return true;
                }
                catch(nlohmann::json::exception &e )
                {
                    HENT_LOG("fromJson error %s",e.what());
                    return false;
                }
            }
        };


        class rpc_error : public std::runtime_error
        {
        public:
            rpc_error(const std::string &msg) : std::runtime_error(msg){ }
        };

    }
}
#endif

