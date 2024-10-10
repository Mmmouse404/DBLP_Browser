#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pivoter.h"
double comb[2000][2000];

void Pivoter::S_comb_init(int n) {
    for (int i = 0; i <= n; ++i)
        comb[i][0] = 1.;

    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= i; ++j) {
            comb[i][j] = comb[i - 1][j - 1] + comb[i - 1][j];
        }
}

Graph * Pivoter::S_core_decomposition(Graph *g) {
    // 重新构成图
    Graph *d = init_graph(g->v_num, g->e_num >> 1);

    int *bin = (int *)calloc(g->max_in_deg + 2, sizeof(int));  // 存储度数的计数数组
    int *rank = (int *)calloc(g->v_num + 1, sizeof(int));  // 存储节点的排名
    int *rank_pos = (int *)calloc(g->v_num + 1, sizeof(int));  // 存储排名对应的节点
    int *deg = (int *)calloc(g->v_num + 1, sizeof(int));  // 存储节点的度数
    memcpy(deg, g->in_deg, sizeof(int) * (g->v_num + 1));  // 复制输入图的度数信息

    // 计算每个度数出现的次数
    for (int i = 1; i <= g->v_num; ++i)
        bin[deg[i] + 1]++;

    // 累加每个度数出现的次数
    for (int i = 1; i <= g->max_in_deg; ++i) {
        bin[i] += bin[i - 1];
    }

    // 根据排名重构图
    for (int i = 1; i <= g->v_num; ++i) {
        rank[i] = ++bin[deg[i]];
        rank_pos[rank[i]] = i;
    }

    // 基于度的排名重构图
    for (int i = 1; i <= g->v_num; ++i) {
        int u = rank_pos[i];
        for (int j = g->first[u]; ~j; j = g->e[j].nxt) {
            int v = g->e[j].dst;
            if (deg[v] <= deg[u]) continue;
            int w = rank_pos[++bin[--deg[v]]];
            rank[w] = rank[v];
            rank[v] = bin[deg[v]];
            rank_pos[rank[v]] = v;
            rank_pos[rank[w]] = w;
        }
    }

    // 重构新图
    for (int u = 1; u <= g->v_num; ++u)
        for (int i = g->first[u]; ~i; i = g->e[i].nxt) {
            int v = g->e[i].dst;
            if (deg[u] < deg[v] || (deg[u] == deg[v] && u < v))
                add_edge_no_weight(d, u, v);  // 添加边
        }

    free(bin);
    free(rank);
    free(rank_pos);
    free(deg);

    return d;  // 返回重构后的图
}

void Pivoter::S_init(int *S, int *S_p, int vertex_num) {
    // 初始化S和S_p数组，使得S中存储[1, vertex_num]，S_p中存储[1, vertex_num]
    for (int i = 1; i <= vertex_num; ++i) {
        S[i] = i;
        S_p[i] = i;
    }
}

void Pivoter::S_insert(int *S, int *S_p, int vertex) {
    // 插入节点到S中，并更新S_p
    int v_p = S_p[vertex];
    if (v_p <= S[0]) return;
    S[v_p] = S[++S[0]];
    S[S[0]] = vertex;
    S_p[vertex] = S[0];
    S_p[S[v_p]] = v_p;
}

void Pivoter::S_delete(int *S, int *S_p, int vertex) {
    // 从S中删除节点，并更新S_p
    int v_p = S_p[vertex];
    if (v_p > S[0]) return;
    S[v_p] = S[S[0]];
    S[S[0]] = vertex;
    S_p[vertex] = S[0]--;
    S_p[S[v_p]] = v_p;
}

