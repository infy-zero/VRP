#include <stdio.h> 
#include <memory.h>
#include <iostream>  
#include <ctime>
#include <string>

using namespace std;

time_t strTime2unix(std::string timeStamp)
{
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    string tmp = timeStamp;
    sscanf_s(timeStamp.c_str(), "%d/%d/%d %d:%d:%d",
        &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
        &tm.tm_hour, &tm.tm_min, &tm.tm_sec);

    tm.tm_year -= 1900;
    tm.tm_mon--;

    return mktime(&tm);
}