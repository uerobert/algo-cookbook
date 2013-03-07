#include "../template.hpp"

struct MCMF {
    struct Edge {
        int u, v;
        LL cap, cost, flow;
    };
    
    vector<Edge> edgeList;
    vector<vector<int> > adjList;
    vector<int> pi;
    vector<LL> dist, expand;
    
    MCMF(int N) {
        adjList.resize(N);
        dist.resize(N);
        expand.resize(N);
        pi.resize(N);
    }
    
    void AddEdge(int u, int v, LL cap, LL cost) {
        Edge f = { u, v, cap, cost, 0 }, b = { v, u, 0, -cost, 0 };
        adjList[u].pb((int) edgeList.size()); edgeList.pb(f);
        adjList[v].pb((int) edgeList.size()); edgeList.pb(b);
    }
    
    LL Dijkstra(int source, int sink) {
        fill(ALL(pi), -1);
        fill(ALL(dist), INF); dist[source] = 0;
        expand[source] = INF;
        priority_queue<pair<LL, int>, vector<pair<LL, int> >, greater<pair<LL, int> > > pq;
        pq.push(mp(0, source));
        while (!pq.empty()) {
            pair<LL, int> front = pq.top(); pq.pop();
            LL d = front.first, u = front.second;
            if (d == dist[u]) {
                for (int i = 0; i < (int) adjList[u].size(); i++) {
                    int id = adjList[u][i], to = edgeList[id].v;
                    LL cost = edgeList[id].cost;
                    if (dist[u] + cost < dist[to] && edgeList[id].flow < edgeList[id].cap) {
                        dist[to] = dist[u] + cost;
                        pi[to] = id;
                        expand[to] = min(expand[u], edgeList[id].cap - edgeList[id].flow);
                        pq.push(mp(dist[to], to));
                    }
                }
            }
        }
        if (pi[sink] == -1) return 0;
        int k = pi[sink];
        while (k != -1) {
            edgeList[k].flow += expand[sink];
            edgeList[k^1].flow -= expand[sink];
            k = pi[edgeList[k].u];
        }
        return dist[sink] * expand[sink];
    }
    
    pair<LL, LL> Get(int source, int sink) {
        for (int i = 0; i < (int) edgeList.size(); i++)
            edgeList[i].flow = 0;
        LL totalCost = 0, totalFlow = 0;
        while (LL cost = Dijkstra(source, sink)) {
            totalCost += cost;
            totalFlow += expand[sink];
        }
        return mp(totalFlow, totalCost);
    }
};