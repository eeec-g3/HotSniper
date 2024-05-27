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
    double beta = 0.0;
    double epsilon = 0.0;
};

typedef std::vector<alphaEpsilonPair> DVFS_META ;

class CoreLearningMaterial {
private:
    float temperature_1 = 0.0;
    float temperature_2 = 0.0;
    int frequency_1 = 0;
    int frequency_2 = 0;
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
        this->temperature_1 = this->temperature_2;
	this->frequency_1 = this->frequency_2;
	this->temperature_2 = temp;
        this->frequency_2 = freq;
    }

    void calculateAlphaEpsilon(float curr_temp, int curr_freq) {
        float old_temp_1 = this->temperature_1;
        float old_temp_2 = this->temperature_2;
        int old_freq_1 = this->frequency_1;
        int old_freq_2 = this->frequency_2;
        
	std::cout << "curr and old freq: " << std::to_string(curr_freq) << " " << std::to_string(old_freq_1) << " " << std::to_string(old_freq_2) << std::endl;

        double a_nomi = (old_temp_1 - old_temp_2) - ((double) (old_freq_1 - old_freq_2) / (old_freq_1 - curr_freq)) * (old_temp_1 - curr_temp);
        double a_deno = (old_freq_1 - old_freq_2) * (old_freq_2 - curr_freq);
	double a = a_nomi / a_deno;
	
	double b_nomi = (old_temp_1 - old_temp_2) - ((double) ((old_freq_1 - old_freq_2) * (old_freq_1 + old_freq_2)) / ((old_freq_1 - curr_freq) * (old_freq_1 + curr_freq))) * (old_temp_1 - curr_temp); 
	double b_deno = (old_freq_1 - old_freq_2) * (1 - (double) (old_freq_1 + old_freq_2) / (old_freq_1 + curr_freq)); 
	double b = b_nomi / b_deno;
	
	double e = old_temp_1 - a * old_freq_1 * old_freq_1 - b * old_freq_1;

        std::cout << "a & b & e: " << a << ' ' << b << ' ' << e  << std::endl;
        // std::cout << "a & freq: " << std::to_string(a * (double) curr_freq) << std::endl;

        auto pair = alphaEpsilonPair {};
        pair.level = this->start_freq - curr_freq;
        pair.alpha = a;
        pair.beta = b;
	pair.epsilon = e;
        dvfs_metadata.push_back(pair);
    }

    float get_temperature() const {
        return this->temperature_2;
    }

    int get_frequency() const {
        return this->frequency_2;
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
