// Problem 5: Fastest Route with Time Schedules
#include "graph_loader.h"
#include <climits>

struct State
{
    int node;
    int timeMinutes;
    State(int n, int t) : node(n), timeMinutes(t) {}
    bool operator>(const State &other) const
    {
        return timeMinutes > other.timeMinutes;
    }
};

class Problem5Solver
{
private:
    GraphLoader &graph;
    map<string, double> speedKmh;
    map<string, int> scheduleInterval;
    map<string, pair<int, int>> operatingHours;

public:
    Problem5Solver(GraphLoader &g) : graph(g)
    {
        // Speed: 10 km/h for all vehicles
        speedKmh["road"] = 10.0;
        speedKmh["metro"] = 10.0;
        speedKmh["bikolpo"] = 10.0;
        speedKmh["uttara"] = 10.0;

        // Schedules: every 15 minutes from 6 AM to 11 PM
        scheduleInterval["metro"] = 15;
        scheduleInterval["bikolpo"] = 15;
        scheduleInterval["uttara"] = 15;

        // Operating hours
        operatingHours["metro"] = {6 * 60, 23 * 60};
        operatingHours["bikolpo"] = {6 * 60, 23 * 60};
        operatingHours["uttara"] = {6 * 60, 23 * 60};
    }

    int getNextDeparture(const string &type, int currentTime)
    {
        if (type == "road")
            return currentTime;

        pair<int, int> hours = operatingHours[type];
        int start = hours.first;
        int end = hours.second;
        int interval = scheduleInterval[type];

        if (currentTime < start)
            return start;
        if (currentTime >= end)
            return -1;

        int timeSinceStart = currentTime - start;
        int nextOffset = ((timeSinceStart + interval - 1) / interval) * interval;
        int nextDeparture = start + nextOffset;

        if (nextDeparture >= end)
            return -1;
        return nextDeparture;
    }

    pair<vector<int>, int> solve(int start, int end, int startTime)
    {
        const vector<Node> &nodes = graph.getNodes();
        map<pair<int, int>, int> visited;
        map<pair<int, int>, pair<int, int>> parent;
        priority_queue<State, vector<State>, greater<State>> pq;

        pq.push(State(start, startTime));
        visited[{start, startTime}] = startTime;

        pair<int, int> bestEnd = {-1, -1};
        int bestTime = INT_MAX;

        while (!pq.empty())
        {
            State current = pq.top();
            pq.pop();

            auto key = make_pair(current.node, current.timeMinutes);
            if (visited.count(key) && current.timeMinutes > visited[key])
                continue;

            if (current.node == end && current.timeMinutes < bestTime)
            {
                bestTime = current.timeMinutes;
                bestEnd = key;
            }

            for (const Edge &e : nodes[current.node].edges)
            {
                int departTime = getNextDeparture(e.type, current.timeMinutes);
                if (departTime == -1)
                    continue;

                double travelTimeHours = e.distance / speedKmh[e.type];
                int travelTimeMinutes = (int)(travelTimeHours * 60);
                int arrivalTime = departTime + travelTimeMinutes;

                auto newKey = make_pair(e.to, arrivalTime);
                if (!visited.count(newKey) || arrivalTime < visited[newKey])
                {
                    visited[newKey] = arrivalTime;
                    parent[newKey] = key;
                    pq.push(State(e.to, arrivalTime));
                }
            }
        }

        // Reconstruct path
        vector<int> path;
        if (bestEnd.first != -1)
        {
            auto curr = bestEnd;
            while (curr.first != -1)
            {
                path.push_back(curr.first);
                if (parent.count(curr))
                {
                    curr = parent[curr];
                }
                else
                {
                    break;
                }
            }
            reverse(path.begin(), path.end());
        }

        return {path, bestTime};
    }

    void printSolution(const Point &source, const Point &dest, const TimeInfo &startTime)
    {
        cout << "\nProblem 5: Fastest Route with Schedules" << endl;
        cout << "Source: (" << source.lon << ", " << source.lat << ")" << endl;
        cout << "Destination: (" << dest.lon << ", " << dest.lat << ")" << endl;
        cout << "Start Time: " << startTime.toString() << endl;

        int startNode = graph.findNearestNode(source);
        int endNode = graph.findNearestNode(dest);

        auto result = solve(startNode, endNode, startTime.toMinutes());
        vector<int> path = result.first;
        int endTime = result.second;

        if (path.empty())
        {
            cout << "No route found!" << endl;
            return;
        }

        const vector<Node> &nodes = graph.getNodes();
        int travelTime = endTime - startTime.toMinutes();
        cout << "Travel Time: " << travelTime << " minutes" << endl;
        cout << "Arrival Time: " << TimeInfo::fromMinutes(endTime).toString() << endl;

        // Generate KML
        vector<Point> points;
        for (int idx : path)
        {
            points.push_back(nodes[idx].location);
        }
        generateKML(points, "problem5_route.kml");
        cout << "KML file generated: problem5_route.kml" << endl;

        // Print route summary
        cout << "\nRoute Summary:" << endl;
        cout << "Total nodes: " << path.size() << endl;
        double totalDist = 0;
        for (size_t i = 1; i < path.size(); i++)
        {
            totalDist += haversineDistance(nodes[path[i - 1]].location, nodes[path[i]].location);
        }
        cout << "Total distance: " << fixed << setprecision(2) << totalDist << " km" << endl;
    }
};

int main()
{
    cout << "Problem 5: Fastest Route with Schedules" << endl;
    cout << "Loading data..." << endl;

    GraphLoader graph;
    graph.loadAllData();
    cout << "Loaded " << graph.getNodeCount() << " nodes" << endl;

    Problem5Solver solver(graph);

    double srcLon, srcLat, dstLon, dstLat;
    int startHour, startMin;

    cout << "\nEnter source coordinates (longitude latitude): ";
    cin >> srcLon >> srcLat;
    cout << "Enter destination coordinates (longitude latitude): ";
    cin >> dstLon >> dstLat;
    cout << "Enter start time (hour minute, 24-hour format): ";
    cin >> startHour >> startMin;

    Point source(srcLon, srcLat);
    Point dest(dstLon, dstLat);
    TimeInfo startTime(startHour, startMin);

    solver.printSolution(source, dest, startTime);

    return 0;
}
