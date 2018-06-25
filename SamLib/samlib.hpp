/**
 * Project: SamLib 
 * @file samlib.hpp
 *
 * @brief Public header for SamLib
 *
 * @author Beri Levi
 * Contact: berilevi@gmail.org
 *
 */ 

#ifndef SAMLIB_H
#define SAMLIB_H
#include <iostream>
#include <string>
#include <vector>

class SamLib
{
private:
    std::string msResponse;
    std::string msHeader;
    std::string msIp;
    std::vector<std::string> mvecHeaders;
    char *mpcLastUrl;
    long mlResponseCode;
    double mdTransferTime;
    double mdTotalTransferTime;
    double mdConnectTime;
    double mdTotalConnectTime;
    double mdStartTime;
    double mdTotalStartTime;
    double mdNameLookupTime;
    double mdTotalNameLookupTime;
    void *mpvInst;
    int miErrorCode;
    int miReqCnt;

public:
    /**
     * @brief SamLib constructor, initializes the resources 
     */
    SamLib();
    /**
     * @brief SamLib destructor, releases the resources 
     */
    ~SamLib();
    /** 
        @brief gets response data from the last HTTP connection
        @return Returns a string containing the response 
    */
    std::string SamResponseData();
    /** 
        @brief gets headers from the last HTTP connection
        @return Returns a string containing the headers  
    */
    std::string SamResponseHeaders();
    /** 
        @brief function to get Ip of the last connected server
        @return Returns a string containing the Ip  
    */
    std::string SamLastConnectionIp();
    /** 
        @brief gets last error message if one of the APIs failed
        @return Returns a string containing an error message  
    */
    std::string SamLastErrorString();
    /** 
        @brief get total time of previous transfer 
        @return returns total time of the transfer time 
    */
    double SamTotalTransferTime();
    /** 
        @brief get total time time until connect
        @return returns total time until connect
    */
    double SamTotalConnectTime();
    /** 
        @brief get total time until the first byte is received
        @return returns total time until the first byte is received
    */
    double SamTotalStartTime();
    /** 
        @brief get total time of the name lookup time 
        @return returns total time of the name lookup time 
    */
    double SamTotalNameLookupTime();
    /** 
        @brief get the last received HTTP response code. 
        @return returns the last received HTTP response code. 
    */
    long SamResponseCode();
    /** 
        @brief set the number of HTTP requests to make
        @param number of requests
        @return returns the number of HTTP requests to make
    */
    void SamHttpRequestCount(int iReqCnt);
    /** 
        @brief do the actual HTTP get request to the url passed as an argument
        @param a string containing a fully qualified domain name
        @return 
    */
    bool SamHttpGet(std::string strUrl);
    /** 
        @brief allows injecting a custom HTTP header
        @param a string containing a custom header
        @return true for success false for error
    */
    void SamHttpHeaderAdd(std::string Header);
    
    void PrintStatistics();
    
};


#endif // SAMLIB_H
