from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
from sklearn.neural_network import MLPClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import SGDClassifier
import pandas as pd

# データの読み込み
dataset = load_iris()
x = dataset.data
t = dataset.target

# 訓練用データセットとテスト用データセットへの分割
x_train, x_test, t_train, t_test = train_test_split(x, t, test_size=0.3, random_state=0)

# モデルの定義
models = {
    "Support Vector Machine": SVC(),
    "Neural Network": MLPClassifier(max_iter=1000),
    "K-Nearest Neighbors": KNeighborsClassifier(),
    "Gaussian Naive Bayes": GaussianNB(),
    "Random Forest": RandomForestClassifier(),
    "Stochastic Gradient Descent": SGDClassifier()
}

# モデルの訓練とテスト
results = {}
for name, model in models.items():
    model.fit(x_train, t_train)
    score = model.score(x_test, t_test)
    results[name] = score

# 結果をカンマ区切りで表示
print("Model,Accuracy")
for name, accuracy in results.items():
    print(f"{name},{accuracy}")
