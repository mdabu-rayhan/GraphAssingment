// Problem 6: Cheapest Route with Time Constraints
#include "graph_loader.h"

struct State
{
    int node;
    double cost;
    int timeMinutes;
    State(int n, double c, int t) : node(n), cost(c), timeMinutes(t) {}
    bool operator>(const State &other) const
    {
        return cost > other.cost;
    }
};

class Problem6Solver
{
private:
    GraphLoader &graph;
    map<string, double> costPerKm;
    map<string, double> speedKmh;
    map<string, int> scheduleInterval;
    map<string, pair<int, int>> operatingHours;

public:
    Problem6Solver(GraphLoader &g) : graph(g)
    {
        // Costs (Problem 6 specific)
        costPerKm["road"] = 20.0;
        costPerKm["metro"] = 5.0;
        costPerKm["bikolpo"] = 7.0;
        costPerKm["uttara"] = 10.0;

        // Speeds (Problem 6 specific)
        speedKmh["road"] = 20.0;
        speedKmh["metro"] = 15.0;
        speedKmh["bikolpo"] = 10.0;
        speedKmh["uttara"] = 12.0;

        // Schedules (Problem 6 specific)
        scheduleInterval["metro"] = 5;
        scheduleInterval["bikolpo"] = 20;
        scheduleInterval["uttara"] = 10;

        // Operating hours (Problem 6 specific)
        operatingHours["metro"] = {1 * 60, 23 * 60};
        operatingHours["bikolpo"] = {7 * 60, 22 * 60};
        operatingHours["uttara"] = {6 * 60, 23 * 60};
    }

    int getNextDeparture(const string &type, int currentTime)
    {
        if (type == "road")
            return currentTime; // Car available immediately

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

    pair<vector<int>, pair<double, int>> solve(int start, int end, int startTime, int deadline)
    {
        const vector<Node> &nodes = graph.getNodes();
        map<pair<int, int>, double> dist;
        map<pair<int, int>, pair<int, int>> parent;
        priority_queue<State, vector<State>, greater<State>> pq;

        dist[{start, startTime}] = 0;
        pq.push(State(start, 0, startTime));

        pair<int, int> bestEnd = {-1, -1};
        double bestCost = INF;

        while (!pq.empty())
        {
            State current = pq.top();
            pq.pop();

            auto key = make_pair(current.node, current.timeMinutes);
            if (dist.count(key) && current.cost > dist[key])
                continue;

            // Must reach before deadline
            if (current.timeMinutes > deadline)
                continue;

            if (current.node == end && current.cost < bestCost)
            {
                bestCost = current.cost;
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

                // Skip if we would arrive after deadline
                if (arrivalTime > deadline)
                    continue;

                double edgeCost = e.distance * costPerKm[e.type];
                double newCost = current.cost + edgeCost;

                auto newKey = make_pair(e.to, arrivalTime);
                if (!dist.count(newKey) || newCost < dist[newKey])
                {
                    dist[newKey] = newCost;
                    parent[newKey] = key;
                    pq.push(State(e.to, newCost, arrivalTime));
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

        return {path, {bestCost, bestEnd.second}};
    }

    void printSolution(const Point &source, const Point &dest,
                       const TimeInfo &startTime, const TimeInfo &deadline)
    {
        cout << "\nProblem 6: Cheapest Route with Time Constraints" << endl;
        cout << "Source: (" << source.lon << ", " << source.lat << ")" << endl;
        cout << "Destination: (" << dest.lon << ", " << dest.lat << ")" << endl;
        cout << "Start Time: " << startTime.toString() << endl;
        cout << "Deadline: " << deadline.toString() << endl;

        int startNode = graph.findNearestNode(source);
        int endNode = graph.findNearestNode(dest);

        auto pathResult = solve(startNode, endNode,
                                startTime.toMinutes(), deadline.toMinutes());
        vector<int> path = pathResult.first;
        double totalCost = pathResult.second.first;
        int endTime = pathResult.second.second;

        if (path.empty())
        {
            cout << "No route found that reaches before deadline!" << endl;
            return;
        }

        const vector<Node> &nodes = graph.getNodes();
        cout << "Total Cost: " << fixed << setprecision(2) << totalCost << endl;
        cout << "Arrival Time: " << TimeInfo::fromMinutes(endTime).toString() << endl;

        int slack = deadline.toMinutes() - endTime;
        cout << "Time before deadline: " << slack << " minutes" << endl;

        // Generate KML
        vector<Point> points;
        for (int idx : path)
        {
            points.push_back(nodes[idx].location);
        }
        generateKML(points, "problem6_route.kml");
        cout << "KML file generated: problem6_route.kml" << endl;

        // Print detailed route with modes
        cout << "\nDetailed Route:" << endl;
        cout << "Number of segments: " << path.size() - 1 << endl;

        double totalDist = 0;
        map<string, double> modeDistance;
        map<string, double> modeCost;

        for (size_t i = 1; i < path.size(); i++)
        {
            double dist = haversineDistance(nodes[path[i - 1]].location, nodes[path[i]].location);
            totalDist += dist;

            string edgeType = "road";
            for (const Edge &e : nodes[path[i - 1]].edges)
            {
                if (e.to == path[i])
                {
                    edgeType = e.type;
                    break;
                }
            }

            modeDistance[edgeType] += dist;
            modeCost[edgeType] += dist * costPerKm[edgeType];
        }

        cout << "\nBreakdown by transport mode:" << endl;
        for (const auto &pair : modeDistance)
        {
            string mode = pair.first;
            double dist = pair.second;
            cout << "  " << getModeDescription(mode) << ": "
                 << dist << " km, Cost: " << modeCost[mode] << endl;
        }
    }
};

int main()
{
    cout << "Problem 6: Cheapest Route with Time Constraints" << endl;
    cout << "Loading data..." << endl;

    GraphLoader graph;
    graph.loadAllData();
    cout << "Loaded " << graph.getNodeCount() << " nodes" << endl;

    Problem6Solver solver(graph);

    double srcLon, srcLat, dstLon, dstLat;
    int startHour, startMin, deadlineHour, deadlineMin;

    cout << "\nEnter source coordinates (longitude latitude): ";
    cin >> srcLon >> srcLat;
    cout << "Enter destination coordinates (longitude latitude): ";
    cin >> dstLon >> dstLat;
    cout << "Enter start time (hour minute, 24-hour format): ";
    cin >> startHour >> startMin;
    cout << "Enter deadline time (hour minute, 24-hour format): ";
    cin >> deadlineHour >> deadlineMin;

    Point source(srcLon, srcLat);
    Point dest(dstLon, dstLat);
    TimeInfo startTime(startHour, startMin);
    TimeInfo deadline(deadlineHour, deadlineMin);

    solver.printSolution(source, dest, startTime, deadline);

    return 0;
}
