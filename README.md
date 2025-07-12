# 洛克王国钟表校准控制方案计算器

一个用于计算“奇怪表盘”从初始时间到目标时间最少按按钮次数的工具，支持 C 语言、HTML 网页两种版本。
使用方法参考该文档。


## 项目背景

有一个特殊表盘，分布着 1~12 数字，无法自主转动，需通过三个按钮控制指针：
- **按钮A**：时针和分针同时顺时针转动到下一个数字（12的下一个是1）
- **按钮B**：分针和秒针同时顺时针转动到下一个数字
- **按钮C**：秒针和时针同时顺时针转动到下一个数字

给定初始时间（H₀:M₀:S₀）和目标时间（H₁:M₁:S₁），需计算最少按按钮次数的方案。


## 数学原理

### 1. 问题建模
- 设时针、分针、秒针的位置为 1~12 的整数，转动规则为“模12运算”（即 12+1=1，等价于 0+1=1 mod 12）。
- 设按钮A、B、C的按压次数为 `a、b、c`（非负整数），其对指针的影响如下：
  - 时针总转动次数：`a + c`（A和C共同作用）
  - 分针总转动次数：`a + b`（A和B共同作用）
  - 秒针总转动次数：`b + c`（B和C共同作用）


### 2. 方程组建立
设初始到目标的指针转动净次数为：
- 时针：`h = (H₁ - H₀ + 12) % 12`（确保结果为 0~11，避免负数）
- 分针：`m = (M₁ - M₀ + 12) % 12`
- 秒针：`s = (S₁ - S₀ + 12) % 12`

需满足以下同余方程组：
```
a + c ≡ h (mod 12)
a + b ≡ m (mod 12)
b + c ≡ s (mod 12)
```


### 3. 求解步骤
1. **可行性判断**：方程组有解的必要条件是 `h + m + s` 为偶数（因三式相加得 `2(a+b+c) ≡ h+m+s (mod 12)`，左边为偶数）。
2. **基础解计算**：通过消元法解得：
   ```
   a₀ = (h + m - s) / 2
   b₀ = (m + s - h) / 2
   c₀ = (h + s - m) / 2
   ```
   （注：`a₀、b₀、c₀` 可能为负数或非整数）
3. **最优解筛选**：由于指针转动具有 12 周期性，通过遍历调整值 `k=0~11`，将基础解修正为非负整数 `(a₀+k, b₀+k, c₀+k)`，取总次数 `a+b+c` 最小的解。


## 代码实现

### 1. C语言版本（推荐，效率最高）

```c
#include <stdio.h>
#include <limits.h>

int main() {
    int H0, M0, S0; // 初始时间：时、分、秒
    int H1, M1, S1; // 目标时间：时、分、秒
    
    // 输入初始和目标时间（1~12）
    printf("请输入初始时间（时 分 秒，空格分隔，1-12）：");
    scanf("%d %d %d", &H0, &M0, &S0);
    printf("请输入目标时间（时 分 秒，空格分隔，1-12）：");
    scanf("%d %d %d", &H1, &M1, &S1);
    
    // 计算转动净次数（模12，确保非负）
    int h = (H1 - H0 + 12) % 12;
    int m = (M1 - M0 + 12) % 12;
    int s = (S1 - S0 + 12) % 12;
    
    // 检查是否有解
    if ((h + m + s) % 2 != 0) {
        printf("无解，无法从初始时间到达目标时间。\n");
        return 0;
    }
    
    // 计算基础解
    int a0 = (h + m - s) / 2;
    int b0 = (m + s - h) / 2;
    int c0 = (h + s - m) / 2;
    
    // 寻找最小总次数的非负解
    int min_total = INT_MAX;
    int best_a, best_b, best_c;
    for (int k = 0; k < 12; k++) {
        int a = a0 + k;
        int b = b0 + k;
        int c = c0 + k;
        if (a >= 0 && b >= 0 && c >= 0) {
            int total = a + b + c;
            if (total < min_total) {
                min_total = total;
                best_a = a;
                best_b = b;
                best_c = c;
            }
        }
    }
    
    // 输出结果
    if (min_total != INT_MAX) {
        printf("最少按按钮次数：%d\n", min_total);
        printf("方案：\n");
        printf("按钮A（时针+分针）按 %d 次\n", best_a);
        printf("按钮B（分针+秒针）按 %d 次\n", best_b);
        printf("按钮C（秒针+时针）按 %d 次\n", best_c);
    } else {
        printf("无解，无法从初始时间到达目标时间。\n");
    }
    
    return 0;
}
```


