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
│   │   ├── ...
│   │   ├── PeriodicPower.log.gz
│   │   ├── PeriodicRvalue.log.gz
│   │   ├── ...
│   │   ├── PeriodicThermal.log.gz
│   │   ├── PeriodicVdd.log.gz
│   │   └── ...
│   ├── results_2024-04-21_15.32_4.0GHz+maxFreq+slowDVFS_parsec-blackscholes-simsmall-2
│   │   └── ...
│   └── results_2024-04-21_15.32_4.0GHz+maxFreq+slowDVFS_parsec-blackscholes-simsmall-3
│   │   └── ...
├── bodytrack
│   ├── results_2024-04-22_17.56_4.0GHz+maxFreq+slowDVFS_parsec-bodytrack-simsmall-1
│   │   └── ...
│   └── results_2024-04-22_17.56_4.0GHz+maxFreq+slowDVFS_parsec-bodytrack-simsmall-2
│   │   └── ...
├── canneal
│   └── ...
├── dedup
│   └── ...
├── fluidanimate
│   └── ...
├── streamcluster
│   └── ...
├── swaptions
│   └── ...
└── x264
    └── ...
```
