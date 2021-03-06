#include <iostream>
#include <fstream> //File input and output
#include <cstdlib> //Random method
#include <string>  //String class 
#include <unistd.h> //Sleep function
#include "Board.h"

using namespace std;

Board::Board()
{

}

Board::~Board()
{

}

void Board::setDimensions(int& rows, int& columns, double& density)  //Sets the rows and columns for the board.
{
	cout << "Enter the amount of rows: ";
	cin >> rows;
	while(rows <= 1 || cin.fail()){
		if(cin.fail()){
			cin.clear();
			cin.ignore(256, '\n');
		}
		cout << "Please enter a number greater than 1: ";
		cin >> rows;
	}
	cout << "Enter the amount of columns: ";
	cin >> columns;
	
	while(columns <= 1 || cin.fail()){
		if(cin.fail()){
			cin.clear();
			cin.ignore(256, '\n');
		}
		cout << "Please enter a number greater than 1: ";
		cin >> columns;
	}
	cout << "Enter the density of the board. Number must be between 0 and 1: ";
	cin >> density;
	while(density < 0 || density > 1 || cin.fail()){
		if(cin.fail()){
			cin.clear();
			cin.ignore(256, '\n');
		}
		cout << "Please enter a valid number between 0 and 1: ";
		cin >> density;
	}
}

void Board::setDimensions(string file, int& rows, int& columns)  //Reads in the first two ints of the mapfile and sets them.
{
	ifstream inputStream;
	inputStream.open(file.c_str());
	inputStream >> rows >> columns;
	inputStream.close();
}

void Board::initializeBoard(int rows, int columns, double density, char**& board) //Initializes the board with the set population density. 
{ 
	for(int i = 0; i < rows; ++i){
		for(int j = 0; j < columns; ++j){board[i][j] = '-';}
	}
	for(int i = 0; i < rows; ++i){
		for(int j = 0; j < columns; ++j){
			double a = 0;
			a = (double)rand()/(RAND_MAX);
			if(a<=density) board[i][j] = 'X';
			else board[i][j] = '-';
		}
	}
}

void Board::initializeBoard(string file, char**& board)  //Iterates through each line of text and sets each row accordingly.
{
	int x, y;
	char text;
	ifstream inputStream;
	inputStream.open(file.c_str());
	inputStream >> x >> y;
	for(int i = 0; i < x; ++i){
		for(int j = 0; j < y; ++j){
			inputStream >> text;
			board[i][j] = text;
		}
	}
	inputStream.close();
}

void Board::prepGame(string& file, int& rows, int& columns) //Method for running the entire game.
{
	int input;
	cout << "This program simulates John Conway's Game of Life.\n";
	cout << "You can initialize your simulation from a random assignment, or a txt file.\n";
	cout << "0 - Random Assignment \t 1 - Map Assignment\n";
	cout << "Please enter the number of your desired option: ";
	cin >> input;

	while(input < 0 || input > 1 || cin.fail())
	{	//Checking for incorrect input
		if(cin.fail())
		{
			cin.clear();
			cin.ignore(256, '\n');
		}
		cout << "Please enter a valid number listed above: ";
		cin >> input;
	}
	
	if(input == 0) //Initiates the creation of a board based on given conditions.
	{
		double density;
		setDimensions(rows, columns, density);
		char** currentGen = new char*[rows];
		for(int i = 0; i < rows; ++i) {currentGen[i] = new char[columns];}
		initializeBoard(rows, columns, density, currentGen);
		startGame(rows, columns, currentGen);
	}
	else if(input == 1) //Initiates the read in of a designated file.
	{
		cout <<"Please enter the filename of your map: ";
		cin >> file;	setDimensions(file.c_str(), rows, columns);
		char** currentGen = new char*[rows];
		for(int i = 0; i < rows; ++i) {currentGen[i] = new char[columns];}
		initializeBoard(file.c_str(), currentGen);
		startGame(rows, columns, currentGen);
	}
}

