#include "../template.hpp"

template <typename Cap = LL, typename Cost = LL> 
struct MCMF {
    struct Edge {
        int u, v;
        Cap cap, flow;
        Cost cost;
    };
    
    vector<Edge> edgeList;
    vector<vector<int> > adjList;
    vector<int> pi;
    vector<Cost> dist;
    vector<Cap> expand;
    
    MCMF(int N) {
        adjList.resize(N);
        dist.resize(N);
        expand.resize(N);
        pi.resize(N);
    }
    
    void AddEdge(int u, int v, Cap cap, Cost cost) {
        Edge f = { u, v, cap, 0, cost }, b = { v, u, 0, 0, -cost };
        adjList[u].pb((int) edgeList.size()); edgeList.pb(f);
        adjList[v].pb((int) edgeList.size()); edgeList.pb(b);
    }
    
    bool Dijkstra(int source, int sink) {
        fill(ALL(pi), -1);
        fill(ALL(dist), INF); dist[source] = 0;
        expand[source] = INF;
        priority_queue<pair<Cost, int> > pq;
        pq.push(mp(0, source));
        while (!pq.empty()) {
            pair<Cost, int> front = pq.top(); pq.pop();
            Cost d = -front.first;
            int u = front.second;
            if (d > dist[u]) continue;
            for (int i = 0; i < (int) adjList[u].size(); i++) {
                int id = adjList[u][i], to = edgeList[id].v;
                Cost cost = edgeList[id].cost;
                if (dist[u] + cost < dist[to] && edgeList[id].flow < edgeList[id].cap) {
                    dist[to] = dist[u] + cost;
                    pi[to] = id;
                    expand[to] = min(expand[u], edgeList[id].cap - edgeList[id].flow);
                    pq.push(mp(-dist[to], to));
                }
            }
        }
        if (pi[sink] == -1) return false;
        for (int k = pi[sink]; k != -1; k = pi[edgeList[k].u]) {
            edgeList[k].flow += expand[sink];
            edgeList[k^1].flow -= expand[sink];
        }
        return true;
    }
    
    pair<Cap, Cost> Get(int source, int sink) {
        for (int i = 0; i < (int) edgeList.size(); i++)
            edgeList[i].flow = 0;
        Cost totalCost = 0;
        Cap totalFlow = 0;
        while (Dijkstra(source, sink)) {
            totalCost += dist[sink] * expand[sink];
            totalFlow += expand[sink];
        }
        return mp(totalFlow, totalCost);
    }
};