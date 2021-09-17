#include "logic.h"

#define INFO(X) cout << "[INFO] ("<<__FUNCTION__<<":"<<__LINE__<<") " << #X << " = " << X << endl;
#define INFO_STRUCT(X) cout << "[INFO] ("<<__FUNCTION__<<":"<<__LINE__<<") " << #X << " = {" << X.row << "," << X.col << "," << X.treasure << "}" << endl;

/**
 * This function is provided; do not change
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  2D dynamic array representation of dungeon map with player's location.
 */
char** loadLevel(const string fileName, int& maxRow, int& maxCol, Player& player) {
    //INFO(maxRow);
    //INFO(maxCol);

    ifstream ifs(fileName);
    if (!ifs.is_open()) {
        cout << "ERROR: Unable to open: " << fileName << endl;
        return nullptr;
    }
    ifs >> maxRow >> maxCol;
    ifs >> player.row >> player.col;

    char** dungeon = createMap(maxRow,maxCol);
    if (dungeon == nullptr) {
        cout << "ERROR: Map did not load." << endl;
        return nullptr;
    }

    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            ifs >> dungeon[i][j];
            if (i == player.row && j == player.col) {
                dungeon[i][j] = TILE_PLAYER;
            }
        }
    }

    return dungeon;
}

/**
 * This function is provided; do not change
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    switch(input) {
        case MOVE_UP:
            nextRow--;
            break;
        case MOVE_DOWN:
            nextRow++;
            break;
        case MOVE_LEFT:
            nextCol--;
            break;
        case MOVE_RIGHT:
            nextCol++;
            break;
    }
}

/**
 * TODO: Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    //INFO(maxRow);
    //INFO(maxCol);

    // create an array of pointers on a heap with size of max rows
    char** map = new char*[maxRow];
    for (int row = 0; row < maxRow; ++row) {
        // for each index, assign it to an array of size col
        map[row] = new char[maxCol];

        // initilize each of the col values for this row
        for (int col = 0; col < maxCol; ++col) {
            map[row][col] = TILE_OPEN;
        }

    }

    // return a pointer to the 2D array
    return map;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 */
