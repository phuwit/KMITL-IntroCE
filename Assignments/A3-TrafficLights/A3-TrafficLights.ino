#define BUTTON_NORTH 3
#define LED_N_R 10
#define LED_N_Y 9
#define LED_N_G 8
#define BUTTON_EAST 2
#define LED_E_R 6
#define LED_E_Y 5
#define LED_E_G 4
#define goN 0
#define waitN 1
#define goE 2
#define waitE 3

enum States { GoNorth, WaitNorth, GoEast, WaitEast };

struct State {
    unsigned long ST_Out;
    // 6-bit pattern to street output
    unsigned long Time;
    // delay in ms units
    unsigned long NextST[4];
};  // next state for inputs 0,1,2,3
typedef const struct State SType;
SType FSM[4] = {{B00001100, 30000, {goN, waitN, goN, waitN}},
                {B00010100, 5000, {goE, goE, goE, goE}},
                {B00100001, 30000, {goE, goE, waitE, waitE}},
                {B00100010, 5000, {goN, goN, goN, goN}}};
unsigned long ST = 0;  // index to the current state

void setup() {
    pinMode(LED_N_R, OUTPUT);
    pinMode(LED_N_Y, OUTPUT);
    pinMode(LED_N_G, OUTPUT);
    pinMode(BUTTON_NORTH, INPUT);
    pinMode(LED_E_R, OUTPUT);
    pinMode(LED_E_Y, OUTPUT);
    pinMode(LED_E_G, OUTPUT);
    pinMode(BUTTON_EAST, INPUT);
}

int input, input1, input2;

void loop() {
    digitalWrite(LED_N_R, FSM[ST].ST_Out & B00100000);
    digitalWrite(LED_N_Y, FSM[ST].ST_Out & B00010000);
    digitalWrite(LED_N_G, FSM[ST].ST_Out & B00001000);
    digitalWrite(LED_E_R, FSM[ST].ST_Out & B00000100);
    digitalWrite(LED_E_Y, FSM[ST].ST_Out & B00000010);
    digitalWrite(LED_E_G, FSM[ST].ST_Out & B00000001);
    delay(FSM[ST].Time);
    input1 = digitalRead(BUTTON_NORTH);
    input2 = digitalRead(BUTTON_EAST);
    input = input1 * 2 + input2;
    ST = FSM[ST].NextST[input];
}
