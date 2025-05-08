#Mohamad Nabih Alkhateeb
#ID: 1002170786
#03/14/2025
#Task 2: 

#use the token <banknote> to load data from: data_banknote_authentication.txt"


import numpy as np
import sys
import pandas as pd
from sklearn.neighbors import KNeighborsClassifier
from sklearn.svm import SVC
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
from sklearn.datasets import load_iris, load_breast_cancer

class MLChoice:
    def __init__(self, model, dataset):
        # initialize the chosen model and dataset
        self.model = model.lower()
        self.dataset = dataset.lower()
        # loading and spliting the data
        self.X_train, self.X_test, self.y_train, self.y_test = self.load_data()

    def load_data(self):
        # loading the dataset based on the user's choice
        if self.dataset == "iris":
            data = load_iris()
            X = data.data
            y = data.target  
        elif self.dataset == "banknote":
            df = pd.read_csv("data_banknote_authentication.txt", header=None)
            X = df.iloc[:, :-1].values  # features
            y = df.iloc[:, -1].values  # labels
        elif self.dataset == "breast_cancer":
            data = load_breast_cancer()
            X = data.data
            y = data.target  # labels (0: Benign, 1: Malignant)
        else:
            raise ValueError("Invalid dataset selection")

        # spliting data into training and testing 
        return train_test_split(X, y, test_size=0.2, random_state=42)

    def knn_from_scratch(self, k=3):
        # knn implementation using euclidian 
        def euclidean_distance(x1, x2):
            return np.sqrt(np.sum((x1 - x2) ** 2))

        predictions = []
        
        for test_sample in self.X_test:
            
            distances = [euclidean_distance(test_sample, x) for x in self.X_train]
            nearest_indices = np.argsort(distances)[:k]
            nearest_labels = self.y_train[nearest_indices]
            predictions.append(np.bincount(nearest_labels).argmax())

        return np.array(predictions)
    
    def svm_from_scratch(self, learning_rate=0.001, lambda_param=0.01, epochs=600):
        # SVM implementation 
        X = self.X_train
        y = np.where(self.y_train <= 0, -1, 1)  # converting labels to -1 and 1

        w = np.zeros(X.shape[1])  # initialize weights
        b = 0  # initialize bias

        for _ in range(epochs):
            for idx, x_i in enumerate(X):
                if y[idx] * (np.dot(x_i, w) + b) >= 1:
                    w -= learning_rate * (2 * lambda_param * w)
                else:
                    w -= learning_rate * (2 * lambda_param * w - np.dot(x_i, y[idx]))
                    b -= learning_rate * y[idx]

        
        predictions = np.sign(np.dot(self.X_test, w) + b)
        return np.where(predictions == -1, 0, 1)

    def train_and_evaluate(self):
        # training data based on the selected model by the user 
        if self.model == "knn":
            # predictions using custom implementation
            y_pred_scratch = self.knn_from_scratch()
            # predictions using sklearn's implementation
            knn = KNeighborsClassifier(n_neighbors=3)
            knn.fit(self.X_train, self.y_train)
            y_pred_sklearn = knn.predict(self.X_test)
        elif self.model == "svm":
            # predictions using custom implementation
            y_pred_scratch = self.svm_from_scratch()
            # predictions using sklearn's implementation
            svm = SVC(kernel='linear')
            svm.fit(self.X_train, self.y_train)
            y_pred_sklearn = svm.predict(self.X_test)
        else:
            raise ValueError("Invalid machine learning model")

        
        accuracy_scratch = accuracy_score(self.y_test, y_pred_scratch) * 100
        accuracy_sklearn = accuracy_score(self.y_test, y_pred_sklearn) * 100
        print(f"DataSet: {self.dataset.capitalize()}")
        print(f"Machine Learning Algorithm Chosen: {self.model.upper()}")
        print(f"Accuracy of Training (Scratch): {accuracy_scratch:.2f}%")
        print(f"Accuracy of ScikitLearn Function: {accuracy_sklearn:.2f}%")

        # predicting a random sample
        random_idx = np.random.randint(0, len(self.X_test))
        test_point = self.X_test[random_idx].tolist()
        actual_class = self.y_test[random_idx]

        if self.model == "knn":
            predicted_class = knn.predict([test_point])[0]
        elif self.model == "svm":
            predicted_class = svm.predict([test_point])[0]

        print("\nPrediction Point:", test_point)
        print("Predicted Class:", predicted_class)
        print("Actual Class:", actual_class)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python MLChoice.py <model> <Dataset>")
    else:
        
        model = sys.argv[1]
        dataset = sys.argv[2]
        model = MLChoice(model, dataset)
        model.train_and_evaluate()

