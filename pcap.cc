/////////////////////////// example /////////////////////
///////////////////// header file ////////////////////////////////
#include <modsecurity/modsecurity.h>
#include <pcap.h>
#include <iostream>
#include <fstream>
#include <string>
// Structs for processing
#include <netinet/ether.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
// inet_aton
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "pcre.h"

#ifndef ETHERTYPE_IP6
#define ETHERTYPE_IP6 0x86dd
#endif

class Packet
{
    public:
        Packet(pcap_pkthdr *header, const u_char *data, int verbose);
        int parsePacket();
        int setTCPIPdata();
        int extractHTTP();
        const char* src;
        const char* dst;
        int srcprt;
        int dstprt;
        
    private:
        int _verbose;
        pcap_pkthdr *_header;
        const u_char *_data;
        struct ether_header* _eth;
        struct ip *_ip;
        struct ip6_hdr *_ip6;
        struct tcphdr *_tcp;
        int _dataLength;
        // ModSecurity engine class
        //static ModSecurity::ModSecurity _modsec;
      
       // modsecurity rules
       //static ModSecurity::Rules _rules;
 
       // modsecurity transaction object
       //ModSecurity::Assay *_pmodsecAssay;
};

// Our constructor that collects the packet header and data
Packet::Packet(pcap_pkthdr *header, const u_char *data, int verbose)
{
    _header = header;
    _data = data;
    _verbose = verbose;
}

// Returns 1 if there is an error
int Packet::parsePacket()   
{
    // Assume Ethr and save the header
    _eth = (struct ether_header*)_data;
    if(_verbose){
        std::cout << "[+] Parsed out Ethernet header" << std::endl;
    }
    // Check if we have a TCP header
    // This is an ugly way to check if we have ipv6 or not
    int hasTCP = 0;

    // Check that we have an IP type packet
    if(ntohs(_eth->ether_type) == ETHERTYPE_IP){
        // Save the IP header
        _ip = (struct ip*)(_data+sizeof(struct ether_header));
        if(_ip->ip_p == IPPROTO_TCP){
            hasTCP = 1;
        }
        if(_verbose){
            std::cout << "[+] Parsed out IP header" << std::endl;
        }
    }else if(ntohs(_eth->ether_type) == ETHERTYPE_IP6){
        _ip6 = (struct ip6_hdr *)(_data+sizeof(struct ether_header));
        if(_ip6->ip6_nxt == IPPROTO_TCP){
            hasTCP = 1;
        }
        if(_verbose){
            std::cout << "[+] Parsed out IPv6 header" << std::endl;
        }
    }else{
        return 1;
    }

    // If we have TCP extract the header
    if(hasTCP){
        _tcp = (struct tcphdr*)(_data+sizeof(struct ether_header)+sizeof(struct ip));
        if(_verbose){
            std::cout << "[+] Parsed out TCP header" << std::endl;
        }
        // Overwrite the data we have with the remaining data
        _data = (u_char*)(_data + sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));
        _dataLength = _header->len - (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));
        if(_verbose){
            std::cout << "[+] Remaining data is " << _dataLength << " bytes long" << std::endl;
        }
    }else{
        return 1;
    }
}

int Packet::setTCPIPdata()   
{

    // Check for IPv4 header and extract addresses
    if(_ip->ip_v == 4){
        char sourceIP[INET_ADDRSTRLEN];
        char destIP[INET_ADDRSTRLEN];
        const char* ret1 = inet_ntop(AF_INET, &_ip->ip_src, sourceIP, sizeof(sourceIP));
        const char* ret2 =inet_ntop(AF_INET, &_ip->ip_dst, destIP, sizeof(destIP));
        if(ret1 == NULL || ret2 == NULL){
            return 1;
        }
        src = sourceIP;
        dst = destIP;
    }
    // Check for IPv6 header and extract addresses
    if(_ip->ip_v == 6){
        char sourceIP[INET6_ADDRSTRLEN];
        char destIP[INET6_ADDRSTRLEN];
        const char* ret1 = inet_ntop(AF_INET6, &_ip->ip_src, sourceIP, sizeof(sourceIP));
        const char* ret2 = inet_ntop(AF_INET6, &_ip->ip_dst, destIP, sizeof(destIP));
        if(ret1 == NULL || ret2 == NULL){
            return 1;
        }
        src = sourceIP;
        dst = destIP;
    }
    srcprt = ntohs(_tcp->source);
    dstprt = ntohs(_tcp->dest);
    if(_verbose){
        std::cout << "[+] Extracted source IP - " << src << " and source port " << srcprt << std::endl;
        std::cout << "[+] Extracted dest IP - " << dst << " and dest port " << dstprt << std::endl;
    }
}

