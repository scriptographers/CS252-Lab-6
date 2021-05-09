CS252: Lab 6

Team Members:
- Harshit Varma (190100055)
- Devansh Jain (190100044)
- Krushnakant Bhattad (190100036)

File Description:
- src/client.c                : Client code
- src/server.c                : Server code
- src/run.sh                  : Bash script for a single file transfer run
- src/experiments.sh          : Bash script for the experiments
- src/confidence_intervals.py : Python script for generating plots
- src/send.txt                : The send file from the experiment
- src/recv.txt                : The recv file from the experiment
- src/Reno.txt                : The Reno log file from the experiment
- src/Cubic.txt               : The Cubic log file from the experiment
- plots/*.png                 : Plots comparing TCP-Reno and TCP-Cubic
- wireshark_graphs/*.png      : Annotated Window Scaling graphs

Usage:
- Conducting the experiments and generating plots:
    - cd to src/
    - Execute: bash experiments.sh
        - It will save time taken in Reno.txt for TCP-Reno runs and Cubic.txt for TCP-Cubic runs
        - Assumption: The loopback interface is lo.
    - After completion of experiments, execute: python3 confidence_intervals.py
        - Required plots are generated in plots/
        - Assumption: pandas, matplotlib and seaborn are installed.

- Running a single file transfer:
    - cd to src/
    - Execute: bash run.sh <congestion_protocol> <flag_gen_file> <file_size> <flag_compile>
        - <congestion_protocol> : "reno" or "cubic"
        - <flag_gen_file> : 0 if send.txt is already defined, 1 if you want to generate a fresh send.txt of size <file_size> bytes
        - <file_size> : Size of send.txt generated
        - <flag_compile> : 0 if client.c and server.c are already compiled, 1 if you want to recompile client.c and server.c
        - Example: bash run.sh cubic 1 5MB 1
