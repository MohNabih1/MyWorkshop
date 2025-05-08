#Mohamad Nabih Alkhateeb
#ID: 1002170786
#03/14/2025
#Task 1:

#Exta credit (bonus) was attempted 
#The code will prompt the user to enter the value of K
#The predicted classes of this dataset is going to get printed to "outputs.txt"
#And the it also includes a plotted graph of different classes using different colors and shapes
# using both Manhattan nad Euclidean distances.


import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from collections import Counter
from sklearn.model_selection import train_test_split

# function to calculate Euclidean distance
def euclidean_distance(x1, x2):
    return np.sqrt(np.sum((np.array(x1) - np.array(x2)) ** 2))

# function to calculate Manhattan distance
def manhattan_distance(x1, x2):
    return np.sum(np.abs(np.array(x1) - np.array(x2)))

# KNN definition
class KNN:
    def __init__(self, k=3, distance_metric='euclidean'):
        self.k = k
        self.distance_metric = distance_metric
        self.distance_function = euclidean_distance if distance_metric == 'euclidean' else manhattan_distance

    def fit(self, X, y):
        self.X_train = X
        self.y_train = y

    def predict(self, X_test):
        return np.array([self._predict(x) for x in X_test])

    def _predict(self, x):
        # calculate distances for training data
        distances = [self.distance_function(x, x_train) for x_train in self.X_train]
        k_indices = np.argsort(distances)[:self.k]
        k_nearest_labels = [self.y_train[i] for i in k_indices]
        most_common = Counter(k_nearest_labels).most_common(1)
        return most_common[0][0]

# manually loading data from the diabetes.csv file
df = pd.read_csv("diabetes.csv")

# specifing features from the dataset
X = df[['Glucose', 'BMI']].values
y = df['Outcome'].values

# splitting the data between training and testing 
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# promting the user for entering the value of k - Bonus 
k = int(input("Enter the value of k: "))


knn_euclidean = KNN(k=k, distance_metric='euclidean')
knn_euclidean.fit(X_train, y_train)
y_pred_euclidean = knn_euclidean.predict(X_test)


knn_manhattan = KNN(k=k, distance_metric='manhattan')
knn_manhattan.fit(X_train, y_train)
y_pred_manhattan = knn_manhattan.predict(X_test)

# outputting the results to outputs.txt
with open("outputs.txt", "w") as f:
    f.write("Glucose   BMI   Eucledian Prediction   Manhattan Prediction\n")
    for i in range(len(X_test)):
        f.write(f"{X_test[i][0]:<10} {X_test[i][1]:<10} {y_pred_euclidean[i]:<20} {y_pred_manhattan[i]}\n")

# plot using Eucluedian
plt.figure(figsize=(8, 6))
plt.title(f"KNN Classification (k={k}, Euclidean Distance)")
for label in np.unique(y_train):
    plt.scatter(X_train[y_train == label][:, 0], X_train[y_train == label][:, 1],
                label=f"Class {label}", s=100, edgecolors="k")
# plot test points with predictions
colors = ['red' if p == 0 else 'blue' for p in y_pred_euclidean]
plt.scatter(X_test[:, 0], X_test[:, 1], c=colors, marker='*', s=200,
            label="Test Points", edgecolors='black')
plt.xlabel("Glucose")
plt.ylabel("BMI")
plt.legend()
plt.show()

# plot using Manhattan 
plt.figure(figsize=(8, 6))
plt.title(f"KNN Classification (k={k}, Manhattan Distance)")
for label in np.unique(y_train):
    plt.scatter(X_train[y_train == label][:, 0], X_train[y_train == label][:, 1],
                label=f"Class {label}", s=100, edgecolors="k")
# plot test points with predictions
colors = ['red' if p == 0 else 'blue' for p in y_pred_manhattan]
plt.scatter(X_test[:, 0], X_test[:, 1], c=colors, marker='*', s=200,
            label="Test Points", edgecolors='black')
plt.xlabel("Glucose")
plt.ylabel("BMI")
plt.legend()
plt.show()
