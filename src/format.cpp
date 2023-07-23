#include <string>
// #include <string>
#include <sstream>
#include "format.h"
#include "iostream"
#include <iomanip>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
// string Format::ElapsedTime(long seconds) { 
//     // return string();
//     std::cout<<"Inside Format "<<std::endl;
//     std::string result;
//     std::cout<<"Inside Format 2"<<std::endl;
//     std::string h = std::to_string(seconds / 3600);
//     std::string m = std::to_string((seconds % 3600) / 60);
//     std::string s = std::to_string(seconds % 60);
//     std::cout<<"Inside Format 3"<<std::endl;
//     // add leading zero if needed
//     std::string hh = std::string(2 - h.length(), '0') + h;
//     std::string mm = std::string(2 - m.length(), '0') + m;
//     std::string ss = std::string(2 - s.length(), '0') + s;
//     std::cout<<"Inside Format 4"<<std::endl;
//     // return mm:ss if hh is 00
//     if (hh.compare("00") != 0) {
//         result = hh + ':' + mm + ":" + ss;
//     }
//     else {
//         result =  mm + ":" + ss;
//     }
//     return result;
//     }
string Format::ElapsedTime(long sec) {
  int hh, mm, ss;
  std::stringstream op;
  
  
  ss = sec % 60;
  mm = (sec / 60) % 60;
  hh = (sec / 3600) % 100;

  op << std::setfill('0') << std::setw(2) << hh << ":" << std::setfill('0') << std::setw(2) << mm << ":" << std::setfill('0') << std::setw(2) << ss;
  std::string str_format = op.str(); 
  
  return str_format;
}