void Board::startGame(int& rows, int& columns, char**& board)
{
	int input, output;
	cout << "Which mode would you like to play?\n" <<
	"1 - Classic Mode\t 2 - Torus Mode\t 3 - Mirror Mode\n" <<
	"Enter the number of your desired mode: "; cin >> input;
	while(input < 1 || input > 3 || cin.fail())
	{
		if(cin.fail()){
			cin.clear();
			cin.ignore(256, '\n');
		}
		cout << "Please enter a valid number listed above: ";
		cin >> input;	
	}
	cout << "How would you like to like the program to proceed?\n"
	<< "1 - Let the program run the simulation on its own\n"
	<< "2 - Press enter to output each consecutive generation\n"
	<< "3 - Output the simulation to a file, 'GameOfLife.txt'\n" << "Enter input: ";
	cin >> output;
	while(output < 1 || output > 3 || cin.fail())
	{
		if(cin.fail())
		{
			cin.clear();
			cin.ignore(256, '\n');
		}
		cout << "Please enter a valid number listed above: ";
		cin >> input;	
	}
	if(output == 1 || output == 2)
	{
		cout << " Generation 0" << endl;
		printBoard(rows, columns, board);
		int stabilized = 0, gen = 1;
		cout << "Press enter to start the simulation: ";
		cin.ignore();	cin.ignore();
		while(stabilized == 0) //Sets the while loop using an indicator for whether the board has stabilized.
		{
			if(input == 1)stabilized = nextBoardClassic(rows, columns, board); //Integrated method for creating new generations returns a value indicating stabilization.
			else if(input == 2)stabilized = nextBoardTorus(rows, columns, board);
			else if(input == 3)stabilized = nextBoardMirrored(rows, columns, board);
	
			if(stabilized == 1)
			{
				cout << "Press enter to exit the program." << endl;
				cin.ignore();
				break;
			}
	
			cout << "Generation " << gen << endl;
			printBoard(rows, columns, board);
	
			gen++;
			if(output==1) sleep(1.5);
			else if(output == 2){
				cout << "Press enter to see the next generation: " << endl;
				cin.ignore();
			}
		}
	}
	else if(output == 3){
		ofstream outFile("GameOfLife.txt");
		int stabilized = 0, count = 1, gen = 1;

		while(stabilized == 0)
		{
			if(input == 1)stabilized = nextBoardClassic(rows, columns, board); 
			else if(input == 2)stabilized = nextBoardTorus(rows, columns, board);
			else if(input == 3)stabilized = nextBoardMirrored(rows, columns, board);
		
			if(stabilized == 1 || count > 100)	//loop continues until either stabilization or 100th generation is reached
			{					//to prevent infinite loop caused by oscillators
				cout << "File successfully printed out\n";
				cout << "Press enter to exit the program." << endl;
				cin.ignore();
				break;
			}
			outFile << "Generation " << gen << endl;
			printBoardFile(rows, columns, board, outFile);
			gen++;	count++;
		}
	}
}

void Board::printBoard(int rows, int columns, char** board)
{
	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < columns; ++j) cout << board[i][j];
		cout << endl;
	}
}

void Board::printBoardFile(int rows, int columns, char** board, ofstream& file) //print out current board to output file
{	
	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < columns; ++j) file << board[i][j];
		file << endl;
	}
}

