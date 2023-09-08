#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include "linux_parser.h"
#include "iostream"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

std::string convert_multiple_space_to_string(const std::string str){
  string nstr;
    
    //loop through the characters of the input string
    for(unsigned int i=0; i<str.length();  ){
        //check if character is white space
        if(str[i] == ' '){
            /* 
              *do not include the white space, if-
              *it is at the trailing or leading position
            */
            if(i==0 || i==str.length()-1){
                i++;
                continue;
            }
            
            /*
              *if space is inbetween then skip it-
              *except the last occurrence
            */
            while(str[i+1] == ' ')
                i++;
        }
        
        //concatenate the character to the new string
        nstr += str[i++];
    }
    // std::cout<<"fjklasjlkfalskflksjfkljskldfjklsdjflkjsdklfkldjflksjdklfjsklfjklsjflksdjflkjslkfjskldfjlskdjfklsjfklsjdklfjskldjfkl      "<<nstr<<std::endl;
    return nstr;
}
// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    std::ifstream meminfo(kProcDirectory + kMeminfoFilename);
    std::string line;
    double totalMemory = 0.0;
    double freeMemory = 0.0;

    while (std::getline(meminfo, line)) {
      // std::cout<<line<<std::endl;
        if (line.find("MemTotal:") != std::string::npos) {
          // std::cout<<line<<std::endl;
            std::sscanf(line.c_str(), "MemTotal: %lf kB", &totalMemory);
            // std::cout<<"Mem Total:-  "<<totalMemory;
        }
        else if (line.find("MemAvailable:") != std::string::npos) {
            std::sscanf((convert_multiple_space_to_string(line.c_str())).c_str(), "MemAvailable: %lf kB", &freeMemory);
            // std::cout<<"mem available "<<line<<" fasdfasdfas  "<<freeMemory;
            
        }
    }

    meminfo.close();

    if (totalMemory == 0.0 || freeMemory == 0.0) {
        // std::cout << "kdsjflkajflkjasdklfjlaksdjflkasjdlkfjaslkfjklasdjflkjsflkjaskldfjklasdjfkljasdklfjklasdjfas" << std::endl;
        return -1.0;
    }
    // std::cout<< "total memory: " << totalMemory << std::endl;
    // std::cout<< "free memory: " << freeMemory << std::endl;
    // std::cout<< "free memory: " << freeMemory << std::endl;
    float memoryUtilization = (totalMemory - freeMemory) / totalMemory ;
    return memoryUtilization;
}

// TODO: Read and return the system uptime
long int LinuxParser::UpTime() {
  // std::cout<<"Read and return the system uptime"<<std::endl;
  std::ifstream uptimeFile(kProcDirectory + kUptimeFilename);
    if (!uptimeFile.is_open()) {
        // std::cout<<"file error closed"<<std::endl;
        return -1.0;
    }
    // std::cout<<"Read and return the system uptime 2"<<std::endl;
    long int uptimeInSeconds = 0.0;
    uptimeFile >> uptimeInSeconds;

    uptimeFile.close();
    // std::cout<<"Read and return the system uptime:: answer "<<uptimeInSeconds<<std::endl;
    return uptimeInSeconds;
 }

