#ifndef PIVOTER_H
#define PIVOTER_H
#include<QVector>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <QDebug>
extern double comb[2000][2000];

//边，图的定义
struct Edge {
    int src, dst, w, nxt;
};
struct Graph {
public:
    int e_num, v_num;
    int e_cnt;
    int *first;
    Edge *e;

    int *in_deg, *out_deg;
    int max_in_deg, max_out_deg;
};
//清空图内容
static void clear_graph(Graph *g) {
    g->e_cnt = g->max_in_deg = g->max_out_deg = 0;
    std::memset(g->first, -1, sizeof(int) * (g->v_num + 1));
    memset(g->in_deg, 0, sizeof(int) * (g->v_num + 1));
    memset(g->out_deg, 0, sizeof(int) * (g->v_num + 1));
}
//初始化图，分配大小，内容清空
static Graph *init_graph(int vertex_num, int edge_num) {
    Graph *g = (Graph *)malloc(sizeof(Graph));
    g->e_num = edge_num;
    g->v_num = vertex_num;
    g->first = (int *)malloc(sizeof(int) * (vertex_num + 1));
    g->e = (Edge *)malloc(sizeof(Edge) * (edge_num + 10));
    g->in_deg = (int *)malloc(sizeof(int) * (vertex_num + 1));
    g->out_deg = (int *)malloc(sizeof(int) * (vertex_num + 1));
    clear_graph(g);
    return g;
}
//释放图
static void free_graph(Graph *g) {
    free(g->first);
    free(g->e);
    free(g->in_deg);
    free(g->out_deg);
    free(g);
}
//添加边（带权值）
static void add_edge(Graph *g, int src, int dst, int w) {
    Edge e={src, dst, w, g->first[src]};
    g->e[++g->e_cnt] = e;
    g->first[src] = g->e_cnt;
    g->in_deg[dst]++;
    g->out_deg[src]++;
    g->max_in_deg = std::max(g->in_deg[dst], g->max_in_deg);
    g->max_out_deg = std::max(g->out_deg[src], g->max_out_deg);
}
//添加无权边
static void add_edge_no_weight(Graph *g, int src, int dst) {
    add_edge(g, src, dst, 0);
}


class Pivoter
{
public:
    Pivoter(){};
    static void S_comb_init(int n);
    static Graph *S_core_decomposition(Graph *g);
    static void S_init(int *S, int *S_p, int vertex_num);
    static void S_insert(int *S, int *S_p, int vertex);
    static void S_delete(int *S, int *S_p, int vertex);

    static void pivoter_dfs(int *S, int *S_p, int **S_nbr, double *C, int c_cnt, int p_cnt);
    static void pivoter(Graph *g, double *C);
};

#endif // PIVOTER_H


