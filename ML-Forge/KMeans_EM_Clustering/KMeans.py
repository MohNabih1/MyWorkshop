#Mohamad Nabih Alkhateeb
#1002170786


"""This script implements the K-Means clustering algorithm from scratch on a 2D 
   dataset providedn 'ClusteringData.txt'.It manually initializes K centroids,
   assigns data points to the nearest centroid using Euclidean distance, and updates
   centroids by calculating the mean of points in each cluster. The algorithm iterates
   until the cluster assignments stop changing or a maximum number of iterations is reached.
   During execution, the script generates and saves three high-resolution PNG visualizations
   showing the clustering process at the initial, middle, and final stages."""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import random

# loading data
def load_data(file_path):
    data = pd.read_csv(file_path, header=None)  # Assumes no header in the dataset file.
    return data.values

# select a random k value to initialize centroids for clustering.
def initialize_centroids(data, K):
    random_indicies = random.sample(range(len(data)), K)  
    return data[random_indicies]

# assigning data ponits to clusters based on the nearest distance 
def assign_clusters(data, centroids):
    # using Eucledian distance to calculate the distances 
    distances = np.linalg.norm(data[:, np.newaxis] - centroids, axis=2)
    return np.argmin(distances, axis=1)

# update centroids by calculating the mean of all points assigned to each cluster
def recalculate_centroids(data, labels, K):
    #compute new centroids for each k 
    return np.array([data[labels == k].mean(axis=0) for k in range(K)])

# creates the visulas 
def plot(data, centroids, labels, K, iteration, label):
    plt.figure()
    for k in range(K):  
        cluster_data = data[labels == k]  
        plt.scatter(cluster_data[:, 0], cluster_data[:, 1], label=f'Cluster {k+1}')
    
    plt.scatter(centroids[:, 0], centroids[:, 1], color='black', marker='X', s=200, label='Centroids')
    plt.title(f'K={K} - {label} (Iteration {iteration})')
    plt.legend()
    filename = f'K{K}_Iteration{iteration}_{label}.png'
    plt.savefig(filename, dpi=600)
    plt.close()


def KMeans(datasetFile, K=2):
    data = load_data(datasetFile)  
    # intitilized the centroid 
    centroids = initialize_centroids(data, K)  
    previous_labels = None  
    iteration = 0  
    mid_iteration = None  
    max_iteration_limit = 100  # limit on the number on iterations to prevent infinite loops

    while True:
        labels = assign_clusters(data, centroids)  # Assign points to the closest centroids.

        if iteration == 0:  # plotting the cluster for the initial iteration
            plot(data, centroids, labels, K, iteration + 1, 'Initial')

        
        if mid_iteration is None and iteration > 1:
            mid_iteration = iteration
            plot(data, centroids, labels, K, iteration + 1, 'Middle')

        new_centroids = recalculate_centroids(data, labels, K)  
        iteration += 1  

        #terminate if the assignment for clustering do not change 
        if previous_labels is not None and np.array_equal(labels, previous_labels):
            break
        # for saftey: terminate if the saftey limit is reached 
        if iteration >= max_iteration_limit:
            break

        
        centroids = new_centroids
        previous_labels = labels.copy()

    
    plot(data, centroids, labels, K, iteration, 'Final')
    print(f"Finished clustering with K={K} in {iteration} iterations.")


if __name__ == "__main__":
    KMeans("ClusteringData.txt") # default K = 2 is used when K is not provided
