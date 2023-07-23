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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    std::ifstream meminfo(kProcDirectory + kMeminfoFilename);
    std::string line;
    float totalMemory = 0.0;
    float freeMemory = 0.0;

    while (std::getline(meminfo, line)) {
        if (line.find("MemTotal:") != std::string::npos) {
            std::sscanf(line.c_str(), "MemTotal: %lf kB", &totalMemory);
        }
        else if (line.find("MemAvailable:") != std::string::npos) {
            std::sscanf(line.c_str(), "MemAvailable: %lf kB", &freeMemory);
        }
    }

    meminfo.close();

    if (totalMemory == 0.0 || freeMemory == 0.0) {
        
        return -1.0;
    }

    float memoryUtilization = (totalMemory - freeMemory) / totalMemory * 100.0;
    return memoryUtilization;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream uptimeFile(kProcDirectory + kUptimeFilename);
    if (!uptimeFile.is_open()) {
        
        return -1.0;
    }

    long uptimeInSeconds = 0.0;
    uptimeFile >> uptimeInSeconds;

    uptimeFile.close();
    return uptimeInSeconds;
 }

std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream stream(input);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
      std::cout <<"curr token "<< token << std::endl;
        tokens.push_back(token);
    }

    return tokens;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
      std::ifstream statFile(kProcDirectory + kStatFilename);
    if(!statFile.is_open()) {
        std::cout<<"Couldn't open " << kProcDirectory + kStatFilename;
        return -1;
    }
    std::string line;
    std::getline(statFile, line);
    statFile.close();
    std::cout<< "Follower: " << line << std::endl;
    std::vector<std::string> tokens = splitString(line, ' ');
    long jiffies = 0;
    for (size_t i = 1; i < tokens.size(); ++i) {
        try {
            std::cout << tokens[i] << std::endl;
            jiffies += std::stol(tokens[i]);
        } catch (const std::exception& e) {
            std::cout <<"Error While computing jiffies"<< e.what() << std::endl;
            return -1;
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

    long  memoryUsage = -1;
    std::string line;
    std::getline(statmFile, line);
    statmFile.close();

    std::istringstream iss(line);
    if (iss >> memoryUsage) {
        long pageSizeInKB = sysconf(_SC_PAGESIZE) / 1024; 
        memoryUsage *= pageSizeInKB;
    } else {
        return "-1";
    }

    return std::to_string(memoryUsage); }

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

      std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();

    std::ifstream statFile(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (!statFile.is_open()) {
        return -1;
    }

    std::string line;
    std::getline(statFile, line);
    statFile.close();

    std::istringstream iss(line);
    std::string field;
    for (int i = 1; i <= 21; ++i) { 
        iss >> field;
    }

    unsigned long long int starttime;
    if (!(iss >> starttime)) {
        return -1;
    }

    long clockTicksPerSecond = sysconf(_SC_CLK_TCK);

    double uptime = static_cast<double>(currentTime.time_since_epoch().count()) / static_cast<double>(clockTicksPerSecond) - static_cast<double>(starttime) / static_cast<double>(clockTicksPerSecond);

    return uptime;
 }
