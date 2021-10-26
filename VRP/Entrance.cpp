#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES

#include <glog/logging.h>

#include <iostream>
#include <string>
#include <tinyxml.h>

#include "ALNS.h"
#include "ALNS/BasicClass/ISolutionNode.h"
#include "Configuration.h"
#include "FerryVehicleTask.h"
#include "Header.h"
#include "IInformation.h"


using namespace std;

void welcomeInterface();
Configuration loadConfiguration();

void loadData();

bool loadXML();
void csv2task();

// 大领域搜索算法
void ALNS_Entrance();
void qt_environment();

int main() {
    google::InitGoogleLogging("VRP");
    FLAGS_alsologtostderr = true;
    FLAGS_colorlogtostderr = true;

    google::SetLogDestination(google::GLOG_INFO, "D:\\学习资料\\研究进度\\研二\\数据分析\\LOG\\INFO_");
    google::SetLogDestination(google::GLOG_WARNING, "D:\\学习资料\\研究进度\\研二\\数据分析\\LOG\\WARNING_");
    google::SetLogDestination(google::GLOG_ERROR, "D:\\学习资料\\研究进度\\研二\\数据分析\\LOG\\ERROR_");
    google::SetLogDestination(google::GLOG_FATAL, "D:\\学习资料\\研究进度\\研二\\数据分析\\LOG\\FATAL_");

    LOG(INFO) << "Welcome to use the Vehicle Routing Problem procedure!";

    welcomeInterface();
    Configuration configuration = loadConfiguration();
   
    // 读取数据
    loadData();
    ALNS alns_cur;
    alns_cur.start();
    return 0;
}

void qt_environment() {

}

void welcomeInterface()
{
   
}
