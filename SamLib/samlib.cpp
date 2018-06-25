/**
 * Project SamLib
 * @file samlib.cpp
 *
 * @brief Implementation file for SamLib API
 *
 * @author Beri Levi
 * Contact: berilevi@gmail.org
 *
 */ 
#include "samlib.hpp"
#include <iostream>
#include <curl/curl.h>
#include <string>

using std::cout;
using std::string;
using std::endl;

size_t WriteDataCallback(void *pvBuf, size_t size, size_t nMemb, void *pBuffer)
{
    if (pBuffer != NULL) 
    {
        ((string *)pBuffer)->append((char *)pvBuf, size * nMemb);
    }
    return size * nMemb;
}

/**
 * @brief SamLib constructor, initializes the resources 
 */
SamLib::SamLib()
{
    mpvInst = curl_easy_init();
    miReqCnt = 1;
    if (!mpvInst) 
    {
        throw std::string ("Unable to initialize library!");
    }
    /* switch off the progress meter */
    curl_easy_setopt(mpvInst, CURLOPT_NOPROGRESS, 1L);
    /* enable TCP keep-alive probing */
    curl_easy_setopt(mpvInst, CURLOPT_TCP_KEEPALIVE, 1L);
    /* set our custom user-agent field here */ 
    curl_easy_setopt(mpvInst, CURLOPT_USERAGENT, "libsam/1.0");
    /* send all data to this callback function  */ 
    curl_easy_setopt(mpvInst, CURLOPT_WRITEFUNCTION, WriteDataCallback);
    /* pass pointer to response string to the callback function */
    curl_easy_setopt(mpvInst, CURLOPT_WRITEDATA, &msResponse);
    /* Pass a pointer to be used to write the header part of the received data to. */
    curl_easy_setopt(mpvInst, CURLOPT_HEADERDATA, &msHeader);
    /* follow HTTP 3xx redirects */
    curl_easy_setopt(mpvInst, CURLOPT_FOLLOWLOCATION, 1L);    
}

/**
 * @brief SamLib destructor, releases the resources 
 */
SamLib::~SamLib()
{
    if (mpvInst)
    {
        /* cleanup curl stuff */ 
        curl_easy_cleanup(mpvInst);
        mpvInst = NULL;
    }
}

/** 
    @brief do the actual HTTP get request to the url passed as an argument
    @param a string containing a fully qualified domain name
    @return true for success false for error
*/
bool SamLib::SamHttpGet(string strUrl)
{
    CURLcode code = CURLE_OK;
    char *pcIp = NULL;
    struct curl_slist *list = NULL;
    /* reset times with each API call */
    mdTotalTransferTime = 0;
    mdTotalStartTime = 0;
    mdTotalNameLookupTime = 0;
    mdTotalConnectTime = 0;
    /* Set URL to get */ 
    curl_easy_setopt(mpvInst, CURLOPT_URL, strUrl.c_str());
    /* Inject custom HTTP headers if there are any set */
    if (mvecHeaders.size())
    {
        for (auto Header : mvecHeaders)
        {
            list = curl_slist_append(list, Header.c_str());
        }
        /* set custom HTTP headers */
        curl_easy_setopt(mpvInst, CURLOPT_HTTPHEADER, list);
    }
    /* do the actual http get request */
    for (int i = 0; i < miReqCnt; ++i)
    {
        miErrorCode = curl_easy_perform(mpvInst);
        if(CURLE_OK != miErrorCode) 
        {
            curl_slist_free_all(list); /* free the list again */
            return false;
        }
        /* get total time of previous transfer */
        code = curl_easy_getinfo(mpvInst, CURLINFO_TOTAL_TIME, &mdTransferTime);
        if (CURLE_OK == code)
        {
            mdTotalTransferTime += mdTransferTime;
        }
        /* get the time until the first byte is received */
        code = curl_easy_getinfo(mpvInst, CURLINFO_STARTTRANSFER_TIME, &mdStartTime);
        if (CURLE_OK == code)
        {
            mdTotalStartTime += mdStartTime;
        }
        /* get the name lookup time */
        code = curl_easy_getinfo(mpvInst, CURLINFO_NAMELOOKUP_TIME, &mdNameLookupTime);
        if (CURLE_OK == code)
        {
            mdTotalNameLookupTime += mdNameLookupTime;
        }
        /* get the time until connect */
        code = curl_easy_getinfo(mpvInst, CURLINFO_CONNECT_TIME, &mdConnectTime);
        if (CURLE_OK == code)
        {
            mdTotalConnectTime += mdConnectTime;
        }
        /* Pass a pointer to receive the last received HTTP response code. */
        code = curl_easy_getinfo(mpvInst, CURLINFO_RESPONSE_CODE, &mlResponseCode);
        if (code != CURLE_OK)
        {
            curl_slist_free_all(list); /* free the list again */
            return false;
        }
    }
    /* IP address of the last connection */
    code = curl_easy_getinfo(mpvInst, CURLINFO_PRIMARY_IP, &pcIp);
    if (CURLE_OK == code && pcIp)
    {
        msIp = pcIp;
    }
    curl_slist_free_all(list); /* free the list again */
    if (code == CURLE_OK)
    {
        return true;
    }
    return false;
}

