#include "../template.hpp"

struct Dinic {
    
    struct Edge {
        int u, v;
        LL cap, flow;
    };
    
    vector<Edge> edges;
    vector<vector<int> > adjlist;
    vector<int> d, p;
    
    Dinic(int N) {
        adjlist.resize(N);
        d.resize(N);
        p.resize(N);
    }
    
    void AddEdge(int u, int v, LL cap) {
        Edge f = { u, v, cap, 0 }, b = { v, u, 0, 0 };
        adjlist[u].pb((int) edges.size()); edges.pb(f);
        adjlist[v].pb((int) edges.size()); edges.pb(b);
    }
    
    LL BlockingFlow(int s, int t) {
        queue<int> q; q.push(s);
        fill(ALL(d), -1); d[s] = 0;
        while (!q.empty() && d[t] == -1) {
            int u = q.front(); q.pop();
            for (int i = 0; i < (int) adjlist[u].size(); i++) {
                int id = adjlist[u][i], to = edges[id].v;
                if (d[to] == -1 && edges[id].flow < edges[id].cap) {
                    q.push(to);
                    d[to] = d[u] + 1;
                }
            }
        }
        if (d[t] == -1) return 0;
        fill(ALL(p), 0);
        LL flow = 0;
        while (LL pushed = Push(s, INF, t))
            flow += pushed;
        return flow;
    }
    
    LL Push(int u, LL flow, int t) {
        if (!flow) return 0;
        if (u == t) return flow;
        for (; p[u] < (int) adjlist[u].size(); ++p[u]) {
            int id = adjlist[u][p[u]], to = edges[id].v;
            if (d[to] != d[u] + 1) continue;
            LL res = edges[id].cap - edges[id].flow,
                pushed = Push(to, min(flow, res), t);
            if (pushed) {
                edges[id].flow += pushed;
                edges[id^1].flow -= pushed;
                return pushed;
            }
        }
        return 0;
    }
    
    LL Maxflow(int s, int t) {
        for (int i = 0; i < (int) edges.size(); i++)
            edges[i].flow = 0;
        LL totalFlow = 0;
        while (LL flow = BlockingFlow(s, t))
            totalFlow += flow;
        return totalFlow;
    }
};