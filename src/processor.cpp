#include "processor.h"
#include "linux_parser.h"
#include "iostream"



// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
      long totalJiffies = LinuxParser::Jiffies();
      std::cout<<"totalJiffies:- "<<totalJiffies<<std::endl;
  long activeJiffies = LinuxParser::ActiveJiffies();
  std::cout<<"activeJiffies:- "<<activeJiffies<<std::endl;
  float utilization = static_cast<float>(activeJiffies) / totalJiffies;
  return utilization;
 }