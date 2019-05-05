#ifndef __TUTILS__
#define __TUTILS__

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <array>
#include <sstream>

bool test_listen(int port) {

    std::stringstream ss;
    ss << "netstat -anp 2>/dev/null|grep LISTEN|grep " << port ;

    FILE* fh = popen(ss.str().c_str(),"r");
    std::array<char, 255> buff;
    
    std::vector<std::string> out_lines;

    while( ! feof(fh) ) {
        fgets(buff.data(),255,fh);
        out_lines.emplace_back(buff.data());
    }
    fclose(fh);

    std::stringstream ss2;
    ss2 << "0.0.0.0:" << port;

    for(auto& l: out_lines) {

        if (l.find(ss2.str()) != std::string::npos && l.find("LISTEN") != std::string::npos) 
        return true;
    }
    
    return false;

}


bool test_http(const char* url, const char* content) {

    std::string cmd = "wget -qO- " + std::string(url);
    std::array<char, 255> buff;
    FILE* fh = popen(cmd.c_str(),"r");
    std::string http_out;
    while( ! feof(fh) ) {
        fgets(buff.data(),255,fh);
        http_out += buff.data();
    }
    fclose(fh);

    return (http_out == std::string(content));
}

#endif