std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream stream(input);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
      // std::cout <<"curr token "<< token << std::endl;
        tokens.push_back(token);
    }

    return tokens;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
      std::ifstream statFile(kProcDirectory + kStatFilename);
    if(!statFile.is_open()) {
        // std::cout<<"Couldn't open " << kProcDirectory + kStatFilename;
        return -1;
    }
    std::string line;
    std::getline(statFile, line);
    statFile.close();
    // std::cout<< "Follower: " << line << std::endl;
    std::vector<std::string> tokens = splitString(line, ' ');
    long jiffies = 0;
    for (size_t i = 1; i < tokens.size(); ++i) {
        try {
            // std::cout << tokens[i] << std::endl;
            jiffies += std::stol(tokens[i]);
        } catch (const std::exception& e) {
            // std::cout <<"Error While computing jiffies"<< e.what() << std::endl;
            // return -1;
            continue;
        }
    }
    return jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
      std::ifstream statFile(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (!statFile.is_open()) {
        return -1;
    }

    std::string line;
    std::getline(statFile, line);
    statFile.close();

    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    if (tokens.size() < 17) {
        return -1;
    }

    try {
        long  utime = std::stol(tokens[13]);
        long  stime = std::stol(tokens[14]);
        long  cutime = std::stol(tokens[15]);
        long  cstime = std::stol(tokens[16]);

        long  activeJiffies = utime + stime + cutime + cstime;
        return activeJiffies;
    } catch (const std::exception& e) {
        return -1;
    }
 }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  std::ifstream statFile(kProcDirectory + kStatFilename);
    if (!statFile.is_open()) {
        return -1;
    }
    std::string line;
    std::getline(statFile, line);
    statFile.close();
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    if (tokens.size() < 4) {
        return -1;
    }

    try {
        long user = std::stol(tokens[1]);
        long nice = std::stol(tokens[2]);
        long system = std::stol(tokens[3]);

        long activeJiffies = user + nice + system;
        return activeJiffies;
    } catch (const std::exception& e) {
        return -1;
    }
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::ifstream statFile(kProcDirectory + kStatFilename);
    if (!statFile.is_open()) {
        return -1;
    }

    std::string line;
    std::getline(statFile, line);
    statFile.close();

    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }


    if (tokens.size() < 5) {
        return -1;
    }

    try {
        long idle = std::stol(tokens[4]);

        return idle;
    } catch (const std::exception& e) {
        return -1;
    }; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
      std::ifstream statFile(kProcDirectory + kStatFilename);
    if (!statFile.is_open()) {
        return {};
    }

    std::string line;
    std::getline(statFile, line);
    statFile.close();

   
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    
    std::vector<std::string> cpuUtilization;
    for (size_t i = 1; i < tokens.size(); ++i) {
        cpuUtilization.push_back(tokens[i]);
    }

    return cpuUtilization;
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
      DIR* procDir = opendir("/proc");
    if (!procDir) {
        return -1;
    }

    int totalProcesses = 0;
    struct dirent* entry;
    while ((entry = readdir(procDir)) != nullptr) {
       
        if (entry->d_type == DT_DIR && std::isdigit(entry->d_name[0])) {
            totalProcesses++;
        }
    }

    closedir(procDir);
    return totalProcesses;
 }

