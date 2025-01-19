#ifndef PERFORMANCE_METRICS_H
#define PERFORMANCE_METRICS_H

#include <string>
#include <windows.h>
#include <pdh.h>
#include <iostream>

class PerformanceMetrics {
public:
    static void getMetrics(const std::string& mode) {
        PDH_HQUERY cpuQuery;
        PDH_HCOUNTER cpuTotal;

        PdhOpenQuery(NULL, 0, &cpuQuery);
        PdhAddCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", 0, &cpuTotal);
        PdhCollectQueryData(cpuQuery);

        Sleep(2000);

        PDH_FMT_COUNTERVALUE counterVal;
        PDH_STATUS status = PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);

        if (status == ERROR_SUCCESS) {
            std::cout << "CPU usage (" << mode << "): " << counterVal.doubleValue << "%" << std::endl;
        } else {
            std::cout << "Error retrieving CPU usage. Code: " << status << std::endl;

            double cpuUsage = calculateCPUUsage();
            if (cpuUsage >= 0) {
                std::cout << "Alternative CPU usage: " << cpuUsage << "%" << std::endl;
            } else {
                std::cout << "Failed to calculate CPU usage." << std::endl;
            }
        }

        PdhCloseQuery(cpuQuery);

        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);

        std::cout << "Total memory usage: " << statex.dwMemoryLoad << "%" << std::endl;
        std::cout << "Available physical memory: " << statex.ullAvailPhys / (1024 * 1024) << " MB" << std::endl;
        std::cout << "Total physical memory: " << statex.ullTotalPhys / (1024 * 1024) << " MB" << std::endl;
    }

private:
    static double calculateCPUUsage() {
        FILETIME idleTime, kernelTime, userTime;
        if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
            static ULARGE_INTEGER prevIdleTime, prevKernelTime, prevUserTime;

            ULARGE_INTEGER idle, kernel, user;
            idle.LowPart = idleTime.dwLowDateTime;
            idle.HighPart = idleTime.dwHighDateTime;

            kernel.LowPart = kernelTime.dwLowDateTime;
            kernel.HighPart = kernelTime.dwHighDateTime;

            user.LowPart = userTime.dwLowDateTime;
            user.HighPart = userTime.dwHighDateTime;

            ULONGLONG idleDiff = (idle.QuadPart - prevIdleTime.QuadPart);
            ULONGLONG kernelDiff = (kernel.QuadPart - prevKernelTime.QuadPart);
            ULONGLONG userDiff = (user.QuadPart - prevUserTime.QuadPart);

            ULONGLONG totalDiff = kernelDiff + userDiff;

            prevIdleTime = idle;
            prevKernelTime = kernel;
            prevUserTime = user;

            return (1.0 - (double)idleDiff / totalDiff) * 100.0;
        }
        return -1.0;
    }
};

#endif // PERFORMANCE_METRICS_H