void Pivoter::pivoter_dfs(int *S, int *S_p, int **S_nbr, double *C, int c_cnt, int p_cnt) {
    // 如果S为空，更新C值
    if (!S[0]) {
        for (int i = 0; i <= p_cnt; ++i)
            C[i + (c_cnt - p_cnt)] += comb[p_cnt][i];
        return;
    }

    // 选择一个pivot
    int pivot = 0, max_nbr_cnt = -1;
    for (int i = 1; i <= S[0]; ++i) {
        int u = S[i], nbr_cnt = 0;
        for (int j = 1; j <= S_nbr[u][0]; ++j) {
            int v = S_nbr[u][j];
            if (S_p[v] <= S[0])
                ++nbr_cnt;
            else
                break;
        }
        if (nbr_cnt > max_nbr_cnt) {
            max_nbr_cnt = nbr_cnt;
            pivot = u;
        }
    }

    int *cand = (int *)calloc(S[0] + 1, sizeof(int));
    memcpy(cand, S, sizeof(int) * (S[0] + 1));

    // 筛选候选节点
    for (int i = 1; i <= S_nbr[pivot][0]; ++i) {
        int u = S_nbr[pivot][i];
        if (S_p[u] <= S[0])
            cand[S_p[u]] = -1;
        else
            break;
    }

    cand[0] = 0;
    for (int i = 1; i <= S[0]; ++i) {
        if (cand[i] == -1) continue;
        cand[++cand[0]] = cand[i];
    }

    int S0_bak = S[0];

    // 递归调用进行搜索
    for (int i = 1; i <= cand[0]; ++i) {
        int u = cand[i], S_sz = S[0];
        S[0] = 0;

        for (int j = 1; j <= S_nbr[u][0]; ++j) {
            int v = S_nbr[u][j];
            if (S_p[v] <= S_sz)
                S_insert(S, S_p, v);
            else if (S_p[v] > S0_bak)
                break;
        }

        // 更新邻居节点
        for (int j = 1; j <= S[0]; ++j) {
            int v = S[j], cnt = 0;
            for (int k = 1; k <= S_nbr[v][0]; ++k) {
                // 重新排序邻居节点数组
                int v_nbr = S_nbr[v][k];
                if (S_p[v_nbr] <= S[0]) {
                    S_nbr[v][k] = S_nbr[v][++cnt];
                    S_nbr[v][cnt] = v_nbr;
                }
                if (S_p[v_nbr] > S0_bak)
                    break;
            }
        }

        pivoter_dfs(S, S_p, S_nbr, C, c_cnt + 1, p_cnt + (int)(u == pivot));  // 递归调用
        S[0] = S_sz;

        S_delete(S, S_p, u);
    }

    S[0] = S0_bak;

    free(cand);
}

// 主要算法
void Pivoter::pivoter(Graph *g, double *C) {
    S_comb_init(1500);  // 初始化组合数

    std::memset(C, 0, sizeof(int) * (g->v_num + 1));  // 清空C数组

    Graph *d = S_core_decomposition(g);  // 执行核分解
    int *S = (int *)calloc(g->v_num + 1, sizeof(int));  // 存储节点
    int *S_p = (int *)calloc(g->v_num + 1, sizeof(int));  // 存储节点位置
    int **S_nbr = (int **)calloc(g->v_num + 1, sizeof(int *));  // 存储邻居节点信息

    S_init(S, S_p, g->v_num);  // 初始化S和S_p
    for (int u = 1; u <= g->v_num; ++u)
        S_nbr[u] = NULL;

    for (int u = 1; u <= g->v_num; ++u) {
        // 构建邻居节点数组
        S[0] = 0;
        for (int i = d->first[u]; ~i; i = d->e[i].nxt) {
            int v = d->e[i].dst;
            S_insert(S, S_p, v);
        }

        for (int i = 1; i <= S[0]; ++i) {
            int u = S[i];
            // 初始化邻居节点数组
            S_nbr[u] = (int *)realloc(S_nbr[u], sizeof(int) * (std::min(S[0], g->in_deg[u]) + 1));
            S_nbr[u][0] = 0;
        }

        for (int i = 1; i <= S[0]; ++i) {
            int u = S[i];
            for (int j = d->first[u]; ~j; j = d->e[j].nxt) {
                int v = d->e[j].dst;
                if (S_p[v] <= S[0]) {
                    S_nbr[u][++S_nbr[u][0]] = v;
                    S_nbr[v][++S_nbr[v][0]] = u;
                }
            }
        }

        pivoter_dfs(S, S_p, S_nbr, C, 1, 0);  // 执行深度优先搜索
    }

    for (int v = 1; v <= g->v_num; ++v)
        if (S_nbr[v] != NULL)
            free(S_nbr[v]);
    free(S);
    free(S_p);
    free(S_nbr);
}
