#Mohamad Nabih Alkhateeb
#ID: 1002170786
# 3/26/2025

""" Assignemnt 4: Baseball Winner Prediction - Decision Tree Classifier:

This assignment implements a Decision Tree model to predict the winner of baseball games using a given data set.

 # Missing Value Handling (Imputation)
- Numerical features: Missing values were imputed using the mean of each column.
- Categorical features: Missing values were imputed using the most frequent (mode) value in each column.

 # Decision Tree Configuration
- The model uses a **DecisionTreeClassifier** from scikit-learn.
- The **splitting criterion** chosen was Gini impurity
- The maximum depth of the tree was limited to 5 to prevent overfitting and ensure interpretability.

#  Output
- The model achieved a validation accuracy of approximately 86%.
- Predictions for the test dataset are saved in `test_predictions.csv`.
- A visual representation of the trained tree is saved as `decision_tree.png` (and optionally via Graphviz for higher clarity)."""


import pandas as pd
import numpy as np
from sklearn.impute import SimpleImputer
from sklearn.preprocessing import OneHotEncoder, StandardScaler
from sklearn.compose import ColumnTransformer
from sklearn.pipeline import Pipeline
from sklearn.tree import DecisionTreeClassifier, plot_tree
from sklearn.metrics import accuracy_score
import matplotlib.pyplot as plt


train_data = pd.read_csv('btrain.csv')
val_data = pd.read_csv('bvalidate.csv')
test_data = pd.read_csv('btest.csv')

# handling missing values
for df in [train_data, val_data, test_data]:
    df.replace("?", np.nan, inplace=True)  # replace ? with nan
    df.columns = df.columns.str.strip() \
                           .str.replace('\n', ' ', regex=False) \
                           .str.replace('\u2019', "'", regex=False) \
                           .str.replace('\u2018', "'", regex=False) \
                           .str.replace('’', "'", regex=False) \
                           .str.replace('‘', "'", regex=False)

print("Cleaned column names:")
print(train_data.columns.tolist())

# separating features from the target label "winner"
target_label = 'winner'
feature_columns = [col for col in train_data.columns if col != target_label]

# identifying numerical vs categorical features
numeric_features = train_data[feature_columns].select_dtypes(include=['float64', 'int64']).columns.tolist()
categorical_features = [col for col in feature_columns if col not in numeric_features]

# numeric feature pipeline
numeric_processing_pipeline = Pipeline([
    ('imputer', SimpleImputer(strategy='mean')),
    ('scaler', StandardScaler())
])

# categorical feature pipeline
categorical_processing_pipeline = Pipeline([
    ('imputer', SimpleImputer(strategy='most_frequent')),
    ('encoder', OneHotEncoder(handle_unknown='ignore'))
])

# preprocessing pipeline
data_preprocessor = ColumnTransformer([
    ('num', numeric_processing_pipeline, numeric_features),
    ('cat', categorical_processing_pipeline, categorical_features)
])

# defining the full pipeline with DecisionTreeClassifier
game_prediction_model = Pipeline([
    ('preprocessor', data_preprocessor),
    ('classifier', DecisionTreeClassifier(criterion='gini', max_depth=5))
])

# removing rows where data is missing in the target label
train_data = train_data.dropna(subset=[target_label])
val_data = val_data.dropna(subset=[target_label])

# extracting features and labels for training and validation
X_train = train_data.drop(target_label, axis=1)
y_train = train_data[target_label].astype(int)

X_val = val_data.drop(target_label, axis=1)
y_val = val_data[target_label].astype(int)

X_test = test_data.drop(target_label, axis=1)

# train the model
game_prediction_model.fit(X_train, y_train)

# evaluate the accuracy
y_val_pred = game_prediction_model.predict(X_val)
val_accuracy = accuracy_score(y_val, y_val_pred)
print(f"\nValidation Accuracy: {val_accuracy:.4f}")

# predict on the test dataset
y_test_pred = game_prediction_model.predict(X_test)

# save predictions to a CSV file
test_results = test_data.copy()
test_results[target_label] = y_test_pred
test_results.to_csv('test_predictions.csv', index=False)
print("Test predictions saved to 'test_predictions.csv'.")

# plotting the decision tree
plt.figure(figsize=(40, 20))
plot_tree(
    game_prediction_model.named_steps['classifier'],
    feature_names=game_prediction_model.named_steps['preprocessor'].get_feature_names_out(),
    class_names=["0", "1"],
    filled=True,
    rounded=True,
    fontsize=5
)
plt.title("Decision Tree - Baseball Game Winner Prediction", fontsize=18)
plt.savefig("decision_tree.png", dpi=400)
plt.show()

print("Decision tree saved as 'decision_tree.png'.")