n = int(input())    
bucket = [0] * 2005
def dfs(u, t, kinds, bucket):
    if u == t:                                   # 一条边凑满
        if all(bucket[i] == 0 for i in range(2001)):
            return True                          # 木棒全用完 → 成功
        return dfs(0, t, kinds, bucket)          # 还差边，重新开始凑
    for k in kinds:                              # 只枚举存在的木棒长度（≤20种）
        if k > t - u:
            continue                               # 升序，超容量就停
        if bucket[k] > 0:
            bucket[k] -= 1
            if dfs(u + k, t, kinds, bucket):
                return True
            bucket[k] += 1                       # 失败恢复
    return False
        
for _ in range(n):
    line = list(map(int, input().split()))
    m = line[0]
    length = line[1:]
    total = sum(length)
    if total % 4 != 0:
        print('no'); continue
    t = total // 4
    bucket = [0] * 2001
    for x in length:
        bucket[x] += 1
    kinds = sorted(set(length),reverse=True)
    print('yes' if dfs(0, t, kinds, bucket) else 'no')
        

