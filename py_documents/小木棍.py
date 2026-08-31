
n = int(input())    
line = list(map(int, input().split()))
kind = sorted(set(line),reverse=True)
bucket = [0] * 55
asum = sum(line)
for i in line:
    bucket[i] += 1    
possible_group_num = [i for i in range(kind[0],asum + 1) if asum % i == 0]


def dfs(group_len,bucket,cur_len,kind, start, remain):
    #jianzhi 3
    if cur_len + remain < group_len:
            return False
    if(cur_len == group_len):
        if remain == 0:
            return True
        return dfs(group_len,bucket,0,kind,0,remain)

    for i in range(start, len(kind)):
        k = kind[i]

        if bucket[k] == 0:
            continue

        if cur_len + k > group_len:
            continue
        bucket[k] -= 1
        if dfs(
            group_len,
            bucket,
            cur_len + k,
            kind,
            i,              # 下一根不能比当前木棍更大
            remain - k
        ):
            return True
        bucket[k] += 1
        # 第一个剪枝
        if cur_len == 0:
            return False
        # 第二个剪枝
        if cur_len + k == group_len:
            return False



    return False

for i in possible_group_num:
    if dfs(i, bucket, 0, kind, 0, asum):
        print(i)
        break
