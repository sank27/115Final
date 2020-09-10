# Project Definition 
The project is a 2D application where the user selects a character in the environment and a target location. The view is from the top, where a map is displayed. The map is represented by a matrix where each cell contains an image corresponding to a type of terrain.

The character will move cell by cell towards the target location by using the shortest path. 
There are multiple types of characters who are present on the map:The human who can just walk and not go in the water or cross the high mountains. The bird that can fly over mountains but that cannot go in the desert. The fish that must stay in the water. The frog that can go everywhere except on the mountains and the desert

Rule1:The human, bird, fish, and frog cannot be on the same cell on the map. We consider the environment represented as a matrix.

Rule 2:In relation to the terrain, the character can take more time to move to one vertex to another vertex in the graph.

Rule 3:The terrain should include special features such as tunnels in the mountains.The terrain should be created randomly by using a procedural function to generate the terrain with quadtrees. The terrain (the map) can be loaded from a text file where the characters represent the different elements to be displayed on the map.

The environment contains different types of cells:high mountain, middle mountain, foothills, plain, forest, desert, beach, and ocean.


# Progress 
- Added shortest path algorithm 
- When user clicks on a grid, the shortest path to that grid is calculated
- Human walks to selected location on the grid by avoiding ocean & high mountain
- Click on any other character to make it the Active Character 
- User can then click on any grid and make it move 
- Added check so that no action can be taken while one action is in progress
- User can choose to generate map (and then choose levels) or user can choose to load last played map 
