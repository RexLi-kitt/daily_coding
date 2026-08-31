n = int(input())
g = [[0] * (n + 1) for _ in range(n + 1)]

while True:
    x, y, v = map(int, input().split())
    if x == 0 and y == 0:
        break
    g[x][y] = v

def dfs(x, y, memo):
    if x > n or y > n:
        return []
    if x == n and y == n:
        return memo
    l1 = dfs(x + 1, y, memo + [(x, y)])
    l2 = dfs(x, y + 1, memo + [(x, y)])
    v1 = sum(g[u][v] for u, v in l1)
    v2 = sum(g[u][v] for u, v in l2)
    return l1 if v1 > v2 else l2

list1 = dfs(1, 1, [])
sum1 = sum(g[u][v] for u, v in list1)
for u, v in list1:
    g[u][v] = 0
list2 = dfs(1, 1, [])
sum2 = sum(g[u][v] for u, v in list2)
print(sum1 + sum2)
