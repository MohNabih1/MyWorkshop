#Mohamad Nabih Alkhateeb
#1002170786
#ExtraCredit attempted - not a member of the "Honors Contract"

"""This script implements EM algorithm for Gaussian Mixture Models (GMM)
   from scratch on a 2D dataset provided in 'ClusteringData.txt'. It manually initializes the parameters 
   (means, covariances, and weights), assigns responsibilities during the E-step, and updates the model 
   during the M-step. The algorithm iterates until the responsibilities converge or a maximum number of 
   iterations is reached. During execution, the script generates and saves three high-resolution PNG 
   visualizations showing the clustering process at the initial, middle, and final stages."""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import multivariate_normal
import random

# loading data
def load_data(file_path):
    data = pd.read_csv(file_path, header=None)  # assuming no header
    return data.values

# initialize the means, covariances, and weights of the Gaussians
def initialize_parameters(data, K):
    num_samples, num_features = data.shape
    means = data[np.random.choice(num_samples, K, replace=False)]  
    covariances = [np.cov(data.T) for _ in range(K)]  
    weights = np.ones(K) / K  
    return means, covariances, weights

# expectation step: calculate responsibilities for each Gaussian
def expectation_step(data, means, covariances, weights, K):
    responsibilities = np.zeros((data.shape[0], K))
    for k in range(K):
        rv = multivariate_normal(mean=means[k], cov=covariances[k])
        responsibilities[:, k] = weights[k] * rv.pdf(data)
    responsibilities /= responsibilities.sum(axis=1, keepdims=True)  # Normalize
    return responsibilities

# maximization step: update weights, means, and covariances
def maximization_step(data, responsibilities, K):
    num_samples = data.shape[0]
    Nk = responsibilities.sum(axis=0)  
    weights = Nk / num_samples  
    means = np.dot(responsibilities.T, data) / Nk[:, np.newaxis]  
    covariances = []
    for k in range(K):
        diff = data - means[k]
        cov = np.dot((responsibilities[:, k][:, np.newaxis] * diff).T, diff) / Nk[k]
        covariances.append(cov)
    return means, covariances, weights

# creates the visuals
def plot(data, responsibilities, means, K, iteration, label):
    plt.figure()
    labels = np.argmax(responsibilities, axis=1)  
    for k in range(K):
        cluster_data = data[labels == k]
        plt.scatter(cluster_data[:, 0], cluster_data[:, 1], label=f'Gaussian {k+1}')
    
    plt.scatter(means[:, 0], means[:, 1], color='black', marker='X', s=200, label='Means')
    plt.title(f'EM - K={K} - {label} (Iteration {iteration})')
    plt.legend()
    filename = f'EM_K{K}_Iteration{iteration}_{label}.png'
    plt.savefig(filename, dpi=600)
    plt.close()

# EM algorithm
def EM(datasetFile, K=2):
    data = load_data(datasetFile)  
    means, covariances, weights = initialize_parameters(data, K) 
    
    responsibilities = None
    previous_responsibilities = None
    iteration = 0
    mid_iteration = None
    max_iteration_limit = 100  # prevent infinite loops

    while True:
        responsibilities = expectation_step(data, means, covariances, weights, K)  # E-step

        if iteration == 0:  
            plot(data, responsibilities, means, K, iteration + 1, "Initial")
        
        if mid_iteration is None and iteration > 1:  # mid iteration visualization
            mid_iteration = iteration
            plot(data, responsibilities, means, K, iteration + 1, "Middle")

        means, covariances, weights = maximization_step(data, responsibilities, K)  # M-step
        iteration += 1

        # stopping criteria: if responsibilities do not change significantly
        if previous_responsibilities is not None and np.allclose(responsibilities, previous_responsibilities, atol=1e-4):
            break
        # saftey stop 
        if iteration >= max_iteration_limit:
            break

        previous_responsibilities = responsibilities.copy()

    
    plot(data, responsibilities, means, K, iteration, "Final")
    print(f"EM completed for K={K} in {iteration} iterations.")


if __name__ == "__main__":
    EM("ClusteringData.txt", K = 5)  # default K = 2 is used when K is not provided
