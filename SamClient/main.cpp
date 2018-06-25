/**
 * Project: SamLib test application
 * @file main.cpp
 *
 * @brief Implementation file for SamLib test application
 *
 * @author Beri Levi
 * Contact: berilevi@gmail.org
 *
 */
#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include "samlib.hpp"

using namespace std; 

void Usage()
{
    cout << "Usage:\n-H \"Header-name: Header-value\": can be used multiple times, each time specifying an extra HTTP header to add to your request" << endl
         << "-n <integer>: number of HTTP requests to make (i.e. the number of samples you will have to take the median of)" << endl
         << "-u Url to connect" << endl;
}

void PrintMedianStatistics(SamLib & slib, int iReqCnt)
{
    cout << "SKTEST;" << slib.SamLastConnectionIp() << ";" << slib.SamResponseCode() << ";" << slib.SamTotalNameLookupTime() / iReqCnt << ";" <<
            slib.SamTotalConnectTime() / iReqCnt << ";" << slib.SamTotalStartTime() / iReqCnt << ";" << slib.SamTotalTransferTime() / iReqCnt << endl;
}

int main(int argc, char *argv[])
{
    int iOpt = 0, iConnReqs = 1;
    string Url;
    string sHeader;
    std::vector<string> vecHeaders;
    while ((iOpt = getopt(argc, argv, "n:H:u:")) != -1) 
    {
        switch (iOpt) 
        {
            case 'n':
                iConnReqs = atoi(optarg);
                break;
            case 'H':
                sHeader = optarg;
                cout << sHeader << endl;
                vecHeaders.push_back(sHeader);
                break;
            case 'u':
                Url = optarg;
                break;
            default: /* '?' */
                Usage();
                exit(EXIT_FAILURE);
        }
    }
    if (Url.empty())
    {
        cout << "Error url option cant be empty!" << endl;
        Usage();
        exit(EXIT_FAILURE);
    }
    try 
    {
        SamLib slib;
        if (vecHeaders.size())
        {
            for (auto header : vecHeaders)
            {
                slib.SamHttpHeaderAdd(header);
            }
        }
        slib.SamHttpRequestCount(iConnReqs);
        bool ret = slib.SamHttpGet(Url);
        if (ret == false)
        {   
            cout <<  "Test aborted!" << endl;
            cout << slib.SamLastErrorString() << endl;
        }
        else
        {
            //cout << slib.SamResponseData() << endl;
            //cout << "Header Debug: "<< slib.SamResponseHeaders() << endl;
            

            PrintMedianStatistics(slib, iConnReqs);
        }
    }
    catch (string & s)
    {
        cerr << s << endl;
    }
    return 0;
}
