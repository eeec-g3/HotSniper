### Brief spec

`plot_power_pie.py` read the gzip file and decompress it, plot the pie chart.

`plot_bar.py` needs to read the gzip files( TODO ) and plot the bar chart.

Note: bar chart in `plot_bar.py` is calculated based on sum results, which is not suitable for temperature. Needs to modify if necessary.

We organize our experimental data according to the following hierarchy, where you can use `collect.sh` to aggregate the data you are interested in into specific directories. Here we assume that the plotting program handles log data in the logs folder.

```
.
├── blackschole
│   ├── results_2024-04-21_15.32_4.0GHz+maxFreq+slowDVFS_parsec-blackscholes-simsmall-1
│   │   ├── PeriodicCPIStack.log.gz
│   │   ├── PeriodicFrequency.log.gz
│   │   ├── PeriodicPower-C_0.png
│   │   ├── PeriodicPower-C_1.png
│   │   ├── PeriodicPower-C_2.png
│   │   ├── PeriodicPower-C_3.png
│   │   ├── PeriodicPower-Cores.png
│   │   ├── PeriodicPower.log.gz
│   │   ├── PeriodicRvalue.log.gz
│   │   ├── PeriodicThermal-C_0.png
│   │   ├── PeriodicThermal-C_1.png
│   │   ├── PeriodicThermal-C_2.png
│   │   ├── PeriodicThermal-C_3.png
│   │   ├── PeriodicThermal-Cores.png
│   │   ├── PeriodicThermal.log.gz
│   │   ├── PeriodicVdd.log.gz
│   │   ├── cpi-stack-trace-c0.png
│   │   ├── cpi-stack-trace-c1.png
│   │   ├── cpi-stack.png
│   │   ├── cpi-stack.txt
│   │   ├── cpi.png
│   │   ├── execution.log.gz
│   │   ├── executioninfo.txt
│   │   ├── frequency.png
│   │   ├── ips.png
│   │   ├── ipssmooth.png
│   │   ├── peak_temperature.png
│   │   ├── sim.cfg
│   │   ├── sim.info
│   │   ├── sim.out
│   │   ├── sim.stats.sqlite3
│   │   └── utilization.png
│   ├── results_2024-04-21_15.32_4.0GHz+maxFreq+slowDVFS_parsec-blackscholes-simsmall-2
│   │   └── ...
│   └── results_2024-04-21_15.32_4.0GHz+maxFreq+slowDVFS_parsec-blackscholes-simsmall-3
│   │   └── ...
├── bodytrack
│   ├── results_2024-04-22_17.56_4.0GHz+maxFreq+slowDVFS_parsec-bodytrack-simsmall-1
│   └── results_2024-04-22_17.56_4.0GHz+maxFreq+slowDVFS_parsec-bodytrack-simsmall-2
├── canneal
│   ├── results_2024-04-19_21.38_4.0GHz+maxFreq+slowDVFS_parsec-canneal-simsmall-3
│   ├── results_2024-04-20_02.22_4.0GHz+maxFreq+slowDVFS_parsec-canneal-simsmall-2
│   └── results_2024-04-20_13.23_4.0GHz+maxFreq+slowDVFS_parsec-canneal-simsmall-1
├── dedup
│   └── results_2024-04-23_22.14_4.0GHz+maxFreq+slowDVFS_parsec-dedup-simsmall-1
├── fluidanimate
│   ├── results_2024-04-23_10.11_4.0GHz+maxFreq+slowDVFS_parsec-fluidanimate-simsmall-1
│   └── results_2024-04-23_10.11_4.0GHz+maxFreq+slowDVFS_parsec-fluidanimate-simsmall-2
├── streamcluster
│   ├── results_2024-04-22_12.41_4.0GHz+maxFreq+slowDVFS_parsec-streamcluster-simsmall-1
│   ├── results_2024-04-22_12.41_4.0GHz+maxFreq+slowDVFS_parsec-streamcluster-simsmall-2
│   └── results_2024-04-22_12.41_4.0GHz+maxFreq+slowDVFS_parsec-streamcluster-simsmall-3
├── swaptions
│   ├── results_2024-04-20_20.09_4.0GHz+maxFreq+slowDVFS_parsec-swaptions-simsmall-1
│   ├── results_2024-04-20_20.09_4.0GHz+maxFreq+slowDVFS_parsec-swaptions-simsmall-2
│   └── results_2024-04-20_20.09_4.0GHz+maxFreq+slowDVFS_parsec-swaptions-simsmall-3
└── x264
    ├── results_2024-04-21_07.37_4.0GHz+maxFreq+slowDVFS_parsec-x264-simsmall-2
    ├── results_2024-04-21_07.37_4.0GHz+maxFreq+slowDVFS_parsec-x264-simsmall-3
    └── results_2024-04-23_19.13_4.0GHz+maxFreq+slowDVFS_parsec-x264-simsmall-1
```
