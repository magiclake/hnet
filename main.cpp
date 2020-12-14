#include "src/hrpc/packs.hpp"
#include "src/hrpc/test.hpp"
#include "stdio.h"
int main(int argc, const char**argv)
{
    hnet::hrpc::RequestPack pack;
    pack.name = "test-name";
    pack.param.push_back(1);
    pack.param.push_back(2);
    pack.param.push_back(3);
    pack.param.push_back(4);

    auto s = pack.toJson();
    printf("tojson:%s\n",s.c_str());
    pack.fromJson(s);

    hnet::hrpc::HRpcTest test;
    test.main(argc,argv);
    return 0;
}