/** 
    @brief gets last error message if one of the APIs failed
    @return Returns a string containing an error message  
*/
string SamLib::SamLastErrorString()
{
    return string(curl_easy_strerror((CURLcode)miErrorCode));
}

/** 
    @brief set the number of HTTP requests to make
    @param number of requests
    @return returns the number of HTTP requests to make
*/
void SamLib::SamHttpRequestCount(int iReqCnt)
{
    miReqCnt = iReqCnt;
}

/** 
    @brief get the last received HTTP response code. 
    @return returns the last received HTTP response code. 
*/
long SamLib::SamResponseCode()
{
    return mlResponseCode;
}

/** 
    @brief get total time of previous transfer 
    @return returns total time of the transfer time 
*/
double SamLib::SamTotalTransferTime()
{
    return mdTotalTransferTime;
}

/** 
    @brief get total time time until connect
    @return returns total time until connect
*/
double SamLib::SamTotalConnectTime()
{
    return mdTotalConnectTime;
}

/** 
    @brief get total time of the name lookup time 
    @return returns total time of the name lookup time 
*/
double SamLib::SamTotalNameLookupTime()
{
    return mdTotalNameLookupTime;
}

/** 
    @brief get total time until the first byte is received
    @return returns total time until the first byte is received
*/
double SamLib::SamTotalStartTime()
{
    return mdTotalStartTime;
}

/** 
    @brief gets headers from the last HTTP connection
    @return Returns a string containing the headers  
*/
std::string SamLib::SamResponseHeaders()
{
    return msHeader;
}

/** 
    @brief gets response data from the last HTTP connection
    @return Returns a string containing the response 
*/
std::string SamLib::SamResponseData()
{
    return msResponse;
}

/** 
    @brief function to get Ip of the last connected server
    @return Returns a string containing the Ip  
*/
std::string SamLib::SamLastConnectionIp()
{
    return msIp;
}

//SKTEST;<IP address of HTTP server>;<HTTP response code>;<median of CURLINFO_NAMELOOKUP_TIME>;<median of CURLINFO_CONNECT_TIME>;
//<median of CURLINFO_STARTTRANSFER_TIME>;<median of CURLINFO_TOTAL_TIME>
void SamLib::PrintStatistics()
{
    cout << "SKTEST;" << SamLastConnectionIp() << ";" << SamResponseCode() << ";" << SamTotalNameLookupTime() / miReqCnt << ";" <<
            SamTotalConnectTime() / miReqCnt << ";" << SamTotalStartTime() / miReqCnt << ";" << SamTotalTransferTime() / miReqCnt << endl;
}

void SamLib::SamHttpHeaderAdd(std::string Header)
{
    mvecHeaders.push_back(Header);
}


