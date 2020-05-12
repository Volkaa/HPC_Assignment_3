import numpy as np
import matplotlib.pyplot as plt

a = np.loadtxt('internode.txt')
sizes = a[:,0]
time = a[:,1]

plt.scatter(sizes,time, s=20, label='data points')
plt.xlabel('Size of the message (Bytes)')
plt.ylabel('Execution time (s)')

(a,b) = np.polyfit(sizes, time, 1)
print(1/a,b)
plt.plot([0,1.12e9], [a*0+b, a*1.12e9+b], c='r', label='y = '+str(a)+'*x + '+str(b), lw=1)
plt.legend()
plt.savefig('Ex3_inter.png')

plt.show()

a = np.loadtxt('intranode.txt')
sizes = a[:,0]
time = a[:,1]

plt.scatter(sizes,time, s=20, label='data points')
plt.xlabel('Size of the message (Bytes)')
plt.ylabel('Execution time (s)')

(a,b) = np.polyfit(sizes, time, 1)
print(1/a,b)
plt.plot([0,1.12e9], [a*0+b, a*1.12e9+b], c='r', label='y = '+str(a)+'*x + '+str(b), lw=1)
plt.legend(loc='upper left')
plt.savefig('Ex3_intra.png')
plt.show()