int Board::nextBoardClassic(int rows, int columns, char**& board) //Creates a new board with adjustments for next generation, then copies it to the primary board.
{
	char** nextGen = new char*[rows]; //Creates a second board based on first board dimensions for copying later.
	for(int i = 0; i < rows; ++i) nextGen[i] = new char[columns];

	//Creating the next generation based on the previous.

	for(int i = 0; i < rows; ++i){
		for(int j = 0; j < columns; ++j){
			int count = 0; 								//Keeps track of amount of neighbors.

			if(i == 0 && j == 0){ 						//All checks necessary to avoid IndexOutOfBounds error.
				if(board[i+1][j] == 'X') count++; 		//Only 3 possible neighbors for corner slots in classic mode.
				if(board[i][j+1] == 'X') count++;
				if(board[i+1][j+1] == 'X') count++;
			}
			else if(i == 0 && j == (columns - 1)){
				if(board[i+1][j] == 'X') count++;
				if(board[i][j-1] == 'X') count++;
				if(board[i+1][j-1] == 'X') count++;
			}
			else if(i == (rows - 1 ) && j == (columns - 1)){
				if(board[i-1][j] == 'X') count++;
				if(board[i][j-1] == 'X') count++;
				if(board[i-1][j-1] == 'X') count++;
			}
			else if(i == (rows - 1) && j == 0){
				if(board[i-1][j] == 'X') count++;
				if(board[i][j+1] == 'X') count++;
				if(board[i-1][j+1] == 'X') count++;
			}
			else if(j == 0){ 							//Only 5 possible neighbors for side slots in classic mode.
				if(board[i-1][j] == 'X') count++;
				if(board[i+1][j] == 'X') count++;
				if(board[i][j+1] == 'X') count++;
				if(board[i+1][j+1] == 'X') count++;
				if(board[i-1][j+1] == 'X') count++;
			}

			else if(i == 0){
				if(board[i+1][j] == 'X') count++;
				if(board[i][j+1] == 'X') count++;
				if(board[i+1][j+1] == 'X') count++;
				if(board[i][j-1] =='X') count++;
				if(board[i+1][j-1] == 'X') count++;
			}
			else if(j == (columns - 1)){
				if(board[i-1][j] == 'X') count++;
				if(board[i+1][j] == 'X') count++;
				if(board[i][j-1] == 'X') count++;
				if(board[i+1][j-1] == 'X') count++;
				if(board[i-1][j-1] == 'X') count++;
			}
			else if(i == (rows - 1)){
				if(board[i-1][j] == 'X') count++;
				if(board[i][j+1] == 'X') count++;
				if(board[i-1][j+1] == 'X') count++;
				if(board[i][j-1] =='X') count++;
				if(board[i-1][j-1] == 'X') count++;
			}
			else 										//Eight possible neighbors for any given middle slot.
			{
				if(board[i+1][j] == 'X') count++;
				if(board[i][j+1] == 'X') count++;
				if(board[i+1][j+1] == 'X') count++;
				if(board[i][j-1] =='X') count++;
				if(board[i-1][j] == 'X') count++;
				if(board[i-1][j-1] == 'X') count++;
				if(board[i+1][j-1] =='X') count++;
				if(board[i-1][j+1] == 'X') count++;
			}

			//Changes the next generation's population based on neighbors.
			if(count < 2) nextGen[i][j] = '-';
			else if(count == 2) nextGen[i][j] = board[i][j];
			else if(count == 3) nextGen[i][j] = 'X';
			else if(count > 3) nextGen[i][j] = '-';
		}
	}

	//Checking to see if the next generation is the same as the previous, returns '1' if true, '0' if false.
	bool stabilized = true;

	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < columns; ++j)
		{
			if(nextGen[i][j] != board[i][j]) stabilized = false;
		}
	}

	if(stabilized)
	{
		cout << "The world has stabilized. There are no further changes to the population." << endl;
		return 1;
	}
	else //Copies the next generation over to the primary board.
	{ 
		for(int i = 0; i < rows; ++i)
		{
			for(int j = 0; j < columns; ++j)board[i][j] = nextGen[i][j];
		}
		return 0;
	}
}

