#include <string>
#include <map>
#include <unordered_map>
using std::string;
using std::map;
using std::unordered_map;
#include <iostream>

string encoder( string input ){

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


// Without '?' character at the end of base_url
string encode_hashmap( string base_url, map<string, string> map ) {
    string result;
    result += '?';
    for (auto keypair : map){
        result +='&';
        
        // .erase removes the incrementing index at the start
        result += encoder(keypair.first) + '=' + encoder(keypair.second);
    }
    //  this erase removes & at the start
    return base_url+result.erase(1,1);
};

string encode_hashmap_withoutURL(map<string, string> map ) {

    string result = "?";

    for ( auto keypair : map ) {

        result +='&';
        string str1 = keypair.first;
        // .erase removes the incrementing index at the start
        result += encoder(str1.erase(0,1)) + '=' + encoder(keypair.second);

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

// For some reason, you have to put a indexing number in order for the map to be sorted.
// Without '?' character at the end of base_url
string encode_hashmap_ordered( string base_url ,map<string, string> map ) {

    string result = "?";

    for ( auto keypair : map ) {

        result +='&';
        string str1 = keypair.first;
        // .erase removes the incrementing index at the start
        result += encoder(str1.erase(0,1)) + '=' + encoder(keypair.second);

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