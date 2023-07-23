#include "ncurses_display.h"
#include "system.h"

#include "iostream"

int main() {
  System system;
  // std::cout<<"Welcome to the Curses Display";
  // std::cout<<system.Kernel();
  // NCursesDisplay::Display(system);
  // for (int i=0; i < system.Processes().size(); i++){
  //   auto process = system.Processes()[i];
  //   std::cout<<process.Command()<<std::endl;
  //   std::cout<<process.CpuUtilization()<<std::endl;
  //   std::cout<<process.Pid()<<std::endl;
  //   std::cout<<process.Ram()<<std::endl;
  //   std::cout<<process.UpTime()<<std::endl;
  //   std::cout<<process.User()<<std::endl;
  //   std::cout<<"________________________________________________________________"<<std::endl;
  // }
  // std::cout<<"CPU Utilization "<<system.Cpu().Utilization()<<std::endl;
  // std::cout<<"Kernel "<<system.Kernel()<<std::endl;
  std::cout<<"MemoryUtilization "<<system.MemoryUtilization()<<std::endl;
  // std::cout<<"OperatingSystem "<<system.OperatingSystem()<<std::endl;
  // std::cout<<"Running Processes "<<system.RunningProcesses()<<std::endl;
  // std::cout<<"TotalProcesses "<<system.TotalProcesses()<<std::endl;
  // std::cout<<"UpTime "<<system.UpTime()<<std::endl;
}