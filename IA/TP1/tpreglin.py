import matplotlib.pyplot as plt
import numpy as np


def compute_errors(m):
    d = 5
    x = np.random.rand(m, d) * 10 - 5
    # x[:,4] = x[:,3]*2 Question 6
    w = np.array([1.2, -1.3, 0.5, 0.8, -2.3])
    v = np.random.randn(m) * -1
    y = x @ w + v
    w2 = np.linalg.inv(x.T @ x) @ x.T @ y
    return np.linalg.norm(w - w2)


def compute_errors_lstsq(m):
    d = 5
    x = np.random.rand(m, d) * 10 - 5
    w = np.array([1.2, -1.3, 0.5, 0.8, -2.3])
    v = np.random.randn(m) * -1
    y = x @ w + v
    res = np.linalg.lstsq(x, y, rcond=None)
    return np.linalg.norm(w - res[0])


def compute_empirical_gap(m):
    d = 5
    x = np.random.rand(m, d) * 10 - 5
    w = np.array([1.2, -1.3, 0.5, 0.8, -2.3])
    v = np.random.randn(m) * -1
    y = x @ w + v
    w2 = np.linalg.inv(x.T @ x) @ x.T @ y
    return np.mean((y - x @ w2) ** 2)


def show_errors():
    plt.figure()

    errors = []
    errors_lstsq = []
    errors_empirical = []
    m = [i for i in range(5, 1000, 10)]

    for i in m:
        errors.append(compute_errors(i))
        errors_lstsq.append(compute_errors_lstsq(i))
        errors_empirical.append(compute_empirical_gap(i))

    plt.subplot(311)
    plt.plot(m, errors)
    plt.legend(["|| w* - w^ ||"])

    plt.subplot(312)
    plt.plot(m, errors_lstsq)
    plt.legend(["lstsq"])

    plt.subplot(313)
    plt.plot(m, errors_empirical)
    plt.legend(["empirical"])

    plt.show()


show_errors()
