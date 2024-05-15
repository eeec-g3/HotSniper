//
// Created by yanghoo on 5/7/24.
//

#ifndef HOTSNIPER_DVFSEDGECOOLINGMODE_H
#define HOTSNIPER_DVFSEDGECOOLINGMODE_H

#include <vector>
#include "dvfspolicy.h"
#include "migrationpolicy.h"
#include "../performance_counters.h"
#include "mappingpolicy.h"
#include <math.h>
#include <map>

enum LEARN_STATE {
    WAIT = 0,
    RUN = 1,
    FINISH = 2,
};

struct alphaEpsilonPair {
    int level = 0;
    double alpha = 0.0;
    double epsilon = 0.0;
};

typedef std::vector<alphaEpsilonPair> DVFS_META ;

class CoreLearningMaterial {
private:
    float temperature = 0.0;
    int frequency = 0;
    int reset_times = 1;

public:
    LEARN_STATE state = LEARN_STATE::WAIT;
    float desired_temp = 0.0;
    bool is_learning = true;
    float step_portion = 0.8;
    int start_freq = 0;
    int step_size = 200;
    DVFS_META dvfs_metadata = DVFS_META();
    bool init = true;

    explicit CoreLearningMaterial(float desired) {
        this->desired_temp = desired;
    }

    void reset() {
        // try to reset 5 times, it will be reset.
        // continuous learning.
        if (this->reset_times % 10 == 0) {
            this->is_learning = true;
            this->state = LEARN_STATE::WAIT;
            this->start_freq = 0;
            this->dvfs_metadata.clear();
        }
        this->reset_times++;
    }

    void recordNewData(float temp, int freq) {
        this->temperature = temp;
        this->frequency = freq;
    }

    void calculateAlphaEpsilon(float curr_temp, int curr_freq) {
        float old_temp = this->temperature;
        int old_freq = this->frequency;
        std::cout << "curr and old freq: " << std::to_string(curr_freq) << " " << std::to_string(old_freq) << std::endl;

        double a = (curr_temp - old_temp) / (float) (curr_freq - old_freq);
        double e = (double) curr_temp - a * (double) curr_freq;

        std::cout << "a & freq: " << std::to_string(a * (double) curr_freq) << std::endl;

        auto pair = alphaEpsilonPair {};
        pair.level = this->start_freq - curr_freq;
        pair.alpha = a;
        pair.epsilon = e;
        dvfs_metadata.push_back(pair);
    }

    float get_temperature() const {
        return this->temperature;
    }

    int get_frequency() const {
        return this->frequency;
    }
};

class DVFSEdgeCoolingMode : public DVFSPolicy {
public:
    DVFSEdgeCoolingMode(
            const PerformanceCounters *performanceCounters,
            int coreRows,
            int coreColumns,
            int minFrequency,
            int maxFrequency,
            int frequencyStepSize,
            float upThreshold
    );

    virtual std::vector<int> getFrequencies(
            const std::vector<int> &oldFrequencies,
            const std::vector<bool> &activeCores);

private:
    const PerformanceCounters *performanceCounters;

    std::vector<CoreLearningMaterial> learnModules;
    std::vector<int> lastRoundFrequencies;
    unsigned int coreRows;
    unsigned int coreColumns;
    int minFrequency;
    int maxFrequency;
    int frequencyStepSize;
    float upThreshold;
    bool init = true;
};

#endif //HOTSNIPER_DVFSEDGECOOLINGMODE_H
