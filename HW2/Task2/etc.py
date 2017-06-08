from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
import tensorflow as tf
import argparse
import sys
import numpy as np
import random as r
from tensorflow.python.framework import graph_util  
from function import *
if sys.version_info[0] == 2:
    import cPickle as pickle
else:
    import pickle
if __name__ == '__main__': 
    f = open("predict.txt", "w+")
    fo = open('../model/train_data.pkl', 'rb')
    if sys.version_info[0] == 2:
        entry = pickle.load(fo)
    else:
        entry = pickle.load(fo, encoding='latin1')
    fo.close()
    Data = entry['train_data2']
    j=0
    TX = []
    TO = []
    for i in range(len(Data)):
        X = pca(Data[i])
        if np.var(X) >7500:
            TO.append(Data[i])
            TX.append(X)
            j += 1

    YY = GreyQuery(TX)
    print(j)
    TA = np.reshape(TO,[-1])
    print(len(TA))
    output = open('data0.pkl', 'wb')
    pickle.dump(YY, output)
    pickle.dump(TA,output)

