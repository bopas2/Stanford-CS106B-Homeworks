/*
 *  Name: Thomas Lang
 *  Assignment: Trailblazer
 *  Section Leader: Joshua
 *
 *  In this assignment the program finds a route from point A to point B, giving the user the choice of which algorithim to use.
 *  Here is a brief description of each algorithim:
 *
 *  Breadth first search (BDF) :
 *      1: Reach new node, and mark it as green (visited) and add the node to the set of visited nodes
 *      2: Loop through that node's neighbors, see if the have already been visited,  if not: mark them yellow (reached),
 *         see if the node is the end point, if already visited, the program glosses over the node.
 *              a: if so, return that path
 *              b: otherwise, create a copy of the path so far, add the new node to the copy and enqueue the new path back into the queue.
 *      3: Loop until the queue is empty, or we have found our answer
 *
 *  Dijkstra's algorithim :
 *      -This algorithim is very similar to BDF, but instead of using a regular queue we use a priority queue, where the path's priority is it's
 *       total cost. This means that we will explore paths that cost less first, which allows us to find the most efficient path to our endpoint.
 *
 *  A* algorithim :
 *      -This algorithim is very similar to Dijkstra's, but, in addition to the total 'so-far' cost of the path we add an amount called a "Heuristic".
 *       This Heuristic makes paths that are moving away from the target more expensive and paths moving towards out target less expensive. This makes
 *       the algorithim more efficient because we ignore path's that won't be the most efficient
 *
 *  Brief explanation of Heuristic :
 *      Since the cost of going from one node to another node is measure in time, out heuristic is also in time units. We calculate the direct distance
 *      from the last node in the path to the end point and divide it by our maximum road speed. This gives us the time it would take to directly travel
 *      to our destination. Thus, points that are further away will have a higher cost, because they are farther away.
 *
 *  Alternative Route :
 *      This algorithim finds the second best route using the A* algorithim. The program first runs A* to find the most efficient path. Then the program
 *      finds the next fastest path, that is still significantly different from the original, by looping through the nodes of the most efficient path,
 *      then running A* under the condition that we cannot use certain nodes. The path that is different from the original, and is the cheapest
 *      alternative path is our answer.
 *
 */

#include "Trailblazer.h"
#include "set.h"
#include "queue.h"
#include "pqueue.h"
#include "vector.h"
#include "map.h"

using namespace std;

Path mapTracting(const RoadGraph& graph, RoadNode* start, RoadNode* end, int whichAlgorithim, RoadNode* ignore, double &priority);
double getHueristic(const RoadGraph& graph, RoadNode* start, RoadNode* end);
Path determineBestPath(const RoadGraph &graph, PriorityQueue<Path> paths, Path bestPath);
Path twoWayRoadA(const RoadGraph& graph, RoadNode* start, RoadNode* end);
static const double SUFFICIENT_DIFFERENCE = 0.2;

//Find the path from pt A to B using Breadth First Searching, scroll above for a greater explanation
Path breadthFirstSearch(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    Set<RoadNode*> visitedNodes;                        //Keeps a set of nodes we have already visited, removes redundancy
    Queue<Path> queue;                                  //In BDF we use a regular queue
    Path firstPath;
    firstPath.add(start);
    queue.enqueue(firstPath);
    while(!queue.isEmpty()) {                           //While we still have paths that aren't dead ends, we continue
        Path path = queue.dequeue();
        RoadNode* lastPath = path.get(path.size() - 1);  //Returns the last node in the path
        if(!visitedNodes.contains(lastPath)) {          //This determines if we have already visited the node
            visitedNodes.add(lastPath);                 //If not, we now have so we mark this node as visited
            lastPath->setColor(Color::GREEN);           //And highlight it as such
            if(lastPath == end) {                       //And if the node is the final destination, we end
                return path;
            }
            for(RoadNode* i : graph.neighborsOf(lastPath)) {    //Other wise we loop through the node's neighbors
                if(!visitedNodes.contains(i)) {                 //If they haven't been visited, we highlight them yelow
                    Path newPath = path;
                    i->setColor(Color::YELLOW);  //Then we create a copy of the path so far, add the neighbor to the end of it and enqueue it
                    newPath.add(i);
                    queue.enqueue(newPath);
                }
            }
        }
    }
    return {}; //If we reach the end of the function, it is implied that no path can be found
}

//Find the path from pt A to B using Dijkstra's Algorithim, scroll above for a greater explanation
Path dijkstrasAlgorithm(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    //For Alternative path we pass the helper function a double which it can edit, so we can save the cost of paths,
    //for dijkstrasAlgorithim, however, this double has no significance.
    double placeHolder = 0;
    return mapTracting(graph,start,end,0,nullptr, placeHolder);
}

//Find the path from pt A to B using aStar Algorithim, scroll above for a greater explanation
Path aStar(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    //For Alternative path we pass the helper function a double which it can edit, so we can save the cost of paths,
    //for A*, however, this double has no significance.
    double x = 0;
    return mapTracting(graph,start,end,1,nullptr,x);
}

//This function returns the Hueristic cost for a node, used for A*
double getHueristic(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    return graph.crowFlyDistanceBetween(start,end) / (graph.maxRoadSpeed());
}