int Board::nextBoardTorus(int rows, int columns, char**& board) //Creates a new board with adjustments for next generation, then copies it to the primary board.
{
	char** nextGen = new char*[rows]; //Creates a second board based on first board dimensions for copying later.
	for(int i = 0; i < rows; ++i) nextGen[i] = new char[columns];

	//Creating the next generation based on the previous.

	for(int i = 0; i < rows; ++i){
		for(int j = 0; j < columns; ++j){
			int count = 0; 								//Keeps track of amount of neighbors.

			if(i == 0 && j == 0){ 						//All checks necessary to avoid IndexOutOfBounds error.
				if(board[i+1][j] == 'X') count++; 		
				if(board[i][j+1] == 'X') count++;
				if(board[i+1][j+1] == 'X') count++;
				if(board[rows-1][j] == 'X') count++;	//Begins checking for off grid areas using torus wrapping.
				if(board[rows-1][j+1] == 'X') count++;
				if(board[rows-1][columns-1] == 'X') count++;
				if(board[i][columns-1] == 'X') count++;
				if(board[i+1][columns-1] == 'X') count++;
			}
			else if(i == 0 && j == (columns - 1)){
				if(board[i+1][j] == 'X') count++;
				if(board[i][j-1] == 'X') count++;
				if(board[i+1][j-1] == 'X') count++;
				if(board[rows-1][j] == 'X') count++;
				if(board[rows-1][j-1] == 'X') count++;
				if(board[rows-1][0] == 'X') count++;
				if(board[i][0] == 'X') count++;
				if(board[i+1][0] == 'X') count++;
			}
			else if(i == (rows - 1 ) && j == (columns - 1)){
				if(board[i-1][j] == 'X') count++;
				if(board[i][j-1] == 'X') count++;
				if(board[i-1][j-1] == 'X') count++;
				if(board[0][j-1] == 'X') count++;
				if(board[0][j] == 'X') count++;
				if(board[0][0] == 'X') count++;
				if(board[i][0] == 'X') count++;
				if(board[i-1][0] == 'X') count++;
			}
			else if(i == (rows - 1) && j == 0){
				if(board[i-1][j] == 'X') count++;
				if(board[i][j+1] == 'X') count++;
				if(board[i-1][j+1] == 'X') count++;
				if(board[0][j] == 'X') count++;
				if(board[0][j+1] == 'X') count++;
				if(board[0][columns-1] == 'X') count++;
				if(board[i-1][columns-1] == 'X') count++;
				if(board[i][columns-1] == 'X') count++;
			}
			else if(j == 0){ 							
				if(board[i-1][j] == 'X') count++;
				if(board[i+1][j] == 'X') count++;
				if(board[i][j+1] == 'X') count++;
				if(board[i+1][j+1] == 'X') count++;
				if(board[i-1][j+1] == 'X') count++;
				if(board[i-1][columns-1] == 'X') count++;
				if(board[i][columns-1] == 'X') count++;
				if(board[i+1][columns-1] == 'X') count++;
			}

			else if(i == 0){
				if(board[i+1][j] == 'X') count++;
				if(board[i][j+1] == 'X') count++;
				if(board[i+1][j+1] == 'X') count++;
				if(board[i][j-1] =='X') count++;
				if(board[i+1][j-1] == 'X') count++;
				if(board[rows-1][j-1] == 'X') count++;
				if(board[rows-1][j] == 'X') count++;
				if(board[rows-1][j+1] == 'X') count++;
			}
			else if(j == (columns - 1)){
				if(board[i-1][j] == 'X') count++;
				if(board[i+1][j] == 'X') count++;
				if(board[i][j-1] == 'X') count++;
				if(board[i+1][j-1] == 'X') count++;
				if(board[i-1][j-1] == 'X') count++;
				if(board[i-1][0] == 'X') count++;
				if(board[i][0] == 'X') count++;
				if(board[i+1][0] == 'X') count++;
			}
			else if(i == (rows - 1)){
				if(board[i-1][j] == 'X') count++;
				if(board[i][j+1] == 'X') count++;
				if(board[i-1][j+1] == 'X') count++;
				if(board[i][j-1] =='X') count++;
				if(board[i-1][j-1] == 'X') count++;
				if(board[0][j-1] == 'X') count++;
				if(board[0][j] == 'X') count++;
				if(board[0][j+1] == 'X') count++;
			}
			else 										
			{
				if(board[i+1][j] == 'X') count++;
				if(board[i][j+1] == 'X') count++;
				if(board[i+1][j+1] == 'X') count++;
				if(board[i][j-1] =='X') count++;
				if(board[i-1][j] == 'X') count++;
				if(board[i-1][j-1] == 'X') count++;
				if(board[i+1][j-1] =='X') count++;
				if(board[i-1][j+1] == 'X') count++;
			}

			//Changes the next generation's population based on neighbors.
			if(count < 2) nextGen[i][j] = '-';
			else if(count == 2) nextGen[i][j] = board[i][j];
			else if(count == 3) nextGen[i][j] = 'X';
			else if(count > 3) nextGen[i][j] = '-';
		}
	}

	//Checking to see if the next generation is the same as the previous, returns '1' if true, '0' if false.
	bool stabilized = true;

	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < columns; ++j)
		{
			if(nextGen[i][j] != board[i][j]) stabilized = false;
		}
	}

	if(stabilized)
	{
		cout << "The world has stabilized. There are no further changes to the population." << endl;
		return 1;
	}
	else //Copies the next generation over to the primary board.
	{ 
		for(int i = 0; i < rows; ++i)
		{
			for(int j = 0; j < columns; ++j)board[i][j] = nextGen[i][j];
		}
		return 0;
	}
}