void deleteMap(char** map, int maxRow) {
    //INFO(map);
    //INFO(maxRow);

    // iterate and delete each of the arrays assigned to each row
    for (int row = 0; row < maxRow; ++row) {
        delete [] map[row];
    }

    // now delete the entire map
    delete [] map;

    return;
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can call your createMap and deleteMap functions to help with this one!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  An updated 2D map that has twice as many columns and rows in size.
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    //INFO(map);
    //INFO(maxRow);
    //INFO(maxCol);

    // create a new map with increase size 
    maxRow = maxRow * 2;
    maxCol = maxCol * 2;
    char** newMap = createMap(maxRow, maxCol);

    // copy over quadrant A
    for (int row = 0; row < (maxRow/2); ++row) {
        for (int col = 0; col < (maxCol/2); ++col) {
            newMap[row][col] = map[row][col];
        }
    }

    // copy over quadrant B
    int ogRow = 0;
    int ogCol = 0;
    for (int row = 0; row < (maxRow/2); ++row) { // from 0 -> (maxRow/2)
        ogCol = 0;
        for (int col = (maxCol/2); col < maxCol; ++col) { // from (maxCol/2) -> maxCol
            if (map[ogRow][ogCol] == TILE_PLAYER) {
                newMap[row][col] = TILE_OPEN;
            } else {
                newMap[row][col] = map[ogRow][ogCol];
            }
            ogCol ++; // iterate origial Col
        }
        ogRow ++; // iterate orignal Row
    }

    // copy over quadrant C
    ogRow = 0;
    ogCol = 0;
    for (int row = (maxRow/2); row < maxRow; ++row) { // from (maxRow/2) -> maxRow
        ogCol = 0;
        for (int col = 0; col < (maxCol/2); ++col) { // from 0 -> (maxCol/2)
            
            if (map[ogRow][ogCol] == TILE_PLAYER) {
                newMap[row][col] = TILE_OPEN;
            } else {
                newMap[row][col] = map[ogRow][ogCol];
            }
            ogCol ++; // iterate origial Col
        }
        ogRow ++; // iterate orignal Row
    }

    // copy over quadrant D
    ogRow = 0;
    ogCol = 0;
    for (int row = (maxRow/2); row < maxRow; ++row) { // from (maxRow/2) -> maxRow
        ogCol = 0;
        for (int col = (maxCol/2); col < maxCol; ++col) { // from (maxCol/2) -> maxCol
            
            if (map[ogRow][ogCol] == TILE_PLAYER) {
                newMap[row][col] = TILE_OPEN;
            } else {
                newMap[row][col] = map[ogRow][ogCol];
            }
            ogCol ++; // iterate origial Col
        }
        ogRow ++; // iterate orignal Row
    }

    // new array has been created entirly

    // delete the old array
    deleteMap(map, maxRow/2);
    
    // return the new array
    return newMap;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int& nextRow, int& nextCol) {
    //INFO(map);
    //INFO(maxRow);
    //INFO(maxCol);
    //INFO_STRUCT(player);
    //INFO(nextRow);
    //INFO(nextCol);

    // status by default
    int status = STATUS_MOVE;

    // within bounds check
    if (nextRow < 0 || nextRow >= maxRow) { // out of row bounds

        // update nextRow and col
        nextRow = player.row;
        nextCol = player.col;

        status = STATUS_STAY;
    }

    if (nextCol < 0 || nextCol >= maxCol) { // out of col bounds

        // update nextRow and Col
        nextRow = player.row;
        nextCol = player.col;

        status = STATUS_STAY;
    }

    // tile check
    char tile = map[nextRow][nextCol];

    // unpassable check
    if (tile == TILE_PILLAR || tile == TILE_MONSTER) {
        
        // make nextRow & nextCol = the player's current position
        nextRow = player.row;
        nextCol = player.col;

        status = STATUS_STAY;
    }

    // treasure check
    if (tile == TILE_TREASURE) {
        // incremenet treasure count
        player.treasure ++;

        // return status
        status = STATUS_TREASURE;
    }

    // amulet check
    if (tile == TILE_AMULET) {

        // return status
        status = STATUS_AMULET;
    }

    if (tile == TILE_DOOR) {

        // return status
        status = STATUS_LEAVE;
    }

    if (tile == TILE_EXIT) {
        if (player.treasure > 0) {

            // retrn status
            status = STATUS_ESCAPE;

        } else { // if they dont have enough treasure
                
            // update next Row
            nextRow = player.row;
            nextCol = player.col;

            // return status 
            status = STATUS_STAY;

        }
    }




    // update the old position of player on map
    map[player.row][player.col] = TILE_OPEN;

    // update the new positon of player on map
    map[nextRow][nextCol] = TILE_PLAYER; 

    // update the player struct position
    player.row = nextRow;
    player.col = nextCol;

    // return status
    return status;




}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, Player& player) {
    //INFO(map);
    //INFO(maxRow);
    //INFO(maxCol);
    //INFO_STRUCT(player);
    
    char tile = TILE_OPEN;
    int row = 0;
    int col = 0;

    // player postition mod
    int upMod = 1;
    int downMod = 1;
    int rightMod = 1;
    int leftMod = 1;


    // prevent over indexing
    if (player.row == 0) {
        upMod = 0;
    }

    if (player.row == maxRow) {
        downMod = 0;
    }

    if (player.col == 0) {
        leftMod = 0;
    }

    if (player.col == maxCol) {
        rightMod = 0;
    }



    // check up
    for (row = (player.row - upMod); row >= 0; row --) { // from (player.row - 1) -> top of map
        col = player.col;
        tile = map[row][col];

        if (tile == TILE_PILLAR) {
            // monster cannot see 
            // so there is nothing on top
            break; // exit loop
        } 

        if (tile == TILE_MONSTER) {

            // THERES A MONSTER! -- move him now

            // update original position of monster
            map[row][col] = TILE_OPEN;

            // update the new position of monster
            map[row + 1][col] = TILE_MONSTER; // ----> he moved down one block here

            // continue checking for more monsters
        }


    }

    // check down
    for (row = (player.row + downMod); row < maxRow; row ++) { // from (player.row + 1) -> bottom of map
        col = player.col;
        tile = map[row][col];

        if (tile == TILE_PILLAR) {
            // monster cannot see 
            // so there is nothing on top
            break; // exit loop
        } 

        if (tile == TILE_MONSTER) {

            // THERES A MONSTER! -- move him now

            // update original position of monster
            map[row][col] = TILE_OPEN;

            // update the new position of monster
            map[row - 1][col] = TILE_MONSTER; // ----> he moved up one block here


            // continue checking for more monsters
        }
    }

    // check left
    for (col = (player.col - leftMod); col >= 0; col --) { // from (player.col - 1) -> left of map
        row = player.row;
        tile = map[row][col];

        if (tile == TILE_PILLAR) {
            // monster cannot see 
            // so there is nothing on top
            break; // exit loop
        } 

        if (tile == TILE_MONSTER) {

            // THERES A MONSTER! -- move him now

            // update original position of monster
            map[row][col] = TILE_OPEN;

            // update the new position of monster
            map[row][col + 1] = TILE_MONSTER; // ----> he moved right one block here



            // continue checking for more monsters
        }
    }

    // check right
    for (col = (player.col + rightMod); col < maxCol; col ++) { // from (player.col + 1) -> right of map
        row = player.row;
        tile = map[row][col];

        if (tile == TILE_PILLAR) {
            // monster cannot see 
            // so there is nothing on top
            break; // exit loop
        } 

        if (tile == TILE_MONSTER) {

            // THERES A MONSTER! -- move him now

            // update original position of monster
            map[row][col] = TILE_OPEN;

            // update the new position of monster
            map[row][col - 1] = TILE_MONSTER; // ----> he moved left one block here

            // continue checking for more monsters
        }
    }


    // check if any monster is on player
    if (map[player.row][player.col] == TILE_MONSTER) {
        return true;
        // MONSTER GOT US !! OH NOOO
    }



    return false;
}
