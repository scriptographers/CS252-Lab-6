import re

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

sns.set_theme(style="darkgrid")
sns.set(rc={'figure.figsize': (12, 10)})

data = {'Congestion Protocol': [], 'Delay': [], 'Loss': [], 'Throughput (in kbps)': []}

for tcp in ['TCP-Reno', 'TCP-Cubic']:
    t = open('{}.txt'.format(tcp[4:])).readlines()

    for i in range(9):
        for j in range(20):
            data['Congestion Protocol'].append(tcp)
            data['Delay'].append(t[21 * i].split()[0])
            data['Loss'].append(t[21 * i].split()[1])
            # Regex to get time taken
            time = float((re.findall(r'\d+\.\d+', t[21 * i + j + 1]))[0])
            data['Throughput (in kbps)'].append(5 * 1024 * 8 / time)

        #calculating mean and standard deviation for each experiment
        mymean=np.mean(data['Throughput (in kbps)'][21*i : 21*(i+1)])
        mystd=np.std(data['Throughput (in kbps)'][21*i : 21*(i+1)])
        print(f"Experiment {i+1} with {tcp}:\t Delay={t[21 * i].split()[0]},\t Loss={t[21 * i].split()[1]}:\t mean={mymean} kbps,\t std deviation={mystd} kbps")


df = pd.DataFrame(data)
#print(df)

#to generate confidence intervals plots:

g = sns.pointplot(x='Delay', y='Throughput (in kbps)',
                  hue='Congestion Protocol', data=df[df['Loss'] == '0.1%'],
                  ci=90, dodge=True, capsize=0.02).set_title('Loss 0.1%')
plt.savefig('../plots/0.1% loss.png')
plt.clf()

g = sns.pointplot(x='Delay', y='Throughput (in kbps)',
                  hue='Congestion Protocol', data=df[df['Loss'] == '0.5%'],
                  ci=90, dodge=True, capsize=0.02).set_title('Loss 0.5%')
plt.savefig('../plots/0.5% loss.png')
plt.clf()

g = sns.pointplot(x='Delay', y='Throughput (in kbps)',
                  hue='Congestion Protocol', data=df[df['Loss'] == '1%'],
                  ci=90, dodge=True, capsize=0.02).set_title('Loss 1.0%')
plt.savefig('../plots/1.0% loss.png')
plt.clf()

g = sns.pointplot(x='Loss', y='Throughput (in kbps)',
                  hue='Congestion Protocol', data=df[df['Delay'] == '10ms'],
                  ci=90, dodge=True, capsize=0.02).set_title('Delay 10ms')
plt.savefig('../plots/10ms delay.png')
plt.clf()

g = sns.pointplot(x='Loss', y='Throughput (in kbps)',
                  hue='Congestion Protocol', data=df[df['Delay'] == '50ms'],
                  ci=90, dodge=True, capsize=0.02).set_title('Delay 50ms')
plt.savefig('../plots/50ms delay.png')
plt.clf()

g = sns.pointplot(x='Loss', y='Throughput (in kbps)',
                  hue='Congestion Protocol', data=df[df['Delay'] == '100ms'],
                  ci=90, dodge=True, capsize=0.02).set_title('Delay 100ms')
plt.savefig('../plots/100ms delay.png')
plt.clf()