### 2. HTML 网页版本（无需编译，直接在浏览器运行）

```html
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>表盘按钮计算器</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; }
        .input-group { margin: 10px 0; }
        input { width: 50px; margin: 0 5px; }
        #result { margin: 20px 0; padding: 10px; border: 1px solid #ccc; }
    </style>
</head>
<body>
    <h1>表盘按钮控制方案计算器</h1>
    
    <div class="input-group">
        初始时间：
        <input type="number" id="h0" min="1" max="12" value="1"> 时针数字
        <input type="number" id="m0" min="1" max="12" value="2"> 分针数字
        <input type="number" id="s0" min="1" max="12" value="3"> 秒针数字
    </div>
    
    <div class="input-group">
        目标时间：
        <input type="number" id="h1" min="1" max="12" value="5"> 时针数字
        <input type="number" id="m1" min="1" max="12" value="8"> 分针数字
        <input type="number" id="s1" min="1" max="12" value="7"> 秒针数字
    </div>
    
    <button onclick="calculate()">计算方案</button>
    <div id="result"></div>

    <script>
        function calculate() {
            // 获取输入值
            const H0 = parseInt(document.getElementById('h0').value);
            const M0 = parseInt(document.getElementById('m0').value);
            const S0 = parseInt(document.getElementById('s0').value);
            const H1 = parseInt(document.getElementById('h1').value);
            const M1 = parseInt(document.getElementById('m1').value);
            const S1 = parseInt(document.getElementById('s1').value);
            
            // 计算转动净次数（模12）
            const h = (H1 - H0 + 12) % 12;
            const m = (M1 - M0 + 12) % 12;
            const s = (S1 - S0 + 12) % 12;
            
            // 检查是否有解
            if ((h + m + s) % 2 !== 0) {
                document.getElementById('result').innerText = "无解，无法从初始时间到达目标时间。";
                return;
            }
            
            // 计算基础解
            const a0 = (h + m - s) / 2;
            const b0 = (m + s - h) / 2;
            const c0 = (h + s - m) / 2;
            
            // 寻找最小总次数的非负解
            let minTotal = Infinity;
            let bestA, bestB, bestC;
            for (let k = 0; k < 12; k++) {
                const a = a0 + k;
                const b = b0 + k;
                const c = c0 + k;
                if (a >= 0 && b >= 0 && c >= 0) {
                    const total = a + b + c;
                    if (total < minTotal) {
                        minTotal = total;
                        bestA = a;
                        bestB = b;
                        bestC = c;
                    }
                }
            }
            
            // 输出结果
            if (minTotal !== Infinity) {
                document.getElementById('result').innerText = 
                    `最少按按钮次数：${minTotal}\n` +
                    `方案：\n` +
                    `按钮A（时针+分针）按 ${bestA} 次\n` +
                    `按钮B（分针+秒针）按 ${bestB} 次\n` +
                    `按钮C（秒针+时针）按 ${bestC} 次`;
            } else {
                document.getElementById('result').innerText = "无解，无法从初始时间到达目标时间。";
            }
        }
    </script>
</body>
</html>
```


## 使用教程

### C语言版本
1. 编译：使用 gcc 或其他编译器编译 `main.c`（如 `gcc main.c -o clock_calculator`）
2. 运行：执行编译后的可执行文件，按提示输入初始和目标时间（1-12），即可得到方案。

### HTML版本
1. 将代码保存为 `index.html`
2. 用浏览器打开该文件，输入时间后点击“计算方案”按钮即可。


## 示例
- 初始时间：1:2:3
- 目标时间：5:8:7
- 输出结果：
  ```
  最少按按钮次数：7
  方案：
  按钮A（时针+分针）按 3 次
  按钮B（分针+秒针）按 3 次
  按钮C（秒针+时针）按 1 次
  ```


## 许可证
本项目采用 MIT 许可证，可自由使用和修改。
