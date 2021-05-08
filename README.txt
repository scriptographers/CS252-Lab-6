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

- Running a single file transfer experiment:
    - cd to src/
    - Execute: bash run.sh <congestion_protocol> <flag_gen_file> <file_size>
        - <congestion_protocol> : "reno" or "cubic"
        - <flag_gen_file> : 0 if send.txt is already defined, 1 if you want to generate a fresh send.txt of size <file_size> bytes
        - <file_size> : Size of send.txt generated
        - Example: bash run.sh cubic 1 5MB

Required plots are present in plots/


