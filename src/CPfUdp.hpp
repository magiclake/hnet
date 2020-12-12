/*
 * @Author: magiclake
 * @Date: 2020-12-11 11:29:36
 * @LastEditTime: 2020-12-12 10:22:28
 * @LastEditors: huhai
 * @FilePath: /src/CUdpPfserver.hpp
 * @Description: 
 */
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <vector>
#include <chrono>
#include <thread>
#include <functional>
#include "Exception.h"
namespace hnet
{

    using UdpData = std::vector<unsigned char>;
    typedef struct sockaddr_un UdpId;

    class CUdpPfUnixSocket
    {
    protected:
        int socketfd;
        UdpId serverAddr;
        unsigned int addrLen;

        const char *GetErrorMsg()
        {
            return strerror(errno);
        }

        bool recvFrom(UdpId &client, UdpData &data)
        {
            char buffer[1000];
            socklen_t len = sizeof(client);
            int nRead = recvfrom(socketfd, (char *)buffer, sizeof(buffer),
                                 0, (struct sockaddr *)&client,
                                 &len);
            if (nRead <= 0)
            {
                return false;
            }
            data.assign(buffer, &buffer[nRead]);
            return true;
        }

        bool sendTo(const UdpId &client, const UdpData &data)
        {
            int nSend = sendto(socketfd, &data[0], data.size(), 0,
                               (const struct sockaddr *)&client, addrLen);
            if (nSend <= 0)
            {
                printf("send fail:%s\n", GetErrorMsg());
                return false;
            }
            return true;
        }

    public:
        CUdpPfUnixSocket(const std::string &path)
        {
            socketfd = -1;
            if (path.length() >= sizeof(serverAddr.sun_path))
            {
                throw network_error(std::string("Error path length > 108"));
            }

            serverAddr.sun_family = AF_UNIX;
            strcpy(serverAddr.sun_path, path.c_str());
            addrLen = sizeof(serverAddr); // strlen(serverAddr.sun_path) + sizeof(serverAddr.sun_family);
        }

        virtual ~CUdpPfUnixSocket()
        {
            if (socketfd != -1)
            {
                close(socketfd);
            }
        }
    };

    class IResponse
    {
    public:
        virtual bool response(const UdpData &data) = 0;
        virtual ~IResponse() {}
    };

    class UdpServerEvent
    {

    private:
        IResponse *responseFunction;
        friend class CUdpPFUnixServer;
        void setRsp(IResponse *rsp)
        {
            responseFunction = rsp;
        }

    protected:
        bool response(const UdpData &data)
        {
            if (responseFunction)
            {
                return responseFunction->response(data);
            }
            return false;
        }

    public:
        UdpServerEvent() : responseFunction(NULL) {}
        virtual bool dataRxEvent(UdpData &data) = 0;
    };

    class CUdpPFUnixServer : public CUdpPfUnixSocket, public IResponse
    {
        UdpId currentClient;
        bool isStop;

        virtual bool response(const UdpData &data) override
        {
            return sendTo(currentClient, data);
        }

        UdpServerEvent *data_rx_callback;

    public:
        CUdpPFUnixServer(const std::string &path, UdpServerEvent *dataInHandler) : CUdpPfUnixSocket(path),
                                                                                   data_rx_callback(dataInHandler)
        {
            unlink(path.c_str());
            socketfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
            if (socketfd < 0)
            {
                throw network_error(std::string("Error new socket: ") + GetErrorMsg());
            }

            if (bind(socketfd, (struct sockaddr *)&serverAddr, addrLen) == -1)
            {
                throw network_error(std::string("Error binding socket: ") + GetErrorMsg());
            }

            isStop = true;
            if (data_rx_callback)
            {
                data_rx_callback->setRsp(this);
            }
            else
            {
                throw network_error(std::string("NULL dataInHandler") + GetErrorMsg());
            }
        }

        void run_forever()
        {
            isStop = false;
            while (!isStop)
            {
                UdpData data;
                memset(&currentClient, 0, sizeof(currentClient));
                if (recvFrom(currentClient, data))
                {
                    //printf("run_forever rx :%s [%s]\n", currentClient.sun_path, &data[0]);
                    if (data_rx_callback != nullptr)
                    {
                        data_rx_callback->dataRxEvent(data);
                    }
                }
                else
                {
                    printf("read no data:%s\n", GetErrorMsg());
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    };

    class CUdpPfClient : public CUdpPfUnixSocket
    {
    public:
        CUdpPfClient(const char *path) : CUdpPfUnixSocket(path)
        {
            socketfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
            if (socketfd < 0)
            {
                throw network_error(std::string("Error new socket: ") + GetErrorMsg());
            }

            struct sockaddr_un localaddr;
            bzero(&localaddr, sizeof(localaddr));
            localaddr.sun_family = AF_LOCAL;
            strncpy(localaddr.sun_path, tmpnam(NULL), sizeof(localaddr.sun_path) - 1);
            if (bind(socketfd, (struct sockaddr *)&localaddr, sizeof(localaddr)) == -1)
            {
                throw network_error(std::string("Error bind: ") + GetErrorMsg());
            }
        }

        bool send(const UdpData &data)
        {
            return sendTo(serverAddr, data);
        }

        bool recv(UdpData &dataOut)
        {
            UdpId theserver;
            return recvFrom(theserver, dataOut);
        }
    };

    class CUdpPfTest
    {
        const char *PF_PATH = "/tmp/dev/ws/esam_pf";
        void udpClientTest(const char *sendData)
        {
            try
            {
                CUdpPfClient client(PF_PATH);
                UdpData data(sendData, sendData + strlen(sendData));
                data.push_back(0);
                int cnt = 1000;
                while (true)
                {
                    if (cnt-- == 0)
                    {
                        break;
                    }

                    if (!client.send(data))
                    {
                        printf("CUdpPfClient:send data fail\n");
                    }
                    else
                    {
                        printf("CUdpPfClient:send data %s OK\n", &data[0]);
                    }

                    UdpData outData;
                    if (!client.recv(outData))
                    {
                        printf("CUdpPfClient:recv data fail\n");
                    }
                    else
                    {
                        printf("CUdpPfClient:recv data %s OK\n", &outData[0]);
                    }
                }
            }
            catch (const hnet::network_error &err)
            {
                printf("CUdpPfClient error:%s\n", err.what());
            }
        }

        class testServerEvent : public UdpServerEvent
        {

        public:
            virtual bool dataRxEvent(UdpData &data)
            {
                printf("testServerEvent:recv data:[%s],now echo\n", &data[0]);
                return response(data);
            }
        };

        void udpServerTest()
        {
            try
            {
                testServerEvent eventHandler;
                CUdpPFUnixServer server(PF_PATH, &eventHandler);
                printf("server start\n");
                server.run_forever();
            }
            catch (const hnet::network_error &err)
            {
                printf("CUdpPfClient error:%s\n", err.what());
            }
        }

    public:
        int test(int argc, const char *argv[])
        {
            if (argc <= 1)
            {
                printf("usage:%s <-c/-s> [data string]\n", argv[0]);
                return -1;
            }
            if (strcmp(argv[1], "-c") == 0)
            {
                if (argc <= 2)
                {
                    printf("usage:%s <-c/-s> [data string]\n", argv[0]);
                    return -1;
                }
                udpClientTest(argv[2]);
            }
            else
            {
                udpServerTest();
            }
            return 0;
        }
    };

} // namespace hnet
