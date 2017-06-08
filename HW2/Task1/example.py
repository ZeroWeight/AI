from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
import tensorflow as tf
import argparse
import sys
import numpy as np
import random as r
from tensorflow.python.framework import graph_util  
from Query import *
if sys.version_info[0] == 2:
    import cPickle as pickle
else:
    import pickle
if __name__ == '__main__': 
    f = open("./predict.txt", "w+")
    fo = open('../model/train_data.pkl', 'rb')
    if sys.version_info[0] == 2:
        entry = pickle.load(fo)
    else:
        entry = pickle.load(fo, encoding='latin1')
    fo.close()
    Data = entry['valid_data1']
    Label = entry['valid_labels1']
    Y = GreyQuery(Data)
    P = Label == Y
    print("test accuracy\t"+str(np.mean(P)))
    for y in Y:
        f.write('%d\n' % y)

