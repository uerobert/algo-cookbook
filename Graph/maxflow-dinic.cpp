#include "../template.hpp"

struct Dinic {
    struct Edge {
        int u, v;
        LL cap, flow;
    };
    
    vector<Edge> edgeList;
    vector<vector<int> > adjList;
    vector<int> depth, pi;
    
    Dinic(int N) {
        adjList.resize(N);
        depth.resize(N);
        pi.resize(N);
    }
    
    void AddEdge(int u, int v, long long cap) {
        Edge f = { u, v, cap, 0 }, b = { v, u, 0, 0 };
        adjList[u].pb((int) edgeList.size());
        edgeList.pb(f);
        adjList[v].pb((int) edgeList.size());
        edgeList.pb(b);
    }
    
    LL BlockingFlow(int source, int sink) {
        queue<int> q;
        q.push(source);
        fill(ALL(depth), -1);
        depth[source] = 0;
        while (!q.empty() && depth[sink] == -1) {
            int u = q.front();
            q.pop();
            for (int i = 0; i < adjList[u].size(); i++) {
                int id = adjList[u][i], to = edgeList[id].v;
                if (depth[to] == -1 && edgeList[id].flow < edgeList[id].cap) {
                    q.push(to);
                    depth[to] = depth[u] + 1;
                }
            }
        }
        fill(ALL(pi), 0);
        LL flow = 0;
        while (LL pushed = Push(source, INF, sink))
            flow += pushed;
        return flow;
    }
    
    LL Push(int u, LL flow, int sink) {
        if (!flow) return 0;
        if (u == sink) return flow;
        for (; pi[u] < (int) adjList[u].size(); ++pi[u]) {
            int id = adjList[u][pi[u]], to = edgeList[id].v;
            if (depth[to] != depth[u] + 1) continue;
            long long res = edgeList[id].cap - edgeList[id].flow,
                pushed = Push(to, min(flow, res), sink);
            if (pushed) {
                edgeList[id].flow += pushed;
                edgeList[id^1].flow -= pushed;
                return pushed;
            }
        }
        return 0;
    }
    
    LL Maxflow(int source, int sink) {
        LL totalFlow = 0;
        while (LL flow = BlockingFlow(source, sink)) {
            totalFlow += flow;
        }
        return totalFlow;
    }
};