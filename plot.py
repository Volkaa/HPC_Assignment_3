import numpy as np
import matplotlib.pyplot as plt

process = [8,16,32,64,128]
#time21 = [0.080388,0.044158,0.027598,0.017776,0.013891]
#time22 = [0.077933,0.041843,0.024631,0.011703,0.006018]
#time23 = [0.077700,0.041200,0.023298,0.014217,0.009796]
#time24 = [0.078813,0.041690,0.027428,0.011702,0.006060]
#time25 = [0.078039,0.041539,0.024517,0.011754,0.006009]
time26 = [0.083961,0.050570,0.032887,0.032713,0.101830]

plt.plot(process,time26)
plt.xlabel('Number of processes')
plt.ylabel('Execution time (s)')
plt.savefig('Ex2_6.png')