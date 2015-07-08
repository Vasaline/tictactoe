/*
0	1	2
0|_|_|_|
1|_|_|_|
2|_|_|_|

|0|1|2|
|3|4|5|
|6|7|8|




*/
enum LOCATION {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT}; //defines constants for values 0->8
enum STATE {UNFILLED, RED, BLUE, TRAVERSED};
const short GRAPH_SIZE = 9;
const short MIDDLE_SQUARE = (short) FOUR;
const short TOTAL_NODES = 16;
const short DIAG_DIST = 1;
const short STRAIGHT_DIST = 2;
const short ANGULAR_INCREMENT = 45;

const float WHEEL_DIAMETRE = 4;
const float	WHEEL_CIRCUMFERENCE = WHEEL_DIAMETRE*PI;

struct Square{
	STATE state;
	int nextIndex;
	int distance;
	int angleFrom;
	Square *next;
};

struct Node{
	Square *next;
};

//struct BGraph{
//	STATE state;
//	Square *head;
//}
/*class TicTacToe{
private:
Square *graph;

};*/
/*
intended graph structure

0|-> 4-> NULL
1|-> 4-> NULL
2|-> 4-> NULL
3|-> 4-> NULL
4|-> 0-> 1-> 2-> 3-> 5-> 6-> 7-> 8-> NULL
5|-> 4-> NULL
6|-> 4-> NULL
7|-> 4-> NULL
8|-> 4-> NULL
*/
void fillGraph(Node *graph, Square *fourMap);
//void fillMiddle(Square mGraph, STATE s_Index, STATE e_Index);

void initializeSquare(Square *editMe, int inputIndex, int inputDistance, int inputAngle);

//returns true if diagonal
bool travelType(int index);

void moveBot(int dist);
void turnBot(int angle);

task main(){
	Node graph[GRAPH_SIZE];
	Square fourMap[TOTAL_NODES];
	fillGraph(graph, fourMap);
	string thingy = "";
	for(Square *i = graph[FOUR].next; i != NULL; i = i->next){
		thingy += '0' + i->nextIndex;
		thingy += ", -";
		thingy += '0' + -(i->distance);
		thingy += ", ";
	}

	displayCenteredTextLine(1,"%d, %d",graph[0].next->nextIndex,graph[0].next->distance);
	displayCenteredTextLine(2,"%d, %d",graph[1].next->nextIndex,graph[1].next->distance);
	displayCenteredTextLine(3,"%d, %d",graph[2].next->nextIndex,graph[2].next->distance);
	displayCenteredTextLine(4,"%d, %d",graph[3].next->nextIndex,graph[3].next->distance);
	displayCenteredTextLine(5,thingy);
	displayCenteredTextLine(6,"%d, %d",graph[5].next->nextIndex,graph[5].next->distance);
	displayCenteredTextLine(7,"%d, %d",graph[6].next->nextIndex,graph[6].next->distance);
	displayCenteredTextLine(8,"%d, %d",graph[7].next->nextIndex,graph[7].next->distance);
	displayCenteredTextLine(9,"%d, %d",graph[8].next->nextIndex,graph[8].next->distance);

	sleep(10000);

}

void fillGraph(Node *graph, Square *fourMap){
	int j = 0;
	int angle = 0;
	for(int i = ZERO; i < MIDDLE_SUQARE; i++){
		initializeSquare(&fourMap[j], MIDDLE_SUQARE, (travelType(i)?DIAG_DIST:STRAIGHT_DIST), 0);
		graph[i].next = &fourMap[j];
		j++;
	}

	initializeSquare(&fourMap[j], ZERO, -(travelType(ZERO)?DIAG_DIST:STRAIGHT_DIST), angle);
	Square* tmp = &fourMap[j];
	graph[FOUR].next = tmp;
	j++;

	for(int i = ONE; i < MIDDLE_SUQARE; i++){
		angle += ANGULAR_INCREMENT;
		initializeSquare(&fourMap[j], i, -(travelType(i)?DIAG_DIST:STRAIGHT_DIST), angle);
		tmp->next = &fourMap[j];
		tmp = tmp->next;
		j++;
	}
	for(int i = FIVE; i <= (long)EIGHT; i++){
		angle += ANGULAR_INCREMENT;
		initializeSquare(&fourMap[j], i, -(travelType(i)?DIAG_DIST:STRAIGHT_DIST), angle);
		tmp->next = &fourMap[j];
		tmp = tmp->next;
		j++;
	}

	for(int i = FIVE; i <= (long)EIGHT; i++){
		initializeSquare(&fourMap[j], MIDDLE_SUQARE, (travelType(i)?DIAG_DIST:STRAIGHT_DIST), 0);
		graph[i].next = &fourMap[j];
		j++;
	}
}


void initializeSquare(Square *editMe, int inputIndex, int inputDistance, int inputAngle){
	editMe->state = UNFILLED;
	editMe->nextIndex = inputIndex;
	editMe->distance = inputDistance;
	editMe->angleFrom = inputAngle;
	editMe->next = NULL;
}

bool travelType(int index){
	return (index == 0 || index == 2 || index == 6 || index == 8);
}

//void moveBot(int dist){
//	int startPos = nMotorEncoder[motorA];
//	int endPos = 0;
//	int speed = (dist>0)? 75: -75);
//	setMotorSpeed(motorA, speed);
//	setMotorSpeed(motorB, speed);
//	int nDeg
//	float turnDegrees = (dist*360)/(WHEEL_CIRCUMFERENCE);
//	while()
//}
//void fillMiddle(Square mGraph, STATE s_Index, STATE e_Index){
//	for(int i = ZERO; i < MIDDLE_SUQARE; i++){
//			Square* newSquare;
//			newSquare->state = UNFILLED;
//			newSquare->nextIndex = i;
//			newSquare->next = NULL;
//			tmp->next = newSquare;
//			tmp = tmp->next;
//	}

//}
