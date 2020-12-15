<!--
 * @Author: 
 * @Date: 2020-12-12 10:48:01
 * @LastEditTime: 2020-12-15 17:32:33
 * @LastEditors: Please set LastEditors
 * @FilePath: /hnet/README.md
 * @Description: 
-->
# hnet
net libs


### pfunix udp

whole code consists of a single header file `CPfUdp.hpp`


- build test:
    ```bash
    cd src && mkdir build
    cd build
    cmake .. && make
    ```
    u will find binary in build/bin.   
        run server: `./udpPFTest -s`  
        run client: `./udpPFTest -c "test words"`  


- server:   
   1.  create data handler
   ``` C++
        class testServerEvent : public hnet::UdpServerEvent
        {

        public:
            virtual bool dataRxEvent(UdpData &data)
            {
                printf("testServerEvent:recv data:[%s],now echo\n", &data[0]);
                // use this funciont to send data back if u need
                return response(data);
            }
        };
    ```

    2. run forever
    ```C++
            using namespace hnet;
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
    ```
- client:

    ``` C++
    using namespace hnet;
    CUdpPfClient client(PF_PATH);
    UdpData data(sendData, sendData + strlen(sendData));

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
    ```


### pfunix hrpc

- rpc server  
    1. first create service
```C++
    #include "IRpcService.hpp"
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

```
    2.  reg to server and run  

```C++
    services::EchoService echoservice;
    HRpcServer server(SERVER_PATH);
    server.add(&echoservice);
    server.runForever();
```

- rpc client

    ```C++
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
```