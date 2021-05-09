import re

import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

sns.set_theme(style="darkgrid")
sns.set(rc={'figure.figsize': (16, 12)})

data = {'Congestion Protocol': [], 'Delay': [], 'Loss': [], 'Throughput (in kbps)': []}

for tcp in ['TCP-Reno', 'TCP-Cubic']:

    t = open('{}.txt'.format(tcp[4:])).readlines()

    for i in range(9):
        for j in range(20):
            data['Congestion Protocol'].append(tcp)
            data['Delay'].append(t[21 * i].split()[0])
            data['Loss'].append(t[21 * i].split()[1])
            time = float((re.findall(r'\d+\.\d+', t[21 * i + j + 1]))[0])
            data['Throughput (in kbps)'].append(5 * 1024 * 8 / time)

df = pd.DataFrame(data)
print(df)