int Packet::extractHTTP()   
{
  return 0;

}

             

int checkArgs(int argc, char** argv, int* verboseRef, std::string* filenameRef)
{
    std::stringstream usage;
    usage << "Usage: " << argv[0] << " [OPTIONS]... [FILE]";

    // Check the number of parameters
    if (argc < 2) {
        std::cerr << usage.str() << std::endl;
        return 1;
    }else{
        *verboseRef = 0;
        if(argc > 2){
            int knownArg = 0;
            // Check if our arguments are valid (except first and last)
            for(int i=1;i<argc-1;i++){
                knownArg = 0;
                // Does everything start with a '-'
                if(argv[i][0] != '-'){
                    std::cerr << usage.str() << std::endl;
                    return 1;
                }
                // ARe they all two chars long?
                if(strnlen(argv[i],5) != 2){
                    std::cerr << usage.str() << std::endl;
                    return 1;
                }
                // Set our verbose argument
                if(argv[i][1] == 'v'){
                    *verboseRef = 1;
                    knownArg = 1;
                }
                // If we get an unknown arg, exit
                if(knownArg == 0){
                    std::cerr << usage.str() << std::endl;
                    return 1;
                }
            }
        }
        // Check if we can find the file
        if(strnlen(argv[argc-1],256) < 255){
            std::string filename = argv[argc-1];
            std::ifstream ifile(filename);
            if (ifile) {
                if(*verboseRef == 1){
                    std::cout << "[+] We were able to succesfully open " << filename << std::endl;
                }
                *filenameRef = filename;
                return 0;
            }else{
                std::cerr << "Error: Could not open/find the file you referenced. Exiting." << std::endl;
                return 1;
            }
        }
        std::cerr << "Error: Unexpected input was identified, exiting." << std::endl;
        std::cerr << "Error: Could not open/find the file you referenced. Exiting." << std::endl;
        return 1;
    }

}


