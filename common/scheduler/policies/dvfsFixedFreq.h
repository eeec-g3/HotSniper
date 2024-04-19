/**
 * This header implements the max. freq DVFS policy
 */

#ifndef __DVFS_ISOLATION_H
#define __DVFS_ISOLATION_H

#include <vector>
#include "dvfspolicy.h"

class DVFSFixedFreq : public DVFSPolicy {
public:
    DVFSFixedFreq(
            const PerformanceCounters *performanceCounters,
            int coreRows,
            int coreColumns,
            int freqCore0,
            int freqCore1,
            int freqCore2,
            int freqCore3
            );
    virtual std::vector<int> getFrequencies(const std::vector<int> &oldFrequencies, const std::vector<bool> &activeCores);

private:
    const PerformanceCounters *performanceCounters;
    unsigned int coreRows;
    unsigned int coreColumns;
    int freqCore0;
    int freqCore1;
    int freqCore2;
    int freqCore3;
    std::vector<int> coresFreq;
};

#endif