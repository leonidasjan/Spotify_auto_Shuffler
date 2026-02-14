#ifndef ENCODER_HPP
#define ENCODER_HPP
#include <string>
#include <map>
#include <unordered_map>
using std::string,std::map,std::unordered_map;

string encoder( string input=" " );
// string encode_hashmap( string base_url=" " ,umap<string, string> map );
string encode_hashmap_ordered( string base_url ,map<string, string> map );
string encode_hashmap_withoutURL(map<string, string> map );
#endif