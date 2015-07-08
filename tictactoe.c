/*
--------
  0 1 2
0|_|_|_|
1|_|_|_|
2|_|_|_|

|0|1|2|
|3|4|5|
|6|7|8|
---------
*/
enum LOCATION {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT}; //defines constants for values 0->8
enum STATE {UNFILLED, RED, BLUE, TRAVERSED};
const short GRAPH_SIZE = 9;
const short MIDDLE_SQUARE = FOUR;
const short TOTAL_NODES = 16;
const short DIAG_DIST = 1;
const short STRAIGHT_DIST = 2;

const float WHEEL_RADIUS = 4;
const float AXEL_RADIUS = 7;
//360 = 1, 180 = 2, 90 = 4, 45 = 8
const short INCREMENT = 8;

//PI will cancel out
// NOTE ADD GEAR RATIO
const float	WHEEL_CIRCUMFERENCE = 2*PI*WHEEL_RADIUS;
const float TURN_DISTANCE = ((2*PI*AXEL_RADIUS)/INCREMENT);

const short ANGULAR_INCREMENT = (short)((TURN_DISTANCE/WHEEL_CIRCUMFERENCE)+.5)*360;

struct Square{
	STATE state;
	int nextIndex;
	int distance;
	int angleFrom;
	Square *next;
};

struct Grid{
	Square *next;
};

/*
------------------------------------------
intended graph structure

0|-> 4-> NULL
1|-> 4-> NULL
2|-> 4-> NULL
3|-> 4-> NULL
4|-> 0-> 1-> 2-> 5-> 8-> 7-> 6-> 3-> NULL
5|-> 4-> NULL
6|-> 4-> NULL
7|-> 4-> NULL
8|-> 4-> NULL
------------------------------------------
*/
void fillGraph(Grid *graph, Square *fourMap);
//void fillMiddle(Square mGraph, STATE s_Index, STATE e_Index);

void initializeSquare(Square *editMe, int inputIndex, int inputDistance, int inputAngle);

//returns true if diagonal
//change to isDiag;
bool isDiag(int index);

void moveBot(int dist, STATE &stateValue);
void turnBot(int angle);

void centreBot();
void resetAngle();
bool toCheck();
void processGrid();
void resetTraversed();

task main(){
	Grid graph[GRAPH_SIZE];
	Square fourMap[TOTAL_NODES];
	fillGraph(graph, fourMap);
	string thingy = "";
	for(Square *i = graph[FOUR].next; i != NULL; i = i->next){
		thingy += '0' + i->nextIndex;
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

void fillGraph(Grid *graph, Square *fourMap){
	int j = 0;
	int angle = ANGULAR_INCREMENT;
	for(int i = ZERO; i < MIDDLE_SQUARE; i++){
		initializeSquare(&fourMap[j], MIDDLE_SQUARE, -(isDiag(i)?DIAG_DIST:STRAIGHT_DIST), 0);
		graph[i].next = &fourMap[j];
		j++;
	}

	initializeSquare(&fourMap[j], ZERO, (isDiag(ZERO)?DIAG_DIST:STRAIGHT_DIST), angle);
	Square *tmp = &fourMap[j];
	graph[FOUR].next = tmp;
	j++;

	for(int i = ONE; i < (long)THREE; i++){
		angle += ANGULAR_INCREMENT;
		initializeSquare(&fourMap[j], i, (isDiag(i)?DIAG_DIST:STRAIGHT_DIST), angle);
		tmp->next = &fourMap[j];
		tmp = tmp->next;
		j++;
	}

	angle += ANGULAR_INCREMENT;
	initializeSquare(&fourMap[j], FIVE, (isDiag(FIVE)?DIAG_DIST:STRAIGHT_DIST), angle);
	tmp->next = &fourMap[j];
	tmp = tmp->next;
	j++;

	for(int i = EIGHT; i >= (long)SIX; i--){
		angle += ANGULAR_INCREMENT;
		initializeSquare(&fourMap[j], i, (isDiag(i)?DIAG_DIST:STRAIGHT_DIST), angle);
		tmp->next = &fourMap[j];
		tmp = tmp->next;
		j++;
	}

	angle += ANGULAR_INCREMENT;
	initializeSquare(&fourMap[j], THREE, (isDiag(THREE)?DIAG_DIST:STRAIGHT_DIST), angle);
	tmp->next = &fourMap[j];
	tmp = tmp->next;
	j++;

	for(int i = FIVE; i <= (long)EIGHT; i++){
		initializeSquare(&fourMap[j], MIDDLE_SQUARE, -(isDiag(i)?DIAG_DIST:STRAIGHT_DIST), 0);
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

bool isDiag(int index){
	return (index == 0 || index == 2 || index == 6 || index == 8);
}

void moveBot(int dist, STATE &stateValue){
	//stores starting encoder value
	int startPos = nMotorEncoder[motorA];
	//int endPos = 0;
	int speed = ((dist>0)? 75: -75);
	setMotorSpeed(motorA, speed);
	setMotorSpeed(motorB, speed);
	int turnDegrees = (int)(((float)(dist*360))/(WHEEL_CIRCUMFERENCE) + .5);
	//int targetEncoderVal = startPos - turnDegrees;
	if(dist > 0){
		while(((startPos+turnDegrees) < nMotorEncoder[motorA]) &&(SensorValue[S1] <= 0)){}
	}
	else{
		while(((startPos+turnDegrees) > nMotorEncoder[motorA]) &&(SensorValue[S1] <= 0)){}
	}
	if(SensorValue[S1]>0){
		stateValue = RED;
	}
	else{
		stateValue = TRAVERSED;
	}
	nMotorEncoder[motorA] =  startPos;
}

void turnBot(int angle){
	setMotorSpeed(motorA,50);
	setMotorSpeed(motorB,-50);
	while(nMotorEncoder[motorA] <= angle){}
}

//void fillMiddle(Square mGraph, STATE s_Index, STATE e_Index){
//	for(int i = ZERO; i < MIDDLE_SQUARE; i++){
//			Square* newSquare;
//			newSquare->state = UNFILLED;
//			newSquare->nextIndex = i;
//			newSquare->next = NULL;
//			tmp->next = newSquare;
//			tmp = tmp->next;
//	}
//}
