//  Copyright 2019 Tsung-Han lee
#include "../include/a_star.h"

std::vector<MapNode> getAvailableAdjacents(MapNode* const n, const MapNode& dest) {
    auto canidates = n->getAdjacents();
    /* check possibility */
    for (auto itr = canidates.begin(); itr != canidates.end(); ) {
        if (itr->getX() < 0 || itr->getY() < 0) {
            itr = canidates.erase(itr);
        } else {
            itr->updateCosts(dest);
            ++itr;
        }
    }

    return canidates;
}

int main(int argc, char* argv[]) {
    MapNode dest(5, 5);
    MapNode src(2, 2);
    src.updateCosts(dest);

    OpenList list;
    list.pushNode(src);
    std::cout << list << std::endl;

    auto first = list.popNode();
    std::cout << "1. " << first << std::endl << std::endl;

    auto canidates = getAvailableAdjacents(&first, dest);
    list.pushNodes(canidates);
    std::cout << list << std::endl;

    auto second = list.popNode();
    std::cout << "2. " << second << std::endl << std::endl;

    canidates = getAvailableAdjacents(&second, dest);
    list.pushNodes(canidates);
    std::cout << list << std::endl;

    auto third = list.popNode();
    std::cout << "3. " << third << std::endl << std::endl;

    canidates = getAvailableAdjacents(&third, dest);
    list.pushNodes(canidates);
    std::cout << list << std::endl;

    auto fourth = list.popNode();
    auto path = fourth.getPath();
    for (auto point : path) {
        std::cout << static_cast<Node>(point) << " -> ";
    }


/*
    std::cout << "-------------------------\n";
    MapNode newnode = canidates[7];
    newnode.setFCost(1);
    list.modifyNode(canidates[7], newnode);
    std::cout << list << std::endl;
*/

    return 0;
}
