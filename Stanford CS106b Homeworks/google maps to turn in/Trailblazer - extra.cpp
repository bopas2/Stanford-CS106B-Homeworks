/*
 *  Name: Thomas Lang
 *  Assignment: Trailblazer
 *  Section Leader: Joshua
 *
 *  This is Trailblazer extended, the extension that I completed was the bi-directional A* algorithim. This basically means that the program
 *  completes A* by running A* at each node, and when the paths collide we make the combined path the final path. This is more efficient than plain
 *  A* because now A* doesn't explore as many un-nessesary nodes.
 *
 *  To use bidirectional A* run breadthFirstSearch
 *
 *  A more fun extension that I invented myself I call "apple maps", this function finds a very inefficient route from the first node to the last node.
 *  This algorithim may be used by taxi drivers to get a large fare for tourists in foreign countries. This is done by giving more costly paths a higher
 *  priority. (The joke is that apple maps is an inferior application).
 *
 *  Run Dijkstra to run apple maps
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
double getHueristic(const RoadGraph& graph, RoadNode* start, RoadNode* end);
Path determineBestPath(const RoadGraph &graph, PriorityQueue<Path> paths, Path bestPath);
Path twoWayRoadA(const RoadGraph& graph, RoadNode* start, RoadNode* end);
static const double SUFFICIENT_DIFFERENCE = 0.2;


double getHueristic(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    return graph.crowFlyDistanceBetween(start,end) / (graph.maxRoadSpeed());
}


//Bidirectional A* explained above
Path breadthFirstSearch(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    Set<RoadNode*> visitedNodesA; Set<RoadNode*> visitedNodesB;
    PriorityQueue<Path> queueA; PriorityQueue<Path> queueB;
    Path firstPathA; Path firstPathB;
    firstPathA.add(start); firstPathB.add(end);
    queueA.enqueue(firstPathA,getHueristic(graph,firstPathA.get(firstPathA.size() - 1),end));
    queueB.enqueue(firstPathB,getHueristic(graph,firstPathB.get(firstPathB.size() - 1),start));
    while(!queueA.isEmpty() || !queueB.isEmpty()) {
        if(!queueA.isEmpty()) {
            double oldPriorityAndHueristic = queueA.peekPriority();
            Path pathA = queueA.dequeue();
            RoadNode * lastNode = pathA.get(pathA.size() - 1);
            double actualPathCost = oldPriorityAndHueristic - getHueristic(graph,lastNode, end);
            if(!visitedNodesA.contains(lastNode)) {
                visitedNodesA.add(lastNode);
                lastNode->setColor(Color::GREEN);
                if(lastNode == end) {
                    return pathA;
                }
                if(visitedNodesB.contains(lastNode)) {
                    int index = 0; bool found = false; Path thisPathB;
                    while(!queueB.isEmpty()) {
                        Path x = queueB.dequeue();
                        index = 0;
                        for(RoadNode * i : x) {
                            if(i == lastNode) {
                                found = true;
                                thisPathB = x;
                                break;
                            }
                            index++;
                        }
                        if(found) {
                            break;
                        }
                    }
                    for(int h = (index - 1); h >= 0; h--) {
                        pathA.add(thisPathB[h]);
                    }
                    return pathA;
                }
                for(RoadNode* i : graph.neighborsOf(lastNode)) {
                    if(!visitedNodesA.contains(i)) {
                        Path newPath = pathA;
                        i->setColor(Color::YELLOW);
                        newPath.add(i);
                        double Ppriority = graph.edgeBetween(lastNode,i)->cost() + actualPathCost + (getHueristic(graph,i,end));
                        queueA.enqueue(newPath,Ppriority); //enqueue the copy
                    }
                }
            }
            if(!queueB.isEmpty()) {
                double oldPriorityAndHueristic = queueB.peekPriority();
                Path pathB = queueB.dequeue();
                RoadNode * lastNode = pathB.get(pathB.size() - 1);
                double actualPathCost = oldPriorityAndHueristic - getHueristic(graph,lastNode, start);
                if(!visitedNodesB.contains(lastNode)) {
                    visitedNodesB.add(lastNode);
                    lastNode->setColor(Color::GREEN);
                    if(lastNode == start) {
                        Path final;
                        for(int k = pathB.size() - 1; k >= 0; k--) {
                            final.add(pathB[k]);
                        }
                        return final;
                    }
                    if(visitedNodesA.contains(lastNode)) {
                        int index = 0; bool found = false; Path thisPathA;
                        while(!queueA.isEmpty()) {
                            Path x = queueA.dequeue();
                            index = 0;
                            for(RoadNode * i : x) {
                                if(i == lastNode) {
                                    found = true;
                                    thisPathA = x;
                                    break;
                                }
                                index++;
                            }
                            if(found) {
                                break;
                            }
                        }
                        for(int h = (index - 1); h >= 0; h--) {
                            pathB.add(thisPathA[h]);
                        }
                        Path final;
                        for(int k = pathB.size() - 1; k >= 0; k--) {
                            final.add(pathB[k]);
                        }
                        return final;
                    }
                    for(RoadNode* i : graph.neighborsOf(lastNode)) {
                        if(!visitedNodesB.contains(i)) {
                            Path newPath = pathB;
                            i->setColor(Color::YELLOW);
                            newPath.add(i);
                            double Ppriority = graph.edgeBetween(lastNode,i)->cost() + actualPathCost + (getHueristic(graph,i,start));
                            queueB.enqueue(newPath,Ppriority); //enqueue the copy
                        }
                    }
                }
            }
        }
    }
    return {}; //If we reach the end of the function we know there isn't a possible path so we return an empty path
}

//Apple maps =)
Path dijkstrasAlgorithm(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    Set<RoadNode*> visitedNodes;                //Keeps a set of nodes we have already visited
    PriorityQueue<Path> queue;
    Path firstPath;
    firstPath.add(start);
    queue.enqueue(firstPath,-(getHueristic(graph,firstPath.get(firstPath.size() - 1),end)));            //We enqueue our first path
    while(!queue.isEmpty()) {                                           //While there are still nodes we can visit, we continue
        double originalPathCost = -(queue.peekPriority());               //Saves the cost of the path so we can have access to it's cost after we dequeue it
        Path path = queue.dequeue();
        //The line below removes the additional hueristic cost of the previous node, so we can have an unbiased path cost.
        double originalPriority = originalPathCost - (getHueristic(graph,path.get(path.size() - 1),end));
        RoadNode* lastPath = path.get(path.size() - 1);                     //Get the last node in the path
        if(!visitedNodes.contains(lastPath)) {                              //If we haven't already seen this node
            visitedNodes.add(lastPath);                                     //Mark this node has visited
            lastPath->setColor(Color::GREEN);                               //Highlight it as visited
            if(lastPath == end) {                                           //See if we have reached the destination
                return path;                //Ends search
            }
            for(RoadNode* i : graph.neighborsOf(lastPath)) { //Otherwise glance at a node's neighbors
                if(!visitedNodes.contains(i)) {             //Ensures that we don't highlight green nodes yellow
                    Path newPath = path;                    //Copy the current path we have
                    i->setColor(Color::YELLOW);             //Highlight that node as glanced at
                    newPath.add(i);                         //Add the node we are glancing at to the end of the copy of the path
                    //The line below updates the cost of the path (with the hueristic if whichAlgorithim deems it approprite)
                    double Ppriority = -(graph.edgeBetween(lastPath,i)->cost() + originalPriority + (getHueristic(graph,i,end)));
                    queue.add(newPath,Ppriority); //enqueue the copy
                }
            }
        }
    }
    return {}; //If we reach the end of the function we know there isn't a possible path so we return an empty path
}

Path aStar(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    return {};
}

Path alternativeRoute(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    return {};
}

