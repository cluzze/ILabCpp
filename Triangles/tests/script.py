import numpy as np
def generate(count = 10000, scale = 3):
    MAX = 100
    topx = np.random.random([count,3])*MAX
    topy = topx + abs(np.random.normal(0,scale,[count,3]))
    add = abs(np.random.normal(0,scale,[count,3]))
    add[:,0]*=0
    topz = add + topx
    top = np.concatenate([topx, topy, topz], axis=1)
    #top = (top*1000).astype(int)
    print(top)
    np.savetxt('tests/data.txt',top, fmt='%.2f')
generate()