#include "dvfsFixedFreq.h"
#include <iomanip>
#include <iostream>

using namespace std;

DVFSFixedFreq::DVFSFixedFreq(const PerformanceCounters *performanceCounters,
                                     int coreRows,
                                     int coreColumns,
                                     int freqCore0,
                                     int freqCore1,
                                     int freqCore2,
                                     int freqCore3)
        : performanceCounters(performanceCounters),
          coreRows(coreRows),
          coreColumns(coreColumns),
          freqCore0(freqCore0),
          freqCore1(freqCore1),
          freqCore2(freqCore2),
          freqCore3(freqCore3) {

    this->coresFreq.push_back(freqCore0);
    this->coresFreq.push_back(freqCore1);
    this->coresFreq.push_back(freqCore2);
    this->coresFreq.push_back(freqCore3);
}

std::vector<int>
DVFSFixedFreq::getFrequencies(const std::vector<int> &oldFrequencies, const std::vector<bool> &activeCores) {
    std::vector<int> frequencies(coreRows *coreColumns);

    for (unsigned int coreCounter = 0; coreCounter < coreRows * coreColumns; coreCounter++) {
        if (activeCores.at(coreCounter)) {
            float power = performanceCounters->getPowerOfCore(coreCounter);
            float temperature = performanceCounters->getTemperatureOfCore(coreCounter);
            int frequency = oldFrequencies.at(coreCounter);
            float utilization = performanceCounters->getUtilizationOfCore(coreCounter);

            cout << "[Scheduler][DVFS_ISOLATION_FREQ]: Core " << setw(2) << coreCounter << ":";
            cout << " P=" << fixed << setprecision(3) << power << " W";
            cout << " f=" << frequency << " MHz";
            cout << " T=" << fixed << setprecision(1) << temperature << " °C";
            cout << " utilization=" << fixed << setprecision(3) << utilization << endl;
        }
        frequencies.at(coreCounter) = this->coresFreq[coreCounter] * 1000;
    }

    return frequencies;
}