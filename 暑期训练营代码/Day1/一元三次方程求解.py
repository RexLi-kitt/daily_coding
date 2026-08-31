import math

def equation(a, b, c, d, x):
    return a * (x ** 3) + b * (x ** 2) + c * x + d

def derivative(a, b, c):
    a = 3 * a
    b = 2 * b
    c = c 
    delta = math.sqrt(b ** 2 - 4 *a *c)
    return [min((-b - delta) / (2 * a), (-b + delta) / (3 * a)),max((-b - delta) / (2 * a), (-b + delta) / (3 * a))]

def binary_search(a, b, c, d,x1,x2,f = equation):
    if f(a, b, c, d, x1) >= 0:
        x1, x2 = x2, x1
    while abs(x1 - x2) > 0.001: 
        x3 = (x1 + x2) / 2
        if(f(a, b, c, d, x3) <= 0):
            x1 = x3
        else:
            x2 = x3
    return x1
            
def solution(a, b, c, d,f = derivative, g = binary_search):
    x1 = 100.0
    x2 = -100.0
    sp = f(a, b, c)
    starter0 = sp[0]
    starter1 = sp[1]
    sol0 = g(a, b, c, d, x2, starter0)
    sol1 = g(a, b, c, d, x1, starter1)
    sol2 = g(a, b, c, d, starter0,starter1)
    return [sol0, sol2, sol1]



def main():
    a, b, c, d = map(float, input().split())
    ans = solution(a, b, c, d)
    print(f"{ans[0]:.2f} {ans[1]:.2f} {ans[2]:.2f}")

if __name__ == "__main__":
    main()
