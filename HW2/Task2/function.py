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

def RGBQuery(pic):
    Tx = np.reshape(pic,[-1,28*28,3])
    Ty = np.zeros(len(Tx))
    X1 = np.zeros((len(Tx),28*28))
    X2 = np.zeros((len(Tx),28*28))
    X = np.zeros((len(Tx),28*28))
    for i in range(len(Tx)):
        X1[i] = kcluster(Tx[i])
        X2[i] = pca(Tx[i])
        X[i,:] = np.array([X1[i],X2[i]]).min(axis=0)

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
            y_out = sess.run(y, feed_dict={x:X[i],k:1})  
            Ty[i] = np.argmax(y_out)
    
    return Ty

def pca(ori):
    mat = np.reshape(ori,[28*28,3])
    meanVal = np.mean(mat,axis=0)
    newData = mat - meanVal
    covMat = np.cov(newData,rowvar=0)
    eigVals,eigVects=np.linalg.eig(np.mat(covMat))
    eigVect=eigVects[:,np.argmax(eigVals)]
    mat1=newData*eigVect
    mat1 = (mat1 - min(mat1))/(max(mat1) - min(mat1)) * 255
    if np.mean(mat1) > 128:
        mat1 = 255 - mat1
    mat1 = np.reshape(mat1,[28*28])
    return mat1

def euclDistance(vector1, vector2):  
    return np.sqrt(sum(np.power(vector2 - vector1, 2)))  

def initCentroids(dataSet, k):  
    numSamples, dim = dataSet.shape  
    centroids = np.zeros((k, dim))  
    for i in range(k):  
        index = int(r.uniform(0, numSamples))  
        centroids[i, :] = dataSet[index, :]  
    return centroids  

def kmeans(dataSet, k):  
    numSamples = dataSet.shape[0]    
    clusterAssment = np.zeros(numSamples)
    clusterChanged = True   
    centroids = initCentroids(dataSet, k)  
    while clusterChanged:  
        clusterChanged = False  
        for i in xrange(numSamples):  
            minDist  = 100000.0  
            minIndex = 0  
            for j in range(k):  
                distance = euclDistance(centroids[j, :], dataSet[i, :])  
                if distance < minDist:  
                    minDist  = distance  
                    minIndex = j  
              
            if clusterAssment[i] != minIndex:  
                clusterChanged = True  
                clusterAssment[i] = minIndex
  
        for j in range(k):  
            pointsInCluster = dataSet[np.nonzero(clusterAssment[:] == j)]  
            centroids[j, :] = np.mean(pointsInCluster, axis = 0)  
    return clusterAssment  

def kcluster(ori):
    mat = np.reshape(ori,[28*28,3])
    clusterAssment = kmeans(mat,2)
    mat1 = np.zeros(28*28)
    for i in range (28*28):
        if clusterAssment[i] == 0:
            mat1[i] = 0
        else:
            mat1[i] = 255

    if np.mean(mat1) >128:
        mat1 = 255-mat1

    return mat1

