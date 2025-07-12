#include <stdio.h>
#include <limits.h> // 用于INT_MAX定义

int main() {
    int H0, M0, S0; // 初始时间：时、分、秒
    int H1, M1, S1; // 目标时间：时、分、秒
    
    // 输入初始和目标时间（1~12）
    printf("请输入初始数值（时针 分针 秒针，空格分隔，1-12）：");
    scanf("%d %d %d", &H0, &M0, &S0);
    printf("请输入目标时间（时针 分针 秒针，空格分隔，1-12）：");
    scanf("%d %d %d", &H1, &M1, &S1);
    
    // 计算每个指针需要转动的净次数（模12，确保结果为0~11）
    // +12再模12是为了避免负数（例如1-3的结果会转为10）
    int h = (H1 - H0 + 12) % 12;  // 时针需要的总转动次数
    int m = (M1 - M0 + 12) % 12;  // 分针需要的总转动次数
    int s = (S1 - S0 + 12) % 12;  // 秒针需要的总转动次数
    
    // 检查是否有解：h + m + s必须为偶数（因三个方程相加后左边为2*(a+b+c)）
    if ((h + m + s) % 2 != 0) {
        printf("无解，无法从初始时间到达目标时间。\n");
        return 0;
    }
    
    // 计算基础解（可能为负数，需后续调整）
    int a0 = (h + m - s) / 2;  // 按钮A的基础次数
    int b0 = (m + s - h) / 2;  // 按钮B的基础次数
    int c0 = (h + s - m) / 2;  // 按钮C的基础次数
    
    // 寻找最小总次数及对应的a、b、c（非负整数）
    int min_total = INT_MAX;   // 最小总次数（初始化为极大值）
    int best_a, best_b, best_c; // 最优方案的按钮次数
    
    // 遍历可能的调整值k（因模12周期性，k范围0~11即可覆盖所有情况）
    // 调整原理：给a0、b0、c0同时加k，可抵消负数并保持方程成立
    for (int k = 0; k < 12; k++) {
        int a = a0 + k;
        int b = b0 + k;
        int c = c0 + k;
        
        // 确保a、b、c均为非负整数（按钮次数不能为负）
        if (a >= 0 && b >= 0 && c >= 0) {
            int total = a + b + c;
            // 更新最小总次数及对应方案
            if (total < min_total) {
                min_total = total;
                best_a = a;
                best_b = b;
                best_c = c;
            }
        }
    }
    
    // 输出最优方案
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