int main(int argc, char* argv[])
{
    int verbose = 0;
    char errbuff[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr *header;
    const u_char *data;
    std::string  filename;

    // Check if our arg values are wrong
    if(checkArgs(argc,argv,&verbose,&filename) == 1){
        return 1;    
    }
    // Read in our pcap file
    pcap_t * pcap = pcap_open_offline(filename.c_str(),errbuff);
    if( pcap == NULL ){
        std::cerr << "Error: unable to open Pcap File supplied: " << errbuff << std::endl;
        return 1;
    }
    
    // Loop through each packet
    while(pcap_next_ex(pcap, &header, &data) >= 0)
    {  
        Packet * mypacket = new Packet(header,data,verbose);
        // Try parsing each packet
        if(!mypacket->parsePacket()){
            if(verbose){
                std::cerr << "Error: There was a problem parsing a packet" << std::endl;
            }
        }else{
            if(!mypacket->setTCPIPdata()){  
                if(verbose){
                    std::cerr << "Error: There was problem extracting TCPIP data" << std::endl;
                }
            }
        }
    }
}

/*
class ModSecurityAnalyze
{
public:
       ModSecurityAnalyze();
       ~ModSecurityAnalyze();
 
       // modsecurity execute engine
       bool AnalyzeRequestHeaders(const CRequest& req);
       bool AnalyzeRequestBody(const CRequest& req);
       bool AnalyzeReplyHeaders(const CReply& req);
       bool AnalyzeReplyBody(const CReply& req);
       bool Reset();
 
 
       //Load Rules
       static bool LoadRules();
 
       //init modsecurity engine
       bool Init(const CRequest& req);
 
private:
       static ModSecurity::ModSecurity _modsec;
      
       // should be changed and moved to policy
       static ModSecurity::Rules _rules;
 
       // modsecurity engine object
       ModSecurity::Assay *_pmodsecAssay;
 
};
 
 
/////////////////// source file ////////////////////
//#include “modsecurityanalyze.h”
//#include “request.h” // our internal class
//#include “reply.h” // our internal class
 
/*
// modsecurity analysis class implementations
ModSecurity::ModSecurity ModSecurityAnalyze::_modsec;
ModSecurity::Rules ModSecurityAnalyze::_rules;
 
ModSecurityAnalyze::ModSecurityAnalyze() : _pmodsecAssay(nullptr)
{
}
 
ModSecurityAnalyze::~ModSecurityAnalyze()
{
       Reset();
}
 
 
bool ModSecurityAnalyze::AnalyzeRequestHeaders(const CRequest& req)
{
       Init(req);
 
       ModSecurity::ModSecurityIntervention status_it;
 
 
       //Add each header by loop;
       CHeadersIterator header_iter = req.GetHeaders();
       CHeader *header;
       while (header = header_iter.GetNextHeader())
       {
      
              if(_pmodsecAssay->addRequestHeader(header->GetHeaderName(),header->GetHeaderValue()) < 0 )
                     LOGGER_LOG(INTERNAL_LOGGER, GL_ERROR, "modsecuritylib addRequestHeader failed: header_name:%s header_value:%s", header->GetHeaderName(),header->GetHeaderValue());
       }
 
 
       //analysis attack by modesecurity engine
       _pmodsecAssay->processRequestHeaders();
       _pmodsecAssay->intervention(&status_it);
       if( status_it.disruptive == 1) //modsecurity match some rule
       {
              if(status_it.log != NULL)
                     LOGGER_LOG(INTERNAL_LOGGER, GL_INFO, "processRequestHeaders intervention: %s", status_it.log);
              else
                     LOGGER_LOG(INTERNAL_LOGGER, GL_ERROR, "no received data from modsecuritylib in processRequestHeaders: status_it.log is NULL");
 
       }
 
       return true;
}
 
bool ModSecurityAnalyze::AnalyzeRequestBody(const CRequest& req)
{
       Init(req);
 
       ModSecurity::ModSecurityIntervention status_it;
 
       _pmodsecAssay->appendRequestBody(req.GetRequestData(),req.GetRequestData().length());
      
       //analysis attack by modesecurity engine
       _pmodsecAssay->processRequestBody();
       _pmodsecAssay->intervention(&status_it);
       if( status_it.disruptive == 1)
       {
              if(status_it.log != NULL)
                     LOGGER_LOG(INTERNAL_LOGGER, GL_INFO, "processRequestBody intervention: %s", status_it.log);
              else
                     LOGGER_LOG(INTERNAL_LOGGER, GL_ERROR, "no received data from modsecuritylib in processRequestBody: status_it.log is NULL");
 
       }
 
 
       return true;
}
 
bool ModSecurityAnalyze::AnalyzeReplyHeaders(const CReply& rep)
{
      
       ModSecurity::ModSecurityIntervention status_it;
 
       //Add each header by loop
       CHeadersIterator header_iter = rep.GetHeaders();
       CHeader *header;
       while (header = header_iter.GetNextHeader())
       {
      
              if(_pmodsecAssay->addResponseHeader(header->GetHeaderName(),header->GetHeaderValue()) < 0 )
                     LOGGER_LOG(INTERNAL_LOGGER, GL_ERROR, "modsecuritylib addResponseHeader failed: header_name:%s header_value:%s", header->GetHeaderName(), header->GetHeaderValue());
       }
 
 
       //analysis attack by modesecurity engine
       _pmodsecAssay->processResponseHeaders();
       _pmodsecAssay->intervention(&status_it);
       if( status_it.disruptive == 1)
       {
              if(status_it.log != NULL)
                     LOGGER_LOG(INTERNAL_LOGGER, GL_INFO, "processResponseHeaders intervention: %s", status_it.log);
              else
                     LOGGER_LOG(INTERNAL_LOGGER, GL_ERROR, "no received data from modsecuritylib in processResponseHeaders: status_it.log is NULL");
 
       }
 
       return true;
}
 
bool ModSecurityAnalyze::AnalyzeReplyBody(const CReply& rep)
{
       ModSecurity::ModSecurityIntervention status_it;
 
       _pmodsecAssay->appendResponseBody(rep.GetReplyData(),rep.GetReplyData().GetLength());
      
       //analysis attack by modesecurity engine
       _pmodsecAssay->processResponseBody();
       _pmodsecAssay->intervention(&status_it);
       if( status_it.disruptive == 1)
       {
              if(status_it.log != NULL)
                     LOGGER_LOG(INTERNAL_LOGGER, GL_ERROR, "processResponseBody intervention: %s", status_it.log);
              else
                     LOGGER_LOG(INTERNAL_LOGGER, GL_ERROR, "no data received from modsecuritylib in processResponseBody: status_it.log is NULL");
 
       }
 
       return true;
}
 
bool ModSecurityAnalyze::Reset()
{
       if(_pmodsecAssay != nullptr)
              delete _pmodsecAssay;
 
       _pmodsecAssay = nullptr;
 
       return true;
}
 
//init modsecurity engine
bool ModSecurityAnalyze::Init(const CRequest& req)
{
       if(_pmodsecAssay != nullptr)
              return true;
      
       _pmodsecAssay = new ModSecurity::Assay(&_modsec, &_rules, NULL);
       CTCPAddr clientIPport = req.GetSource();
       CTCPAddr serverIPport = req.GetDest();
       if(_pmodsecAssay->processConnection( clientIPport.GetHost(), clientIPport.GetPort(), serverIPport.GetHost(), serverIPport.GetPort()) < 0 )
       {
              LOGGER_LOG(INTERNAL_LOGGER, GL_ERROR, "modsecuritylib: processConnection failed: clientIP:%s client_port:%d, serverIP:%s server_port:%d", clientIPport.GetHost(), clientIPport.GetPort(), serverIPport.GetHost(), serverIPport.GetPort());
              return false;
       }
      
 
       if(_pmodsecAssay->processURI(req.GetUri(),req.GetMethodString(),req.GetProtocolVersionStr()) < 0 )
       {
              LOGGER_LOG(INTERNAL_LOGGER, GL_ERROR, "modsecuritylib: processURI failed: URI:%s Method:%s, Version:%s", gs_uri.GetBuffer().Get(), gs_method.GetBuffer().Get(), gs_protocolVersion.GetBuffer().Get());
              return false;
       }
 
       ModSecurity::ModSecurityIntervention status_it;
       _pmodsecAssay->intervention(&status_it);
       if( status_it.disruptive == 1)
       {
 
              if(status_it.log != NULL)
                     LOGGER_LOG(INTERNAL_LOGGER, GL_INFO, "processURI intervention: %s", status_it.log);
              else
                     LOGGER_LOG(INTERNAL_LOGGER, GL_ERROR, "no data received from modsecuritylib in processURI: status_it.log is NULL");
 
       }
      
       return true;
}
 
bool ModSecurityAnalyze::LoadRules()
{
       // it should be per site
       const char* rules_file = "/tmp/basic_rules.conf";
       if(_rules.loadFromUri(rules_file) < 0)
       {
              LOGGER_LOG(INTERNAL_LOGGER, GL_ERROR, "modsecuritylib: load rules from /tmp/basic_rules.conf failed");
              return false;
       }
 
       LOGGER_LOG(INTERNAL_LOGGER, GL_INFO, "modsecuritylib: successfully load rules from /tmp/basic_rules.conf");
 
       return true;
}
*/
