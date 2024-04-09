/**
 * 此头文件实现了将新应用程序映射到最冷核心的策略
 * 并将线程从热核心迁移到最冷核心。
 */

#ifndef __COLDESTCORE_H
#define __COLDESTCORE_H

#include <vector>
#include "mappingpolicy.h"
#include "migrationpolicy.h"
#include "performance_counters.h"

class ColdestCore : public MappingPolicy, public MigrationPolicy
{
public:
    ColdestCore(
        const PerformanceCounters *performanceCounters,
        int coreRows,
        int coreColumns,
        float criticalTemperature);
    virtual std::vector<int> map(
        String taskName,
        int taskCoreRequirement,
        const std::vector<bool> &availableCores,
        const std::vector<bool> &activeCores);
    virtual std::vector<migration> migrate(
        SubsecondTime time,
        const std::vector<int> &taskIds,
        const std::vector<bool> &activeCores);

private:
    const PerformanceCounters *performanceCounters;

    unsigned int coreRows;
    unsigned int coreColumns;
    float criticalTemperature;

    int getColdestCore(const std::vector<bool> &availableCores);
    void logTemperatures(const std::vector<bool> &availableCores);
};

#endif