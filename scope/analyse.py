import numpy as np
import logging
import matplotlib.pyplot as plt

LOGGER = logging.getLogger(__name__)
logging.basicConfig(format='%(message)s', level=logging.INFO)

def detectBursts(diffs):
    """Detect individual bursts in capture."""

    m = np.median(diffs)
    th = m * 15

    labels = np.empty(len(t), dtype=np.int32)
    label = 0
    for i in range(0, len(t)):
        if diffs[i] > th:
            label += 1            
        labels[i] = label

    nbursts = len(np.unique(labels))
    
    return nbursts, labels, [np.where(labels==id)[0] for id in range(0, nbursts)]

def computeBurstStats(nbursts, ids, diffs):
    """Computes pairwise bursts on/off time differences and inter burst times."""

    intra = np.empty((nbursts, nbursts), dtype=complex)
    for i in range(0, nbursts-1):
        for j in range(i, nbursts):
            ids0, ids1 = ids[i][1:], ids[j][1:]
            if len(ids0) != len(ids1):
                intra[i, j] = intra[j, i] = complex(np.nan, np.nan)
            else:
                deltas = []
                for i0, i1 in zip(ids0, ids1):
                    deltas.append(abs(diffs[i0] - diffs[i1]))
                intra[i, j] = intra[j, i] = complex(np.mean(deltas), np.std(deltas))

    inter = np.empty(nbursts - 1, dtype=float)
    for i in range(1, nbursts):
        inter[i-1] = diffs[ids[i][0]]

    return intra, inter

def saveBurts(nbursts, ids, t, prefix="burst_"):
    for i in range(0, nbursts):
        np.savetxt(
            "{}{}.csv".format(prefix, i), 
            t[ids[i]] - t[ids[i][0]],
            fmt='%u',
            delimiter=' ', 
            header='Generated by https://github.com/cheind/arduino-infrared')



   
t = np.loadtxt("capture.csv")
diffs = (t-np.roll(t,1))
diffs[0] = 0
nbursts, labels, ids = detectBursts(diffs)


intra, inter = computeBurstStats(nbursts, ids, diffs)
np.set_printoptions(suppress=True, precision=3)
print(np.real(intra))
np.set_printoptions(suppress=True, precision=3)
print(np.imag(intra))
np.set_printoptions(suppress=True, precision=3)
print(inter)

s = np.empty(len(t))
s[::2] = 0
s[1::2] = 1

plt.xlabel("Time $us$")
plt.ylabel("Value")

for i in range(0, nbursts):
    plt.step(t[ids[i]], s[ids[i]], where='post')

plt.ylim(-0.5, 1.5)
plt.show()
plt.close()

saveBurts(nbursts, ids, t)