#include <iostream>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

struct termios orig_termios;

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);
	
	struct termios raw = orig_termios;
	raw.c_lflag &= ~(ECHO | ICANON | ISIG );
	
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void printBoard( string board[9] ) {
	cout << "_________\n";
	for (int i = 0; i < 9; i++) {
		cout << "|" << board[i] << "|";
		if ( (i + 1)% 3 == 0){
			cout << endl;
		};
	};
	cout << "---------\n";
};

bool validPlay(string board[9], bool xTurn, string play){
	string player;
	if(xTurn){
		player = "x";
	} else {
		player = "o";
	};	

	struct {
	bool valid = false;
	int index;
	} validMove;

	
	for (int i = 0; i <= 9; i++){
		if (board[i] == play){ 
			validMove.valid = true;
			validMove.index = i;
			break;
		};	
	};
	if(!validMove.valid){
		cout << "invalid move plz try again\n";
		printBoard(board);
		return false;
	};
	board[validMove.index] = player;
	return true;
};

bool checkBoard(string board[9]){
	bool over;

	string x;
	// check horizontal
	for (int i = 0; i < 3; i += 3){
		if(board[i] == board[i+1] && board[i+1] == board[i+2]){
			return true; 
		};
	};
	// check vertical
	for (int i = 0; i < 3; i++){
		if(board[i] == board[i+3] && board[i+3] == board[i+6]){
			return true;
		};
	};
	
	// check diagonal
	if (board[0] == board[4] && board[4] == board[8]){
		return true;
	};
	if (board[2] == board[4] && board[4] == board[6]){
		return true;
	};

	return false;
};

int main() {
	enableRawMode();
	system("clear");

	string board[9];
	bool xTurn = true;

	for (int i = 0; i < 9; i++) {
		board[i] = std::to_string(i + 1);
	};

	cout << "Welcome to Tic Tac Toe!\nPress q to quit and s to start\n";

	char c;
	bool start = false;
	bool notFirst = false;

	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
		if (iscntrl(c)) {
	    	//	printf("%d\n", c);
		//        printf("%d ('%c')\n", c, c);
		//        printf("%d\n", c);
			system("clear");
			cout << "Invalid key, please enter s to start or q to quit\n";

	  	} else {
			if (start){
				system("clear");
			} else if (c == 's'){
				start = true;
				cout << (xTurn ? "x" : "o") << "'s move\n";
				printBoard(board);
				continue;
			} else {
				system("clear");
				cout << "Invalid key, please enter s to start or q to quit\n";
				continue;
			};

			cout << "Press q to quit\n\n";


			string play(1, c);

			if (!validPlay(board, xTurn, play)){
				continue;
			};

			if (checkBoard(board)){
				cout << (xTurn ? "x" : "o") << " wins!!!\n";
				printBoard(board);
				break;
			};
			xTurn = !xTurn;
			cout << (xTurn ? "x" : "o") << "'s move\n";
			printBoard(board);
	  	} 
	}
	return 0;	
}
