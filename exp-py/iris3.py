from sklearn.datasets import fetch_olivetti_faces
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
from sklearn.neural_network import MLPClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import SGDClassifier
from sklearn.model_selection import GridSearchCV

# Olivetti Facesデータセットの読み込み
dataset = fetch_olivetti_faces()
x = dataset.data
t = dataset.target
x_train, x_test, t_train, t_test = train_test_split(x, t, test_size=0.3, random_state=0)

# モデルの定義とパラメータグリッドの設定
param_grids = {
    "Support Vector Machine": {
        "model": SVC(),
        "params": {"kernel": ["linear", "poly", "rbf", "sigmoid"]}
    },
    "Neural Network": {
        "model": MLPClassifier(max_iter=1000),
        "params": {"hidden_layer_sizes": [(50,), (100,), (150,)]}
    },
    "K-Nearest Neighbors": {
        "model": KNeighborsClassifier(),
        "params": {"n_neighbors": [3, 5, 7, 9]}
    },
    "Random Forest": {
        "model": RandomForestClassifier(),
        "params": {"n_estimators": [50, 100, 150]}
    },
    "Stochastic Gradient Descent": {
        "model": SGDClassifier(),
        "params": {"penalty": ["l2", "l1", "elasticnet"]}
    }
}

# 結果を保持する辞書
results = {}

# グリッドサーチによる最適化
for model_name, config in param_grids.items():
    model = config["model"]
    params = config["params"]
    grid_search = GridSearchCV(model, params, cv=5, scoring='accuracy')
    grid_search.fit(x_train, t_train)
    best_params = grid_search.best_params_
    best_score = grid_search.best_score_

    # テストセットでのスコアを計算
    best_model = grid_search.best_estimator_
    test_score = best_model.score(x_test, t_test)

    # 結果を保存
    results[model_name] = {
        "Best Params": best_params,
        "Test Accuracy": test_score
    }

# 結果をカンマ区切りで表示
print("Model,Best Params,Test Accuracy")
for model_name, result in results.items():
    best_params = ",".join([f"{k}={v}" for k, v in result["Best Params"].items()])
    test_accuracy = result["Test Accuracy"]
    print(f"{model_name},{best_params},{test_accuracy:.6f}")