int Board::nextBoardMirrored(int rows, int columns, char**& board) //Creates a new board with adjustments for next generation, then copies it to the primary board.
{
	char** nextGen = new char*[rows]; //Creates a second board based on first board dimensions for copying later.
	for(int i = 0; i < rows; ++i) nextGen[i] = new char[columns];

	//Creating the next generation based on the previous.

	for(int i = 0; i < rows; ++i){
		for(int j = 0; j < columns; ++j){
			int count = 0; 								//Keeps track of amount of neighbors.

			if(i == 0 && j == 0){ 						//All checks necessary to avoid IndexOutOfBounds error.
				if(board[i+1][j] == 'X') count+=2; 		
				if(board[i][j+1] == 'X') count+=2;
				if(board[i+1][j+1] == 'X') count++;
				if(board[i][j] == 'X') count+=3;	//Begins checking for off grid areas using torus wrapping.
			}
			else if(i == 0 && j == (columns - 1)){
				if(board[i+1][j] == 'X') count+=2;
				if(board[i][j-1] == 'X') count+=2;
				if(board[i+1][j-1] == 'X') count++;
				if(board[i][j] == 'X') count+=3;
			}
			else if(i == (rows - 1 ) && j == (columns - 1)){
				if(board[i-1][j] == 'X') count+=2;
				if(board[i][j-1] == 'X') count+=2;
				if(board[i-1][j-1] == 'X') count++;
				if(board[i][j] == 'X') count+=3;
			}
			else if(i == (rows - 1) && j == 0){
				if(board[i-1][j] == 'X') count+=2;
				if(board[i][j+1] == 'X') count+=2;
				if(board[i-1][j+1] == 'X') count++;
				if(board[i][j] == 'X') count+=3;
			}
			else if(j == 0){ 							
				if(board[i-1][j] == 'X') count+=2;
				if(board[i+1][j] == 'X') count+=2;
				if(board[i][j+1] == 'X') count++;
				if(board[i+1][j+1] == 'X') count++;
				if(board[i-1][j+1] == 'X') count++;
				if(board[i][j] == 'X') count++;
			}

			else if(i == 0){
				if(board[i+1][j] == 'X') count++;
				if(board[i][j+1] == 'X') count+=2;
				if(board[i+1][j+1] == 'X') count++;
				if(board[i][j-1] =='X') count+=2;
				if(board[i+1][j-1] == 'X') count++;
				if(board[i][j] == 'X') count++;
			}
			else if(j == (columns - 1)){
				if(board[i-1][j] == 'X') count+=2;
				if(board[i+1][j] == 'X') count+=2;
				if(board[i][j-1] == 'X') count++;
				if(board[i+1][j-1] == 'X') count++;
				if(board[i-1][j-1] == 'X') count++;
				if(board[i][j] == 'X') count++;
			}
			else if(i == (rows - 1)){
				if(board[i-1][j] == 'X') count++;
				if(board[i][j+1] == 'X') count+=2;
				if(board[i-1][j+1] == 'X') count++;
				if(board[i][j-1] =='X') count+=2;
				if(board[i-1][j-1] == 'X') count++;
				if(board[i][j] == 'X') count++;
			}
			else 										
			{
				if(board[i+1][j] == 'X') count++;
				if(board[i][j+1] == 'X') count++;
				if(board[i+1][j+1] == 'X') count++;
				if(board[i][j-1] =='X') count++;
				if(board[i-1][j] == 'X') count++;
				if(board[i-1][j-1] == 'X') count++;
				if(board[i+1][j-1] =='X') count++;
				if(board[i-1][j+1] == 'X') count++;
			}

			//Changes the next generation's population based on neighbors.
			if(count < 2) nextGen[i][j] = '-';
			else if(count == 2) nextGen[i][j] = board[i][j];
			else if(count == 3) nextGen[i][j] = 'X';
			else if(count > 3) nextGen[i][j] = '-';
		}
	}

	//Checking to see if the next generation is the same as the previous, returns '1' if true, '0' if false.
	bool stabilized = true;

	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < columns; ++j)
		{
			if(nextGen[i][j] != board[i][j]) stabilized = false;
		}
	}

	if(stabilized)
	{
		cout << "The world has stabilized. There are no further changes to the population." << endl;
		return 1;
	}
	else //Copies the next generation over to the primary board.
	{ 
		for(int i = 0; i < rows; ++i)
		{
			for(int j = 0; j < columns; ++j)board[i][j] = nextGen[i][j];
		}
		return 0;
	}
}
