#include "staticMigration.h"
#include <iomanip>

using namespace std;

StaticMigration::StaticMigration(
        const PerformanceCounters *performanceCounters,
        int coreRows,
        int coreColumns,
        int threads)
        : performanceCounters(performanceCounters),
          coreRows(coreRows),
          coreColumns(coreColumns),
          threads(threads) {
        this->next = 0;
}

std::vector<migration> StaticMigration::migrate(
        SubsecondTime time,
        const std::vector<int> &taskIds,
        const std::vector<bool> &activeCores)
{

    std::vector<migration> migrations;

    std::vector<bool> availableCores(coreRows * coreColumns);
    migration m;

    m.fromCore = this->next;
    m.toCore = (this->next+1) % this->threads;
    this->next = m.toCore;
    m.swap = true;
    migrations.push_back(m);

    return migrations;
}
