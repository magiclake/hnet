/*
 * @Author: 
 * @Date: 2020-12-12 11:09:59
 * @LastEditTime: 2020-12-14 16:14:06
 * @LastEditors: Please set LastEditors
 * @FilePath: /hnet/src/hrpc/hrpc.hpp
 * @Description: 
 */
#include <functional>
#include <vector>
#include <string>
#include <chrono>
#include <exception>
#include <sstream>
#include <string.h>
#include "RpcClient.hpp"
#include "RpcServer.hpp"
#include "transfer.hpp"
#include "services/EchoService.hpp"
namespace hnet
{
    namespace hrpc
    {
        const char *SERVER_PATH = "/tmp/testecho";
        class HRpcTest
        {

        public:
            int main(int argc, const char** argv)
            {
                if (argc <= 1)
                {
                    HENT_LOG("usage:%s <-c/-s> [data string]", argv[0]);
                    return -1;
                }
                if (strcmp(argv[1], "-c") == 0)
                {
                    if (argc <= 2)
                    {
                        HENT_LOG("usage:%s <-c/-s> [data string]", argv[0]);
                        return -1;
                    }
                    testClient(argc-1, ++argv);
                }
                else
                {
                    testServer();
                }
                return 0;

            }

            void testClient(int argc, const char* argv[])
            {
                if(argc <= 1)
                {
                    HENT_LOG("Useage:./%s -c echo.write/echo.read [data]");
                    return;
                }

                PfunixClientTransfer clientTransfer(SERVER_PATH);
                HRpcClient client(&clientTransfer);
                std::vector<unsigned char> param;
                std::vector<unsigned char> retValue;
                unsigned int timeOutMs = 5000;
                if(argc > 3)
                {
                    param.assign(argv[2],argv[2]+strlen(argv[2]));
                }

                if (!client.call(argv[1], param,  timeOutMs, retValue))
                {
                    HENT_LOG("rpc request fail");
                    return;
                }

                HENT_LOG("return:%s", &retValue[0]);
            }

            void testServer()
            {
                services::EchoService echoservice;
                HRpcServer server(SERVER_PATH);
                server.add(&echoservice);
                server.runForever();
            }
        };

    } // namespace hrpc

} // namespace hnet
