# irisデータの読み込み
from sklearn.datasets import load_iris
dataset = load_iris()
x = dataset.data
t = dataset.target

# 訓練用データセットとテスト用データセットへの分割
from sklearn.model_selection import train_test_split
x_train, x_test, t_train, t_test = train_test_split(x, t, test_size=0.3, random_state=0)

# モデルの定義
from sklearn import svm
classifier = svm.SVC()

# モデルの訓練
classifier.fit(x_train, t_train)

# テスト用データセットの正解率
print(classifier.score(x_test, t_test))
