#include "Stack.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h> 
#include <ctime>
#include <vector>
#include <chrono>

using namespace std;

struct cell{
	bool path_visited;
	bool visited;
    bool left;
    bool right;
    bool up;
    bool down;

	cell(): path_visited(0), visited(0),left(1),right(1),up(1),down(1){};
};

static int num_of_walls = 0; // must be MxN-1

// write a funtion to randomly pick one wall and knocks it down
// create an unvisited cell and update it
// when a wall gets knocked down the wall of effected cell also changes 
void knock_down(int rows, int cols, vector<vector<cell>> &maze ) {

	srand(static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count()));

	int visit_num = 0; // must be MxN
	

    // Start at cell (0,0)
    int row = 0;
    int col = 0;

    // Create a stack to keep track of the visited cells
	// first visisted cell is (0,0)
    Stack<pair<int, int>> visited_cells;
    visited_cells.push(make_pair(row, col));
    maze[rows - 1 - row][col].visited = true;

    // Generate the maze 
	while (!visited_cells.isEmpty()) {
        // Get the current cell
        pair<int, int> current_cell = visited_cells.top();
        visited_cells.pop();
        int row = current_cell.first;
        int col = current_cell.second;

	
		// Choose a random unvisited neighbor cell
		vector<pair<int, int>> neighbor_cells;
		if (row > 0 && !maze[rows - 1 - (row-1)][col].visited) {
			neighbor_cells.push_back(make_pair(row-1, col));  // down
		}
		if (row < rows-1 && !maze[rows - 1 - (row+1)][col].visited) {
			neighbor_cells.push_back(make_pair(row+1, col));  // up
		}
		if (col > 0 && !maze[rows - 1 - row][col-1].visited) {
			neighbor_cells.push_back(make_pair(row, col-1));  // left
		}
		if (col < cols-1 && !maze[rows - 1 - row][col+1].visited) {
			neighbor_cells.push_back(make_pair(row, col+1));  // right
		}

		if (!neighbor_cells.empty()) {
			// Choose a random neighbor cell
			pair<int, int> neighbor_cell = neighbor_cells[rand() % neighbor_cells.size()];

			// Remove the wall between the cells
			if (neighbor_cell.first < row) {
				maze[rows - 1 -row][col].down = false;
				maze[rows - 1 - neighbor_cell.first][neighbor_cell.second].up = false;
			}
			else if (neighbor_cell.first > row) {
				maze[rows - 1 -row][col].up = false;
				maze[rows - 1 - neighbor_cell.first][neighbor_cell.second].down = false;
			}
			else if (neighbor_cell.second < col) {
				maze[rows - 1 - row][col].left = false;
				maze[rows - 1 - neighbor_cell.first][neighbor_cell.second].right = false;
			}
			else if (neighbor_cell.second > col) {
				maze[rows - 1 - row][col].right = false;
				maze[rows - 1 - neighbor_cell.first][neighbor_cell.second].left = false;
			}

			// Add the new cell to the stack and mark it as visited
			visited_cells.push(make_pair(row, col)); // adding current again after pop it
			visited_cells.push(neighbor_cell); // also adding the new cell
			maze[rows - 1 - neighbor_cell.first][neighbor_cell.second].visited = true;
			visit_num++;
		}
		else {
			// If no wall exists to knock down for the current cell,
			// backtrack using the stack until you find a cell which has an unvisited neighbor cell
		while (!visited_cells.isEmpty()) {
			pair<int, int> backtracked_cell = visited_cells.topAndPop();
			row = backtracked_cell.first;
			col = backtracked_cell.second;
			vector<pair<int, int>> neighbor_cells;
			if (row > 0 && !maze[rows - 1 - (row-1)][col].visited) {
				neighbor_cells.push_back(make_pair(row-1, col));  // down
			}
			if (row < rows-1 && !maze[rows - 1 - (row+1)][col].visited) {
				neighbor_cells.push_back(make_pair(row+1, col));  // up
			}
			if (col > 0 && !maze[rows - 1 - row][col-1].visited) {
				neighbor_cells.push_back(make_pair(row, col-1));  // left
			}
			if (col < cols-1 && !maze[rows - 1 - row][col+1].visited) {
				neighbor_cells.push_back(make_pair(row, col+1));  // right
			}
			if (!neighbor_cells.empty()) {
				// Choose a random neighbor cell
				pair<int, int> neighbor_cell = neighbor_cells[rand() % neighbor_cells.size()];

				// Remove the wall between the cells
				if (neighbor_cell.first < row) {
					maze[rows - 1 -row][col].down = false;
					maze[rows - 1 - neighbor_cell.first][neighbor_cell.second].up = false;
				}
				else if (neighbor_cell.first > row) {
					maze[rows - 1 -row][col].up = false;
					maze[rows - 1 - neighbor_cell.first][neighbor_cell.second].down = false;
				}
				else if (neighbor_cell.second < col) {
					maze[rows - 1 - row][col].left = false;
					maze[rows - 1 - neighbor_cell.first][neighbor_cell.second].right = false;
				}
				else if (neighbor_cell.second > col) {
					maze[rows - 1 - row][col].right = false;
					maze[rows - 1 - neighbor_cell.first][neighbor_cell.second].left = false;
				}

				// Add the new cell to the stack and mark it as visited
				visited_cells.push(make_pair(row, col));
				visited_cells.push(neighbor_cell);
				maze[rows - 1 - neighbor_cell.first][neighbor_cell.second].visited = true;

				// Exit the loop
				break;
			}
		}

		}
    }
	
	num_of_walls++;
}

