#This is a code to create the RandomForest from the data in the databank, as said in the RandomForest file, we strongly
#recommend you to create and collect your own data as this will make your results more precise as they will be adapted to your
#situation

import numpy as np 
import pandas as pd

import h5py

from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score

from micromlgen import port

def correlations(parameter):
    parameterPandas = pd.DataFrame(parameter)
    corr = parameterPandas.corr().fillna(0)
    tr_upper = np.triu(np.ones(corr.shape), k=1).astype('bool')
    return corr.where(tr_upper).stack().to_numpy()

def media(parameter):
    return np.mean(parameter,axis=0)


def createFeatures(dataset):
    used_features = [correlations,media]
    features = []
    for sample in dataset:
        features.append(np.hstack([feature(sample) for feature in used_features]))
    print(features[0].shape)
    return features

dataBank = h5py.File('dataBankRigel.h5','a')
x = np.array(dataBank['x'])
y = np.array(dataBank['y'])

meanX = x.reshape(-1,30).mean(axis=0)
stdX = x.reshape(-1, 30).std(axis=0)
print(meanX)
print(stdX)

x = (x-meanX)/stdX

(x_train,x_test,y_train,y_test) = train_test_split(x, y, test_size=0.15, random_state=42)

x_features_train = createFeatures(x_train)
x_features_test = createFeatures(x_test)

classifier = RandomForestClassifier(n_estimators=100, max_features=0.5, max_samples=0.5,
                                    bootstrap=True, verbose=1)
classifier.fit(x_features_train,y_train)

predicted = classifier.predict(x_features_test)
print(accuracy_score(y_test,predicted))

with open('RandomForestRegressor.h', 'w') as file:
    file.write(port(classifier))

dataBank.close()
