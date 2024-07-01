#This is a code to create the RandomForest from the data in the databank, as said in the RandomForest file, we strongly
#recommend you to create and collect your own data as this will make your results more precise as they will be adapted to your
#situation

#imports the needed libraries
import numpy as np 
import pandas as pd
import h5py

#The train_test_split is used to split data into training and testing subsets. Useful for machine learning.
from sklearn.model_selection import train_test_split
#Imports the Random Forest itself, RandomForests are an classifier method
from sklearn.ensemble import RandomForestClassifier
#This evaluates the accuracy of the classifier
from sklearn.metrics import accuracy_score
#This library generates code for microcontrollers from machine learning models.
from micromlgen import port

#Define a function to compute correlations between features
def correlations(parameter):
    parameterPandas = pd.DataFrame(parameter)
    corr = parameterPandas.corr().fillna(0)
    tr_upper = np.triu(np.ones(corr.shape), k=1).astype('bool')
    return corr.where(tr_upper).stack().to_numpy()

#Define a function to compute the mean of features
def mean(parameter):
    return np.mean(parameter,axis=0)

#Create features by combining correlation and mean for each sample in the dataset
def createFeatures(dataset):
    used_features = [correlations,mean]
    features = []
    for sample in dataset:
        features.append(np.hstack([feature(sample) for feature in used_features]))
    #Print the shape of the features for the first sample
    print(features[0].shape) 
    return features

#Load data from an HDF5 file
dataBank = h5py.File('dataBankRigel.h5','a')
x = np.array(dataBank['x'])
y = np.array(dataBank['y'])

#Normalize features by subtracting mean and dividing by standard deviation
meanX = x.reshape(-1,30).mean(axis=0)
stdX = x.reshape(-1, 30).std(axis=0)
#Print the mean and the standard deviation of features
print(meanX)
print(stdX)
x = (x-meanX)/stdX

#Split data into training and testing sets
(x_train,x_test,y_train,y_test) = train_test_split(x, y, test_size=0.15, random_state=42)

#Create features for training and testing sets
x_features_train = createFeatures(x_train)
x_features_test = createFeatures(x_test)

#Train a RandomForest classifier
classifier = RandomForestClassifier(n_estimators=100, max_features=0.5, max_samples=0.5,
                                    bootstrap=True, verbose=1)
classifier.fit(x_features_train,y_train)

#Make predictions on the testing set
predicted = classifier.predict(x_features_test)
#Print the accuracy of predictions
print(accuracy_score(y_test,predicted))

#Generate code for the trained classifier and save it to a file
with open('RandomForestRegressor.h', 'w') as file:
    file.write(port(classifier))

#Close the file
dataBank.close()
