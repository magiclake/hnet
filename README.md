<!--
 * @Author: 
 * @Date: 2020-12-12 10:48:01
 * @LastEditTime: 2020-12-12 10:57:41
 * @LastEditors: huhai
 * @FilePath: /hnet/README.md
 * @Description: 
-->
# hnet
net libs


### pfunix udp

 whole code consists of a single header file `CPfUdp.hpp`

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

