import numpy as np

from src.excelHandling import ExcelHandling
from scipy.optimize import minimize
import matplotlib.pyplot as plt

file = 'source/АВТ_2.xlsx'
EHandling = ExcelHandling(file)
y = EHandling.getDependenceValuesByNameAndIndex(
    [
        {"nameColumn": "Unnamed: 6", "indexRow": 2},
        {"nameColumn": "Unnamed: 6", "indexRow": 12},
        {"nameColumn": "Unnamed: 6", "indexRow": 17},
        {"nameColumn": "Unnamed: 6", "indexRow": 18},
        {"nameColumn": "Unnamed: 6", "indexRow": 19},
        {"nameColumn": "Unnamed: 6", "indexRow": 20},
        {"nameColumn": "Unnamed: 6", "indexRow": 21},
    ], False)[0]
x0 = y.copy()


def value(x):
    sum = 0
    for i in range(len(x)):
        if y[i] == 0 :
            continue
        sum += ((y[i] - x[i]) / y[i]) ** 2
    return sum


cons = {'type': 'eq',
        'fun': lambda x:
        np.array(x[0]  - x[1] - x[2] - x[3] - x[4] - x[5] - x[6])
        }

res = minimize(value, x0, method='SLSQP', constraints=cons, options={'maxiter': 1000})
np.set_printoptions(precision=3)
print('N измеренные согласованные коррекция')
for i in range(len(y)):
    print('{} {:>7.3f} {:12.3f} {:9.3f}'.format(i + 1, y[i],
                                                res.x[i], res.x[i] - y[i]))
x = np.arange(0, len(y), 1)
plt.plot(x, res.x, 'o', c='g', label='сoгласов')
plt.plot(x, y, 'x', label='измеренные')
plt.grid(axis='y')
plt.legend(loc='lower right')
plt.title('согласование данных')
plt.ylabel('значение')
plt.show()
