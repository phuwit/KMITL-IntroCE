#define BUTTON_NORTH 3
#define LED_N_R 10
#define LED_N_Y 9
#define LED_N_G 8
#define BUTTON_EAST 2
#define LED_E_R 6
#define LED_E_Y 5
#define LED_E_G 4

enum States { NorthGreen, NorthYellow, EastGreen, EastYellow, Pedestrian, PedestrianBlink };

struct State {
    // int is 2 bytes, it is enough
    unsigned int lightsData;
    // 6-bit pattern to street output
    unsigned int delay;
    // delay in ms units
    States nextState[8];
};  // next state for inputs 0,1,2,3
typedef const struct State SType;
SType stateData[sizeof(States)] = {
    {b10001100, 2000, {NorthGreen, NorthYellow, NorthGreen, NorthYellow, NorthYellow, NorthYellow, NorthYellow, NorthYellow}},
    {b10010100, 500,  {EastGreen, EastGreen, EastGreen, EastGreen, Pedestrian, Pedestrian, Pedestrian, Pedestrian}},
    {b10100001, 2000, {EastGreen, EastGreen, EastYellow, EastYellow, EastYellow, EastYellow, EastYellow, EastYellow}},
    {b10100010, 500,  {NorthGreen, NorthGreen, NorthGreen, NorthGreen, Pedestrian, Pedestrian, NorthGreen, NorthGreen}},
    {b01100100, 2000, {Pedestrian, PedestrianBlink, PedestrianBlink, Pedestrian, PedestrianBlink, PedestrianBlink, PedestrianBlink}},
    {b10100100, 500,  {EastGreen, EastGreen, NorthGreen, EastGreen, NorthGreen, NorthGreen, EastGreen, EastGreen}}
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
