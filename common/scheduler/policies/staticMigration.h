#ifndef __STATIC_MIGRATION_H
#define __STATIC_MIGRATION_H

#include <vector>
#include "string"
#include "mappingpolicy.h"
#include "migrationpolicy.h"
#include "performance_counters.h"

class StaticMigration: public MigrationPolicy
{
public:
    StaticMigration(
            const PerformanceCounters *performanceCounters,
            int coreRows,
            int coreColumns,
            int threads);

    virtual std::vector<migration> migrate(
            SubsecondTime time,
            const std::vector<int> &taskIds,
            const std::vector<bool> &activeCores);

private:
    const PerformanceCounters *performanceCounters;

    int next;
    int coreRows;
    int coreColumns;
    int threads;
};

#endif
