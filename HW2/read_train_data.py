import sys
import numpy
if sys.version_info[0] == 2:
    import cPickle as pickle
else:
    import pickle
 
fo = open('train_data.pkl', 'rb')
if sys.version_info[0] == 2:
    entry = pickle.load(fo)
else:
    entry = pickle.load(fo, encoding='latin1')
fo.close()
train_data2 = entry['train_data2']
train_data1 = entry['train_data1']
train_labels1 = entry['train_labels1']
valid_data1 = entry['valid_data1']
valid_labels1 = entry['valid_labels1']
valid_data2 = entry['valid_data2']
valid_labels2 = entry['valid_labels2']
import numpy, scipy.io
scipy.io.savemat('mydata0.mat', mdict={'train_data2': train_data2})
scipy.io.savemat('mydata1.mat', mdict={'train_data1': train_data1})
scipy.io.savemat('mydata2.mat', mdict={'train_labels1': train_labels1})
scipy.io.savemat('mydata3.mat', mdict={'valid_data1': valid_data1})
scipy.io.savemat('mydata4.mat', mdict={'valid_labels1': valid_labels1})
scipy.io.savemat('mydata5.mat', mdict={'valid_data2': valid_data2})
scipy.io.savemat('mydata6.mat', mdict={'valid_labels2': valid_labels2})