// writes maze's data into a file and name as maze_mazeID.txt
void maze_to_file(vector<vector<cell>> &maze, int num_of_maze) {
	string filename = "maze_" + to_string(num_of_maze) + ".txt";
	ofstream out(filename);
	int row =  maze.size()-1;

	// Writes maze dimensions
	out << maze.size() << " " << maze[0].size() << " ";  // y x -> row and col

	for (int i = 0; i < maze.size(); i++) {
		for (int j = 0; j < maze[0].size(); j++) {
			out << "\nx=" << j << " y=" << i // maze[i][j]
				<< " l=" <<  maze[row-i][j].left
				<< " r=" << maze[row-i][j].right
				<< " u=" <<  maze[row-i][j].up
				<< " d=" << maze[row-i][j].down << " ";
		}
	}

	out.close();
}

// writes maze's data into a file and name as maze_mazeID.txt
void path_to_file(vector<vector<cell>> &maze, int num_of_maze , 
				  int entry_x,int entry_y, int out_x, int out_y, vector<pair<int, int>>&track) {
					  // maze_2_path_0_0_3_4.txt
	string filename = "maze_" + to_string(num_of_maze) +  "_path_" + to_string(entry_x) + "_" + to_string(entry_y) + 
					  "_" + to_string(out_x) + "_" + to_string(out_y) + ".txt";

	ofstream out(filename);

	for (int i = 0; i < track.size(); i++)
	{
		out << track[i].second  << " " << track[i].first  << endl;
	}
	out.close();
}

// knock down wall till all the cells are visited 
void generate_maze_helper( int rows, int cols, vector<vector<cell>> &maze){
	// base condition
	if(num_of_walls < rows*cols - 1 ){ 
		knock_down(rows,cols,maze);
		generate_maze_helper(rows,cols,maze);
	}
	num_of_walls = 0;
}


void generate_maze(int row, int col, int num_of_maze, vector<vector<cell>> &maze ){
	//Using the top element in the stack to choose the next cell to be added to maze
	generate_maze_helper(row,col, maze);
	//print the information in the stack into a file
	maze_to_file(maze, num_of_maze);
}

void discover_path(int rows, int cols, vector<vector<vector<cell>>> &mazes) {
    // Ask the user which maze to choose
    int maze_num;
	cout << "Enter a maze ID between 1" <<" to "<< mazes.size() <<" inclusive to find a path: " ;
    cin >> maze_num;

	int entry_x, entry_y;
	int out_x,out_y;

	cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): " ;
	cin >> entry_x >> entry_y;
	cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
	cin >> out_x >> out_y;

	vector<vector<cell>> maze;
	maze = mazes[maze_num -1];

	// pair up the entry and out points
	 pair<int, int> entry_point, exit_point;
     entry_point = make_pair(entry_y, entry_x);
	 exit_point = make_pair(out_y,out_x);

	 // Stack that will keep the final path
	 vector<pair<int, int>> final_path ;
 
     // Start with an initially empty stack and push the entry cell to the stack
     Stack<pair<int, int>> path_stack;
     path_stack.push(entry_point);
	 // x and y
	 final_path.push_back(entry_point);

    // Use the top element in the stack to choose the next cell to visit
     while (!path_stack.isEmpty()) {
        // Get the current cell
        pair<int, int> current_cell = path_stack.top();
        int row = current_cell.first;
        int col = current_cell.second;

        // Check if the current cell is the exit point
        if (current_cell == exit_point) {
			path_to_file(maze, maze_num , entry_x, entry_y,  out_x,  out_y, final_path) ;
            return;
        }

        // Mark the current cell as visited
        maze[rows - 1 - row][col].path_visited = true;

        // Choose the next unvisited neighbor cell
        vector<pair<int, int>> neighbor_cells;
        if (row > 0 && !maze[rows - 1 - (row-1)][col].path_visited && !maze[rows - 1 -row][col].down) {
            neighbor_cells.push_back(make_pair(row-1, col));  
        }
        if (row < rows-1 && !maze[rows - 1 -(row+1)][col].path_visited && !maze[rows - 1 -row][col].up) {
            neighbor_cells.push_back(make_pair(row+1, col));   
        }
        if (col > 0 && !maze[rows - 1 -row][col-1].path_visited && !maze[rows - 1 -row][col].left) {
            neighbor_cells.push_back(make_pair(row, col-1));   
        }
        if (col < cols-1 && !maze[rows - 1 -row][col+1].path_visited && !maze[rows - 1 -row][col].right) {
            neighbor_cells.push_back(make_pair(row, col+1));    
        }

        if (!neighbor_cells.empty()) {
            // Choose a random neighbor cell
            pair<int, int> neighbor_cell = neighbor_cells[rand() % neighbor_cells.size()];
            // Add the new cell to the stack and mark it as visited
            path_stack.push(neighbor_cell);
			final_path.push_back(neighbor_cell);
            maze[rows - 1 - neighbor_cell.first][neighbor_cell.second].path_visited = true;
        } else {
            // If no unvisited neighbor cell exists for the current cell,
            // backtrack using the stack until you find an unvisited cell to go
            path_stack.pop();
			final_path.pop_back();
        }
		
    }
}

int main(){

	int maze_num = 0;
	int rows = 0, cols = 0;

	cout << "Enter the number of mazes: ";
	cin >> maze_num;
	cout << "Enter the number of rows and columns (M and N): ";
	cin >> rows >> cols;

	// generate 3D vector to keep the matrix in a vector too
	vector<vector<vector<cell>>> mazes;

	// generates mazes and writes them into seperate files as maze_mazeID.txt
	for (int i = 1; i <= maze_num; i++)
	{
		mazes.push_back(vector<vector<cell>>(rows, vector<cell>(cols)));
		generate_maze( rows,  cols,  i, mazes[i-1]);
	
	}

	cout << "All mazes are generated." << endl;
	discover_path(rows, cols, mazes);

	return 0;
}