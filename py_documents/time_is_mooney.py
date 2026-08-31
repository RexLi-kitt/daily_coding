n, m, c = map(int, input().split())

# 1-based：w[1] 对应城市 1
w = [0] + list(map(int, input().split()))

graph = [[] for _ in range(n + 1)]

for _ in range(m):
    u, v = map(int, input().split())
    graph[u].append(v)

MAX_DAYS = 1000
NEG_INF = -10**18

dp = [NEG_INF] * (n + 1)
dp[1] = 0

ans = 0

for day in range(1, MAX_DAYS + 1):
    next_dp = [NEG_INF] * (n + 1)

    for u in range(1, n + 1):
        if dp[u] == NEG_INF:
            continue

        for v in graph[u]:
            next_dp[v] = max(
                next_dp[v],
                dp[u] + w[v]
            )

    dp = next_dp

    # 回到城市 1 时，计算收益
    if dp[1] != NEG_INF:
        money = dp[1] - c * day * day
        ans = max(ans, money)

print(ans)
