#include "../template.hpp"

template <class Flow = LL, class Cost = LL> 
struct MCMF {
    
    struct Edge {
        int u, v;
        Flow cap, flow;
        Cost cost;
    };
    
    vector<Edge> edges;
    vector<vector<int> > adjlist;
    vector<int> p;
    vector<Cost> dist;
    vector<Flow> pushed;
    
    MCMF(int N) {
        adjlist.resize(N);
        p.resize(N);
        dist.resize(N);
        pushed.resize(N);
    }
    
    void AddEdge(int u, int v, Flow cap, Cost cost) {
        Edge f = { u, v, cap, 0, cost }, b = { v, u, 0, 0, -cost };
        adjlist[u].pb((int) edges.size()); edges.pb(f);
        adjlist[v].pb((int) edges.size()); edges.pb(b);
    }
    
    bool Dijkstra(int s, int t) {
        fill(ALL(p), -1);
        fill(ALL(dist), INF); dist[s] = 0;
        pushed[s] = INF;
        priority_queue<pair<Cost, int> > q; q.push(mp(0, s));
        while (!q.empty()) {
            pair<Cost, int> front = q.top(); q.pop();
            Cost d = -front.first; int u = front.second;
            if (d > dist[u]) continue;
            for (int i = 0; i < (int) adjlist[u].size(); i++) {
                int id = adjlist[u][i], to = edges[id].v;
                Cost cost = edges[id].cost;
                if (dist[u] + cost < dist[to] && edges[id].flow < edges[id].cap) {
                    dist[to] = dist[u] + cost;
                    p[to] = id;
                    pushed[to] = min(pushed[u], edges[id].cap - edges[id].flow);
                    q.push(mp(-dist[to], to));
                }
            }
        }
        if (p[t] == -1) return false;
        for (int k = p[t]; k != -1; k = p[edges[k].u]) {
            edges[k].flow += pushed[t];
            edges[k^1].flow -= pushed[t];
        }
        return true;
    }
    
    pair<Flow, Cost> Get(int s, int t) {
        for (int i = 0; i < (int) edges.size(); i++)
            edges[i].flow = 0;
        Flow totalFlow = 0; Cost totalCost = 0;
        while (Dijkstra(s, t)) {
            totalFlow += pushed[t];
            totalCost += dist[t] * pushed[t];
        }
        return mp(totalFlow, totalCost);
    }
};