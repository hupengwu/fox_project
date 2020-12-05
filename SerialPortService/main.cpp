#include <cstdio>
#include <map>
#include <string>

#include <STLMapUtils.h>
#include <STLListUtils.h>
#include <STLVectorUtils.h>


#include <FoxDateTime.h>




#include <FoxString.h>
#include <FoxStringHelper.h>

#include <FoxDateTime.h>
#include <FoxByteArray.h>

#include "CUDPItemClientSocket.h"

#include <UUIDUtils.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "CJsonObject.hpp"
#include <FoxStty.h>
#include <fox_json_dict.h>

#include <CSemaphoreTest.h>

using namespace std;
using namespace fox_json;


int main()
{
    CSemaphoreTest::test();

    fox_json::JsonDict jd;
    name_space_def ss1;
    ss1.nameSpace = "abc";
    jd.putNameSpaceDef(ss1);
    name_space_def ss123 = jd.getNameSpaceDef("abc1");
    if (ss123.nameSpace=="")
    {
        int i1 = 0;
    }
    FoxStty::demo();

  //  test1();

   int lth = sizeof(unsigned long int);
   lth = sizeof(unsigned long long);

   string uuid = UUIDUtils::generateUUID();
   uuid = UUIDUtils::generateUUID();
   uuid = UUIDUtils::generateUUID();

   FoxByteArray byArr;
   byArr.Add(1);
   byArr.Add(2);
   BYTE by = byArr[0];
   by = byArr[1];
   by = byArr[2];



   FoxDateTime dat = FoxDateTime::getCurrentTime();
   FoxString s =  dat.format("%y-%m-%d %h:%M:%s");



    FoxString str = "abc";
    str.format("%s.%s", "1", "2");
    str.format("%d.%d", 1, 2);

    string aaa = "aaa";
    string bbb = "bbb";
    aaa.swap(bbb);



    std::vector<FoxString> vexString;
    FoxStringHelper::Split("abcd,abc,123", ",", vexString);

    map<string, int>  k2v;
    k2v["1"] = 1;
    k2v["2"] = 2;
    k2v["3"] = 3;

    map<int, string> v2k = STLMapUtils<string, int>::swapKeyValue(k2v);
    set<string> keys = STLMapUtils<string, int>::getKeySet(k2v);
    list<int> lists = STLMapUtils<string, int>::getValueList(k2v);
    vector<int> vectors = STLListUtils::list2vector(lists);
    set<int> sets = STLListUtils::list2set(lists);
    lists = STLListUtils::set2list(sets);
    lists = STLListUtils::vector2list(vectors);

    sets = STLVectorUtils::vector2set(vectors);
    lists = STLVectorUtils::vector2list(vectors);
    vectors = STLVectorUtils::list2vector(lists);
    vectors = STLVectorUtils::set2vector(sets);


    CUDPItemClientSocket udpsocket;
    BOOL ret = udpsocket.Create(6666, SOCK_DGRAM);
    if (!ret)
    {
        std::cout << "failed" << std::endl;
    }

    udpsocket.Start();

    FoxPThreadModule::Sleep(1000000000);



    printf("%s 向你问好!\n", "SerialPortService");
    return 0;
}