bool isProcessRunning(const std::string& pid) {
    std::ifstream statusFile("/proc/" + pid + "/status");
    if (!statusFile.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(statusFile, line)) {
        if (line.compare(0, 6, "State:") == 0) {
            std::istringstream iss(line);
            std::string field, state;
            iss >> field >> state;
            if (state == "R") {
                return true;
            }
        }
    }

    return false;
}
// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
      DIR* procDir = opendir("/proc");
    if (!procDir) {
        return -1;
    }

    int runningProcesses = 0;
    struct dirent* entry;
    while ((entry = readdir(procDir)) != nullptr) {
        
        if (entry->d_type == DT_DIR && std::isdigit(entry->d_name[0])) {
            std::string pid = entry->d_name;
            if (isProcessRunning(pid)) {
                runningProcesses++;
            }
        }
    }

    closedir(procDir);
    return runningProcesses;
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
    std::ifstream cmdlineFile(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
    if (!cmdlineFile.is_open()) {
        return "";
    }

    std::string command;
    std::getline(cmdlineFile, command);

    cmdlineFile.close();

   
    for (char& c : command) {
        if (c == '\0') {
            c = ' ';
        }
    }

    return command;
 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {     
  std::ifstream statmFile("/proc/" + std::to_string(pid) + "/statm");
    if (!statmFile.is_open()) {
        return "-1";
    }

    // long long  memoryUsage = -1;
    // std::string line;
    // std::getline(statmFile, line);
    // statmFile.close();

    // std::istringstream iss(line);
    // if (iss >> memoryUsage) {
    //     long long pageSizeInMB = sysconf(_SC_PAGESIZE) / (1024 * 1024); 
    //     // double memory_usage_in_mb = static_cast<double>(memory_usage) / (1024 * 1024);
    //     memoryUsage *= pageSizeInMB;
    // } else {
    //     return "-1";
    // }

    // return std::to_string(memoryUsage); 
    
    // Read the memory usage from the file
    long long memory_usage = -1;
    std::string line;
    if (std::getline(statmFile, line)) {
        std::istringstream iss(line);
        iss >> memory_usage;
    }

    // Close the file
    statmFile.close();

    // Memory usage is in pages, so convert to bytes
    long long page_size = sysconf(_SC_PAGESIZE);
    if (page_size > 0) {
        memory_usage *= page_size;
    }

    // Convert bytes to megabytes
    double memory_usage_in_mb = static_cast<double>(memory_usage) / (1024 * 1024);

    return std::to_string(static_cast<int>(memory_usage_in_mb));

    }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
      std::ifstream statusFile("/proc/" + std::to_string(pid) + "/status");
    if (!statusFile.is_open()) {
        return "-1";
    }

    int userID = -1;
    std::string line;
    while (std::getline(statusFile, line)) {
        if (line.compare(0, 5, "Uid:\t") == 0) {
            std::istringstream iss(line);
            std::string field;
            iss >> field >> userID;
            break;
        }
    }

    statusFile.close();
    return std::to_string(userID);
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
      // Step 1: Read the UID from the /proc/<PID>/status file
    std::ifstream statusFile(kProcDirectory + std::to_string(pid) +kStatusFilename);
    if (!statusFile.is_open()) {
        return "";
    }

    int userID = -1;
    std::string line;
    while (std::getline(statusFile, line)) {
        if (line.compare(0, 5, "Uid:\t") == 0) {
            std::istringstream iss(line);
            std::string field;
            iss >> field >> userID;
            break;
        }
    }

    statusFile.close();

    std::ifstream passwdFile(kPasswordPath);
    if (!passwdFile.is_open()) {
        return "";
    }

    std::string username;
    while (std::getline(passwdFile, line)) {
        std::istringstream iss(line);
        std::string entry;
        std::map<int, std::string> userInfo;

        while (std::getline(iss, entry, ':')) {
            userInfo[userInfo.size()] = entry;
        }

        if (userInfo.size() >= 3) {
            int uid = std::stoi(userInfo[2]);
            if (uid == userID) {
                username = userInfo[0];
                break;
            }
        }
    }

    passwdFile.close();
    return username;
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  // std::cout<<"Inside UPTIME Calculation in linux"<<std::endl;

    //   std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();

    std::ifstream statFile(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (!statFile.is_open()) {
        return -1;
    }

    // std::string line;
    // std::getline(statFile, line);
    // statFile.close();

    // std::istringstream iss(line);
    // std::string field;
    // for (int i = 1; i <= 21; ++i) { 
    //     iss >> field;
    // }

    // unsigned long long int starttime;
    // if (!(iss >> starttime)) {
    //     return -1;
    // }

    // long clockTicksPerSecond = sysconf(_SC_CLK_TCK);

    // double uptime = static_cast<double>(currentTime.time_since_epoch().count()) / static_cast<double>(clockTicksPerSecond) - static_cast<double>(starttime) / static_cast<double>(clockTicksPerSecond);

    // return uptime;
    // Read the contents of the /proc/<pid>/stat file
    std::string line;
    if (std::getline(statFile, line)) {
        // Tokenize the content to extract the required fields
        std::istringstream iss(line);
        std::string token;
        long uptime = 0;

        // Iterate through the tokens to extract the fields
        for (int i = 0; i < 22; ++i) {
            iss >> token;
            if (i == 21) {
                // Field 22 represents process start time in clock ticks
                uptime = std::stoll(token);
                break;
            }
        }

        // Close the file
        statFile.close();

        // Calculate uptime in seconds
        long clock_ticks_per_second = sysconf(_SC_CLK_TCK);
        if (clock_ticks_per_second > 0) {
            return uptime / clock_ticks_per_second;
        }
    }

    // Close the file
    statFile.close();

    return -1; 
 }
