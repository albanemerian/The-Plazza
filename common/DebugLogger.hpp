/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-albane.merian
** File description:
** DebugLogger
*/

#include <fstream>
#include <iostream>
#include <string>
#include <mutex>

#include <iomanip>
#include <sstream>

#ifndef DEBUGLOGGER_HPP_
#define DEBUGLOGGER_HPP_

class DebugLogger {
    public:
        /* Open the loging file */
        DebugLogger(const std::string& logFile) {
            _logFile.open(logFile, std::ios::out | std::ios::app);
            if (!_logFile.is_open()) {
                std::cerr << "Error opening log file: " << logFile << std::endl;
            }
        }
        /* Close the file at the end */
        ~DebugLogger() {
            if (_logFile.is_open()) {
                _logFile.close();
            }
        }

        /* Log the string */
        void log(const std::string& message) {
            std::lock_guard<std::mutex> lock(_logMutex);
            if (_logFile.is_open()) {
                auto now = std::chrono::system_clock::now();
                auto in_time_t = std::chrono::system_clock::to_time_t(now);
                std::tm tm = *std::localtime(&in_time_t);
                _logFile << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " - " << message << std::endl;
            } else {
                std::cerr << "Log file is not open. Cannot log message: " << message << std::endl;
            }
        }
    protected:
    private:
        std::ofstream _logFile;
        std::mutex _logMutex;
};

#endif /* !DEBUGLOGGER_HPP_ */
