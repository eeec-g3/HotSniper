//
// Created by yanghoo on 5/11/24.
//

#include "dvfsEdgeCoolingMode.h"
#include <iostream>
#include <string>

using namespace std;

DVFSEdgeCoolingMode::DVFSEdgeCoolingMode(
        const PerformanceCounters *performanceCounters,
        int coreRows,
        int coreColumns,
        int minFrequency,
        int maxFrequency,
        int frequencyStepSize,
        float upThreshold)
        : performanceCounters(performanceCounters),
          coreRows(coreRows),
          coreColumns(coreColumns),
          minFrequency(minFrequency),
          maxFrequency(maxFrequency),
          frequencyStepSize(frequencyStepSize),
          upThreshold(upThreshold) {

    this->init = true;
    this->lastRoundFrequencies = vector<int>(this->coreRows * this->coreColumns);

    for (unsigned int i = 0; i < this->coreRows * this->coreColumns; i++) {
        learnModules.push_back(CoreLearningMaterial(upThreshold));
    }
}

std::vector<int> DVFSEdgeCoolingMode::getFrequencies(
        const std::vector<int> &oldFrequencies,
        const std::vector<bool> &activeCores
) {
    std::vector<int> frequencies(coreRows * coreColumns);

    for (unsigned int coreCounter = 0; coreCounter < coreRows * coreColumns; coreCounter++) {
        float power = performanceCounters->getPowerOfCore(coreCounter);
        float temperature = performanceCounters->getTemperatureOfCore(coreCounter);
        int core_freq = oldFrequencies.at(coreCounter);
        float utilization = performanceCounters->getUtilizationOfCore(coreCounter);
        cout << "[Scheduler][edgeCoolingMode]: Core " << coreCounter << ":";
        cout << " P=" << fixed <<  power << " W";
        cout << " f=" << core_freq << " MHz";
        cout << " T=" << fixed << temperature << " C";
        cout << " utilization=" << fixed << utilization << endl;

        if (utilization <= 0.05) {
            frequencies.at(coreCounter) = this->minFrequency;
            continue;
        }

        if (activeCores.at(coreCounter)) {
            CoreLearningMaterial* learnModule = &learnModules[coreCounter];

            if (learnModule->init) {
                frequencies.at(coreCounter) = this->maxFrequency;
                cout << "[Scheduler][edgeCoolingMode]: Init core: " << to_string(coreCounter) << " with " << to_string(this->maxFrequency) << endl;

                learnModule->init = false;
                continue;
            }

            if (temperature > upThreshold) {
                cout << "Jump to minimum frequency" << endl;
                frequencies.at(coreCounter) = this->minFrequency;
                learnModule->MARGIN -= 0.02;
                cout << "Current Margin: " << to_string(learnModule->MARGIN) << endl;

            } else if (temperature >= upThreshold * learnModule->MARGIN) {
                if (learnModule->state == LEARN_STATE::WAIT || learnModule->state == LEARN_STATE::RUN) {
                    if (learnModule->state == LEARN_STATE::WAIT) {
                        learnModule->start_freq = core_freq;
                    }
                    // All cores remain the same frequency.
                    cout << "start learning" << endl;

                    float dvfs_size = learnModule->step_size;
                    frequencies.at(coreCounter) = oldFrequencies[coreCounter] - dvfs_size;

                    if (learnModule->get_frequency() != 0 || learnModule->get_frequency() == core_freq) {
                        learnModule->calculateAlphaEpsilon(temperature, core_freq);
                    }
                    cout << "frequency in learning module: " << to_string(learnModule->get_frequency()) << endl;
                    cout << "core frequency: " << to_string(core_freq) << endl;

                    learnModule->state = LEARN_STATE::RUN;
                } else {
                    int l_prev = 0;
                    bool find_expected = false;
                    for (auto pair : learnModule->dvfs_metadata) {
                        int l_i = pair.level;
                        double a = pair.alpha;

                        float target_temp = learnModule->MARGIN * this->upThreshold;

                        // The predictive value.
                        int l_this = round((temperature - target_temp) / a);
                        cout << "calculated level" << to_string(l_this) << endl;
                        cout << "l_i" << to_string(l_i) << endl;
                        cout << "a: " << to_string(pair.alpha) << " e: " << to_string(pair.epsilon) << endl;

                        if (l_this <= l_i && l_this > 0) {
                            cout << "expected level" << to_string(l_this) << endl;
                            frequencies.at(coreCounter) = oldFrequencies.at(coreCounter) - l_this;
                            find_expected = true;
                            break;

                        } else {
                            l_prev = l_this;
                        }
                    }
                    if (!find_expected) {
                        if (l_prev > 0 && (oldFrequencies.at(coreCounter) - l_prev) >= 500) {
                            cout << "not optimized level" << to_string(l_prev) << endl;
                            frequencies.at(coreCounter) = oldFrequencies.at(coreCounter) - l_prev;
                        } else {
                            frequencies.at(coreCounter) = oldFrequencies.at(coreCounter) - learnModule->step_size;
                        }
                    }
                    learnModule->reset(temperature);
                }
            } else {
                cout << "[Safe temperature, no need to adjust]" << endl;
                if (learnModule->state == LEARN_STATE::RUN) {
                    // Make it to the finish state.
                    cout << "[Scheduler][edgeCoolingMode]: INTO FINISH STATE" << endl;
                    learnModule->state = LEARN_STATE::FINISH;
                }
                // frequencies.at(coreCounter) = oldFrequencies.at(coreCounter) + learnModule->step_size;
                frequencies.at(coreCounter) = oldFrequencies.at(coreCounter) + (learnModule->step_size * 2);
            }
            learnModule->recordNewData(temperature, core_freq);
        }
    }

    for (int i = 0; i < 4; i++) {
        cout << "freq: " << to_string(frequencies.at(i)) << endl;
    }
    return frequencies;
}