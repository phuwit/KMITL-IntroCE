#define BUTTON_NORTH 3
#define LED_N_R 10
#define LED_N_Y 9
#define LED_N_G 8
#define BUTTON_EAST 2
#define LED_E_R 6
#define LED_E_Y 5
#define LED_E_G 4

enum States { GoNorth, WaitNorth, GoEast, WaitEast };

struct State {
    unsigned long lightsData;
    // 6-bit pattern to street output
    unsigned long delay;
    // delay in ms units
    unsigned long nextState[4];
};  // next state for inputs 0,1,2,3
typedef const struct State SType;
SType stateData[4] = { {B00001100, 30000, {States::GoNorth, States::WaitNorth, States::GoNorth, States::WaitNorth}},
                       {B00010100, 5000, {States::GoEast, States::GoEast, States::GoEast, States::GoEast}},
                       {B00100001, 30000, {States::GoEast, States::GoEast, States::WaitEast, States::WaitEast}},
                       {B00100010, 5000, {States::GoNorth, States::GoNorth, States::GoNorth, States::GoNorth}}
                      };
unsigned long currentState = 0;  // index to the current state

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
    digitalWrite(LED_N_R, stateData[currentState].lightsData & B00100000);
    digitalWrite(LED_N_Y, stateData[currentState].lightsData & B00010000);
    digitalWrite(LED_N_G, stateData[currentState].lightsData & B00001000);
    digitalWrite(LED_E_R, stateData[currentState].lightsData & B00000100);
    digitalWrite(LED_E_Y, stateData[currentState].lightsData & B00000010);
    digitalWrite(LED_E_G, stateData[currentState].lightsData & B00000001);
    delay(stateData[currentState].delay);
    input1 = digitalRead(BUTTON_NORTH);
    input2 = digitalRead(BUTTON_EAST);
    input = input1 * 2 + input2;
    currentState = stateData[currentState].nextState[input];
}
