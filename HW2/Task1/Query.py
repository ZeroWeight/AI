from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
import tensorflow as tf
import argparse
import sys
import numpy as np
import random as r
from tensorflow.python.framework import graph_util  

def GreyQuery(pic):
    Tx = np.reshape(pic,[-1,28*28])
    Ty = np.zeros(len(Tx))
    with tf.gfile.GFile('../model/frozen_model.pb', "rb") as f:  
        graph_def = tf.GraphDef()  
        graph_def.ParseFromString(f.read())  
    with tf.Graph().as_default() as graph:  
        tf.import_graph_def(  
            graph_def,   
            input_map=None,   
            return_elements=None,   
            name="prefix",   
            op_dict=None,   
            producer_op_list=None  
        )    
    x = graph.get_tensor_by_name('prefix/inputs_placeholder:0')  
    y = tf.reshape(graph.get_tensor_by_name('prefix/MatMul_1:0'),[1,10]) 
    k = graph.get_tensor_by_name('prefix/Placeholder:0') 
    with tf.Session(graph=graph) as sess:  
        for i in range(len(Tx)):
            y_out = sess.run(y, feed_dict={x:Tx[i],k:1})  
            Ty[i] = np.argmax(y_out)
    
    return Ty

