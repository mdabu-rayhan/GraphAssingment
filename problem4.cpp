// Problem 4: Cheapest Route with Time Schedules
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

class Problem4Solver
{
private:
    GraphLoader &graph;
    map<string, double> costPerKm;
    map<string, double> speedKmh;
    map<string, int> scheduleInterval;          // minutes
    map<string, pair<int, int>> operatingHours; // start, end in minutes

public:
    Problem4Solver(GraphLoader &g) : graph(g)
    {
        // Costs
        costPerKm["road"] = 20.0;
        costPerKm["metro"] = 5.0;
        costPerKm["bikolpo"] = 7.0;
        costPerKm["uttara"] = 7.0;

        // Speed: 30 km/h for all
        speedKmh["road"] = 30.0;
        speedKmh["metro"] = 30.0;
        speedKmh["bikolpo"] = 30.0;
        speedKmh["uttara"] = 30.0;

        // Schedules: every 15 minutes from 6 AM to 11 PM
        scheduleInterval["metro"] = 15;
        scheduleInterval["bikolpo"] = 15;
        scheduleInterval["uttara"] = 15;

        // Operating hours (in minutes from midnight)
        operatingHours["metro"] = {6 * 60, 23 * 60};
        operatingHours["bikolpo"] = {6 * 60, 23 * 60};
        operatingHours["uttara"] = {6 * 60, 23 * 60};
    }

    int getNextDeparture(const string &type, int currentTime)
    {
        if (type == "road")
            return currentTime; // Car available immediately

        int start = operatingHours[type].first;
        int end = operatingHours[type].second;
        int interval = scheduleInterval[type];

        // If before operating hours, wait until first departure
        if (currentTime < start)
            return start;

        // If after operating hours, no service
        if (currentTime >= end)
            return -1;

        // Find next departure
        int timeSinceStart = currentTime - start;
        int nextOffset = ((timeSinceStart + interval - 1) / interval) * interval;
        int nextDeparture = start + nextOffset;

        if (nextDeparture >= end)
            return -1;
        return nextDeparture;
    }

    pair<vector<int>, pair<double, int>> solve(int start, int end, int startTime)
    {
        const vector<Node> &nodes = graph.getNodes();
        map<pair<int, int>, double> dist;           // (node, time) -> cost
        map<pair<int, int>, pair<int, int>> parent; // (node, time) -> (parent_node, parent_time)
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

            if (current.node == end && current.cost < bestCost)
            {
                bestCost = current.cost;
                bestEnd = key;
            }

            for (const Edge &e : nodes[current.node].edges)
            {
                int departTime = getNextDeparture(e.type, current.timeMinutes);
                if (departTime == -1)
                    continue; // No service available

                int waitTime = departTime - current.timeMinutes;
                double travelTimeHours = e.distance / speedKmh[e.type];
                int travelTimeMinutes = (int)(travelTimeHours * 60);
                int arrivalTime = departTime + travelTimeMinutes;

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

    void printSolution(const Point &source, const Point &dest, const TimeInfo &startTime)
    {
        cout << "\nProblem 4: Cheapest Route with Schedules" << endl;
        cout << "Source: (" << source.lon << ", " << source.lat << ")" << endl;
        cout << "Destination: (" << dest.lon << ", " << dest.lat << ")" << endl;
        cout << "Start Time: " << startTime.toString() << endl;

        int startNode = graph.findNearestNode(source);
        int endNode = graph.findNearestNode(dest);

        auto pathResult = solve(startNode, endNode, startTime.toMinutes());
        vector<int> path = pathResult.first;
        double totalCost = pathResult.second.first;
        int endTime = pathResult.second.second;

        if (path.empty())
        {
            cout << "No route found!" << endl;
            return;
        }

        const vector<Node> &nodes = graph.getNodes();
        cout << "Total Cost: " << fixed << setprecision(2) << totalCost << endl;
        cout << "Arrival Time: " << TimeInfo::fromMinutes(endTime).toString() << endl;

        // Generate KML
        vector<Point> points;
        for (int idx : path)
        {
            points.push_back(nodes[idx].location);
        }
        generateKML(points, "problem4_route.kml");
        cout << "KML file generated: problem4_route.kml" << endl;
    }
};

int main()
{
    cout << "Problem 4: Cheapest Route with Schedules" << endl;
    cout << "Loading data..." << endl;

    GraphLoader graph;
    graph.loadAllData();
    cout << "Loaded " << graph.getNodeCount() << " nodes" << endl;

    Problem4Solver solver(graph);

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
