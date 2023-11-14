
import random
import os
import sys

def matrixChainMemoised(p, i, j, dp):
    if(i == j):
        return 0
     
    if(dp[i][j] != -1):
        return dp[i][j]
     
    dp[i][j] = sys.maxsize
     
    for k in range(i,j):
        dp[i][j] = min(dp[i][j], matrixChainMemoised(p, i, k, dp) + matrixChainMemoised(p, k + 1, j, dp)+ p[i - 1] * p[k] * p[j])

    return dp[i][j]

def MatrixChainOrder(p):
    n = len(p)
    i = 0
    j = n - 1
    dp = [[-1 for q in range(n)] for m in range(n)]
    return matrixChainMemoised(p, i, j, dp)

def genDims(n):
    return [random.randint(2, 100) for _ in range(n)]

def generate_test_files():
    directory = "../input"
    answers_directory = "../answers"
    if not os.path.exists(directory):
        os.makedirs(directory)
    if not os.path.exists(answers_directory):
        os.makedirs(answers_directory)

    for i in range(2, 10):
        file_path = os.path.join(directory, f"test_{i - 1}.dat")
        with open(file_path, "w") as file:
            size = 2 ** i
            dims = genDims(size)
            file.write(f"{size} ")
            file.write(" ".join([str(x) for x in dims]))

        result = MatrixChainOrder(dims)
        output_file_path = os.path.join(answers_directory, f"test_{i - 1}_ans.dat")
        with open(output_file_path, "w") as file:
            file.write(f"{result}")

generate_test_files()