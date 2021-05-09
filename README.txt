CS252: Lab 6

Team Members:
- Harshit Varma (190100055)
- Krushnakant Bhattad (190100036)
- Devansh Jain (190100044)

Documentation:
- Client code: src/client.c
- Server code: src/server.c
- send.txt and recv.txt are present in src/ itself

Usage:
- Running the entire experiment and generating plots:
    - cd to src/
    - Execute: bash experiments.sh
        - It will save time taken for every run in Reno.txt for TCP-Reno runs and Cubic.txt for TCP-Cubic runs
        - Assumption: The loopback interface is lo.
    - After completion of experiments, execute: python3 confidence_intervals.py
        - Assumption: pandas, matplotlib and seaborn are installed.
    - Required plots are generated in plots/

- Running a single file transfer experiment:
    - cd to src/
    - Execute: bash run.sh <congestion_protocol> <flag_gen_file> <file_size> <flag_compile>
        - <congestion_protocol> : "reno" or "cubic"
        - <flag_gen_file> : 0 if send.txt is already defined, 1 if you want to generate a fresh send.txt of size <file_size> bytes
        - <file_size> : Size of send.txt generated
        - <flag_compile> : 0 if client.c and server.c are already compiled, 1 if you want to recompile client.c and server.c
        - Example: bash run.sh cubic 1 5MB 0