/*
 *  This is the helper function that returns the path for Dijkstra, A* and Alternative path. Because these algorithims are similar,
*   we can be efficient by having one function do the work for the three algorithims. But to order to do this we must pass certain
*   parameters so the function knows which algorithim we are using to solve. Some clarification of the new parameters:
*   -- int whichAlgorithim: We multiply any hueristics with this integer. For dijkstra we pass in 0, thus when we multiply the hueristic by the parameter,
*   the parameter nullfies it. If we ARE using hueristics we pass in 1, which doesn't nullify the hueristic.
*   -- RoadNode* ignore: This is used by alternative path when we must ignore nodes to find alternative routes, if we aren't using the alternative
*   path function (nullptr), we continue like usual. If a node is passed in, we add the node to the set of visited nodes so we don't add it to any paths.
*   -- Double Priority: For the Alternative pathing algorithim we need to create a priority queue, by passing in a double by reference we can tell the
*   'parent' function the cost of the path we are returning
*/
Path mapTracting(const RoadGraph& graph, RoadNode* start, RoadNode* end, int whichAlgorithim, RoadNode* ignore, double& priority) {
    Set<RoadNode*> visitedNodes;                //Keeps a set of nodes we have already visited
    if(ignore != nullptr) {                     //If we need to ignore a node, we mark that node as visited
        visitedNodes.add(ignore);
    }
    PriorityQueue<Path> queue;
    Path firstPath;
    firstPath.add(start);
    queue.enqueue(firstPath,(whichAlgorithim * getHueristic(graph,firstPath.get(firstPath.size() - 1),end)));            //We enqueue our first path
    while(!queue.isEmpty()) {                                           //While there are still nodes we can visit, we continue
        double originalPathCost = queue.peekPriority();               //Saves the cost of the path so we can have access to it's cost after we dequeue it
        Path path = queue.dequeue();
        //The line below removes the additional hueristic cost of the previous node, so we can have an unbiased path cost.
        double originalPriority = originalPathCost - (whichAlgorithim * getHueristic(graph,path.get(path.size() - 1),end));
        RoadNode* lastPath = path.get(path.size() - 1);                     //Get the last node in the path
        if(!visitedNodes.contains(lastPath)) {                              //If we haven't already seen this node
            visitedNodes.add(lastPath);                                     //Mark this node has visited
            lastPath->setColor(Color::GREEN);                               //Highlight it as visited
            if(lastPath == end) {                                           //See if we have reached the destination
                priority = originalPriority; //Since priority is passed by reference we can use it for our Pqueue in the alternate path algorithim
                return path;                //Ends search
            }
            for(RoadNode* i : graph.neighborsOf(lastPath)) { //Otherwise glance at a node's neighbors
                if(!visitedNodes.contains(i)) {             //Ensures that we don't highlight green nodes yellow
                    Path newPath = path;                    //Copy the current path we have
                    i->setColor(Color::YELLOW);             //Highlight that node as glanced at
                    newPath.add(i);                         //Add the node we are glancing at to the end of the copy of the path
                    //The line below updates the cost of the path (with the hueristic if whichAlgorithim deems it approprite)
                    double Ppriority = graph.edgeBetween(lastPath,i)->cost() + originalPriority + (whichAlgorithim * getHueristic(graph,i,end));
                    queue.add(newPath,Ppriority); //enqueue the copy
                }
            }
        }
    }
    return {}; //If we reach the end of the function we know there isn't a possible path so we return an empty path
}

//This function is used by the Alternative route to find the next best path possible, that is inexpensive, and different enough from the most
//efficient path to be deemed an alternative path.
Path determineBestPath(const RoadGraph& graph, PriorityQueue<Path> paths, Path bestPath) {
    Set<RoadNode*> bestPathNodes;
    for(RoadNode* i : bestPath) {
        bestPathNodes.add(i);   //Creates a set of the nodes made from the best path, used to see how many of the same nodes are in alternative paths
    }
    //Now we go through the priority queue and find the path that has a sufficient difference from the best path
    while(!paths.isEmpty()) {
        int tempPriority = paths.peekPriority(); //Ensures that empty paths aren't returned
        int sameNodeCount = 0;
        Path temp = paths.dequeue();
        for(RoadNode* i : temp) {
            if(bestPathNodes.contains(i)) {     //If both paths contain a node we increment a count
                sameNodeCount++;
            }
        }
        double x = bestPath.size() - sameNodeCount; //This gets the amount of nodes that are unique to the new path
        //If the % of nodes that are different is greater than 20% we can return this path as an approprite alternative path.
        if(x / bestPath.size() > SUFFICIENT_DIFFERENCE && tempPriority > 0) {
            return temp;
        }
    }
    return {}; //If no path is different enough from the original we return an empty path.
}

// This function returns the second best path that is a certain degree different than the best path
Path alternativeRoute(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    double temp = 0;
    Path bestRoute = mapTracting(graph,start,end,1,nullptr,temp); // Find best route
    PriorityQueue<Path> possibleAlternateRoutes;
    //Loops through the best path, we then call the findPath function that will find the best path ignoring the node we are currently on.
    //Each path must include the first and last node of the best path (start and end) so the for loop reflects this.
    for(int i = 1; i < bestRoute.size() - 1; i++) {
        double priority = 0; //Passed by reference to determine the cost of path
        Path tempo = mapTracting(graph,start,end,1,bestRoute.get(i),priority); //Finds a path ignoring the node we are currently on
        possibleAlternateRoutes.add(tempo,priority); //Adds all the alternate paths to the Pqueue, based on their costs
    }
    return determineBestPath(graph,possibleAlternateRoutes, bestRoute); //Calls the helper function that will return the BEST alternative route
}
