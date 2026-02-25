#pragma once
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include "nlohmann/json.hpp"
using std::string;
using std::map;
using std::unordered_map;


namespace encode {


inline string str(const string input ){

    string output;

    unordered_map<char, string> reserved;

        reserved[' '] = "%20"; reserved['!'] = "%21"; reserved['"'] = "%22"; reserved['#'] = "%23";
        reserved['$'] = "%24"; reserved['%'] = "%25"; reserved['&'] = "%26"; reserved['\''] = "%27";
        reserved['('] = "%28"; reserved[')'] = "%29"; reserved['*'] = "%2A"; reserved['+'] = "%2B";
        reserved[','] = "%2C"; reserved['/'] = "%2F"; reserved[':'] = "%3A"; reserved[';'] = "%3B";
        reserved['<'] = "%3C"; reserved['='] = "%3D"; reserved['>'] = "%3E"; reserved['?'] = "%3F";
        reserved['@'] = "%40"; reserved['['] = "%5B"; reserved['\\'] = "%5C"; reserved[']'] = "%5D";
        reserved['^'] = "%5E"; reserved['`'] = "%60"; reserved['{'] = "%7B"; reserved['|'] = "%7C";
        reserved['}'] = "%7D";
        
    size_t t;
    for( t = 0 ; t <= input.length() ; t++) {
        int found = 0;
        for ( auto ch : reserved ) {
            if ( input[t] == ch.first ) {
                output += ch.second;
                found = 1;
                break;
            };
        };

        if ( !found ) { output += input[t]; };

    };

    return output;
};

inline string map(const string base_url, std::map<string, string> m ) {
    string result;
    
    result += '?';

    for (auto keypair : m){
        result +='&';
        
        // .erase removes the incrementing index at the start
        result += encode::str(keypair.first) + '=' + encode::str(keypair.second);
    }
    //  this erase removes & at the start
    return base_url+result.erase(1,1);
};

inline string map(std::map<string, string> m ) {

    string result; // NO ? sign check shuffler if it matches

    for ( auto keypair : m ) {

        result +='&';
        string str1 = keypair.first;
        // .erase removes the incrementing index at the start
        result += encode::str(str1.erase(0,1)) + '=' + encode::str(keypair.second);

    };

    //Cleanup
    //  this erase removes & at the start
    result.erase(1,1);
    // Remove any null terminate char
    size_t temp = 0;
        for (unsigned char c : result){

            if ((int)c == 0) {
                if ( result.size() > temp ) { 
                    result.erase(temp,1);
                };
            };
            temp++;
        };


    return result;
};

// put a indexing number in first key character for the map to be sorted.
inline string map_ordered(const string base_url, std::map<string, string> m ) {

    string result = "?";

    for ( auto keypair : m ) {

        result +='&';
        string str1 = keypair.first;
        // .erase removes the incrementing index at the start
        result += encode::str(str1.erase(0,1)) + '=' + encode::str(keypair.second);

    };

    //Cleanup
    //  this erase removes & at the start
    result.erase(1,1);
    // Remove any null terminate char
    size_t temp = 0;
        for (unsigned char c : result){

            if ((int)c == 0) {
                if ( result.size() > temp ) { 
                    result.erase(temp,1);
                };
            };
            temp++;
        };


    return base_url+result;
};
// put a indexing number in first key character for the map to be sorted.
inline string map_ordered(std::map<string, string> m ) {

    string result;

    for ( auto keypair : m ) {

        result +='&';
        string str1 = keypair.first;
        // .erase removes the incrementing index at the start
        result += encode::str(str1.erase(0,1)) + '=' + encode::str(keypair.second);

    };

    //Cleanup
    //  this erase removes & at the start
    result.erase(0,1);
    // Remove any null terminate char
    size_t temp = 0;
        for (unsigned char c : result){

            if ((int)c == 0) {
                if ( result.size() > temp ) { 
                    result.erase(temp,1);
                };
            };
            temp++;
        };


    return result;
};
// TODO: find out if i can use ref here
inline std::string json(const std::string base_url,const nlohmann::json &j ){

    std::string result = base_url+'?'+encode::str(j.dump());

    return result;
}


};
