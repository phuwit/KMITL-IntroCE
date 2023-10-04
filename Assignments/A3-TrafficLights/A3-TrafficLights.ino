#define BUTTON_EAST 2
#define BUTTON_NORTH 4
#define BUTTON_PEDESTRIAN 3
#define LED_NORTH_RED 13
#define LED_NORTH_YELLOW 12
#define LED_NORTH_GREEN 11
#define LED_PEDESTRIAN_RED 9
#define LED_PEDESTRIAN_GREEN 8
#define LED_EAST_RED 7
#define LED_EAST_YELLOW 6
#define LED_EAST_GREEN 5

enum States { NorthGreen, NorthYellow, EastGreen, EastYellow, Pedestrian, PedestrianBlink };

struct State {
    // int is 2 bytes, it is enough
    unsigned int lightsData;
    // 6-bit pattern to street output
    unsigned int delay;
    // delay in ms units
    States nextState[8];
};  // next state for inputs 0,1,2,3
const struct State stateData[6] = {
    {B10001100, 2000, {NorthYellow, NorthYellow, NorthGreen, NorthYellow, NorthYellow, NorthYellow, NorthYellow, NorthYellow}},
    {B10010100, 500,  {Pedestrian, EastGreen, EastGreen, EastGreen, Pedestrian, Pedestrian, Pedestrian, Pedestrian}},
    {B10100001, 2000, {EastYellow, EastGreen, EastYellow, EastYellow, EastYellow, EastYellow, EastYellow, EastYellow}},
    {B10100010, 500,  {NorthGreen, NorthGreen, NorthGreen, NorthGreen, Pedestrian, Pedestrian, NorthGreen, NorthGreen}},
    {B01100100, 2000, {PedestrianBlink, PedestrianBlink, PedestrianBlink, PedestrianBlink, Pedestrian, PedestrianBlink, PedestrianBlink}},
    {B10100100, 500,  {EastGreen, EastGreen, NorthGreen, EastGreen, NorthGreen, NorthGreen, EastGreen, EastGreen}}
};
unsigned long currentState = 0;  // index to the current state

void setup() {
    Serial.begin(9600);

    pinMode(BUTTON_NORTH, INPUT_PULLUP);
    pinMode(BUTTON_EAST, INPUT_PULLUP);
    pinMode(BUTTON_PEDESTRIAN, INPUT_PULLUP);
    pinMode(LED_NORTH_RED, OUTPUT);
    pinMode(LED_NORTH_YELLOW, OUTPUT);
    pinMode(LED_NORTH_GREEN, OUTPUT);
    pinMode(LED_PEDESTRIAN_RED, OUTPUT);
    pinMode(LED_PEDESTRIAN_GREEN, OUTPUT);
    pinMode(LED_EAST_RED, OUTPUT);
    pinMode(LED_EAST_YELLOW, OUTPUT);
    pinMode(LED_EAST_GREEN, OUTPUT);
}

int input, inputNorth, inputEast, inputPedestrian;

void loop() {
    digitalWrite(LED_PEDESTRIAN_RED,   stateData[currentState].lightsData & B10000000);
    digitalWrite(LED_PEDESTRIAN_GREEN, stateData[currentState].lightsData & B01000000);
    digitalWrite(LED_NORTH_RED,        stateData[currentState].lightsData & B00100000);
    digitalWrite(LED_NORTH_YELLOW,     stateData[currentState].lightsData & B00010000);
    digitalWrite(LED_NORTH_GREEN,      stateData[currentState].lightsData & B00001000);
    digitalWrite(LED_EAST_RED,         stateData[currentState].lightsData & B00000100);
    digitalWrite(LED_EAST_YELLOW,      stateData[currentState].lightsData & B00000010);
    digitalWrite(LED_EAST_GREEN,       stateData[currentState].lightsData & B00000001);

    delay(stateData[currentState].delay);

    inputNorth = !digitalRead(BUTTON_NORTH);
    inputEast = !digitalRead(BUTTON_EAST);
    inputPedestrian = !digitalRead(BUTTON_PEDESTRIAN);
    input = inputEast + (inputNorth * 2) + (inputPedestrian * 4);
    Serial.print("input : ");
    Serial.println(input);

    currentState = stateData[currentState].nextState[input];
    Serial.print("currentState : ");
    Serial.println(currentState);
}
