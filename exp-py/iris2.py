from sklearn.datasets import load_iris, load_digits, load_wine, load_breast_cancer, fetch_olivetti_faces
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
from sklearn.neural_network import MLPClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import SGDClassifier

# データセットの読み込み関数
def load_datasets():
    datasets = {
        "Iris": load_iris(),
        "Digits": load_digits(),
        "Wine": load_wine(),
        "Breast Cancer": load_breast_cancer(),
        "Olivetti Faces": fetch_olivetti_faces()
    }
    return datasets

# モデルの定義
models = {
    "Support Vector Machine": SVC(),
    "Neural Network": MLPClassifier(max_iter=1000),
    "K-Nearest Neighbors": KNeighborsClassifier(),
    "Gaussian Naive Bayes": GaussianNB(),
    "Random Forest": RandomForestClassifier(),
    "Stochastic Gradient Descent": SGDClassifier()
}

# データセットの読み込み
datasets = load_datasets()

# 結果を保持する辞書
results = {dataset_name: {} for dataset_name in datasets}

# 各データセットに対してモデルを訓練し、精度を測定
for dataset_name, dataset in datasets.items():
    x = dataset.data
    t = dataset.target
    x_train, x_test, t_train, t_test = train_test_split(x, t, test_size=0.3, random_state=0)

    for model_name, model in models.items():
        model.fit(x_train, t_train)
        score = model.score(x_test, t_test)
        results[dataset_name][model_name] = score

# 結果をカンマ区切りで表示
print("Dataset,Model,Accuracy")
for dataset_name, model_scores in results.items():
    for model_name, accuracy in model_scores.items():
        print(f"{dataset_name},{model_name},{accuracy:.6f}")
