#define TINY_GSM_MODEM_SIM7600
#include <Arduino.h>
#include <HardwareSerial.h>
HardwareSerial SerialAT(1);
HardwareSerial SerialSensor(0);

#include <TinyGsmClient.h>
#include <PubSubClient.h>

TinyGsm modem(SerialAT);
TinyGsmClient gsmclient(modem);
PubSubClient client(gsmclient);

#define UART_BAUD 115200

const char apn[] = "internet";
const char gprsUser[] = "";
const char gprsPass[] = "";

// MQTT Network  Home Assistance
// const char *mqtt_server = "203.150.107.185"; // komom
const char *mqtt_server = "203.154.82.28"; // pttfarm
// Home Assistant Credentials for mqtt user
const char *HA_USER = "mqtt_kmm1";
const char *HA_PASS = "1234P@ssw0rd";
const char *ID = "GCCKmmFarm1"; // Name of our device, must be unique
long lastReconnectAttempt = 0;  // mqtt

#define NUM_VALVES 4 // Number of valves

// Array to store the state of each valve (0 = closed, 1 = open)
int valveStates[NUM_VALVES] = {0}; // Initialize all valves as closed (0)
/**
 * function getChipId()1
 */
uint32_t chipId = 0;
char *CALL_NAME = "gcc_kmm_1";

void getChipId()
{
    for (int i = 0; i < 17; i = i + 8)
    {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }

    Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
    Serial.printf("This chip has %d cores\n", ESP.getChipCores());
    Serial.print("Chip ID: ");
    Serial.println(chipId);
}

const char *STATE_TOPIC_V1 = "gcckmm1/switchs/13097652/valve1/state"; // Topic
const char *COMMAND_TOPIC_V1 = "gcckmm1/switchs/13097652/valve1/set"; // Topic
bool STATE_V1 = false;

const char *STATE_TOPIC_V2 = "gcckmm1/switchs/13097652/valve2/state"; // Topic
const char *COMMAND_TOPIC_V2 = "gcckmm1/switchs/13097652/valve2/set"; // Topic
bool STATE_V2 = false;

const char *STATE_TOPIC_V3 = "gcckmm1/switchs/13097652/valve3/state"; // Topic
const char *COMMAND_TOPIC_V3 = "gcckmm1/switchs/13097652/valve3/set"; // Topic
bool STATE_V3 = false;

const char *STATE_TOPIC_V4 = "gcckmm1/switchs/13097652/valve4/state"; // Topic
const char *COMMAND_TOPIC_V4 = "gcckmm1/switchs/13097652/valve4/set"; // Topic
bool STATE_V4 = false;

// SENSOR
const char *SENSOR_TOPIC = "gcckmm1/sensors/13097652/state";

void debugTopic()
{
    Serial.println("[Debug Topic ]");

    Serial.print("  STATE_TOPIC_V1 : ");
    Serial.println(STATE_TOPIC_V1);
    Serial.print("  COMMAND_TOPIC_V1 : ");
    Serial.println(COMMAND_TOPIC_V1);
    Serial.print("  STATE_V1");
    Serial.println(STATE_V1);

    Serial.print("  STATE_TOPIC_V2 : ");
    Serial.println(STATE_TOPIC_V2);
    Serial.print("  COMMAND_TOPIC_V2 : ");
    Serial.println(COMMAND_TOPIC_V2);
    Serial.print("  STATE_V2");
    Serial.println(STATE_V2);

    Serial.print("  STATE_TOPIC_V3 : ");
    Serial.println(STATE_TOPIC_V3);
    Serial.print("  COMMAND_TOPIC_V3 : ");
    Serial.println(COMMAND_TOPIC_V3);
    Serial.print("  STATE_V3");
    Serial.println(STATE_V3);

    Serial.print("  STATE_TOPIC_V4 : ");
    Serial.println(STATE_TOPIC_V4);
    Serial.print("  COMMAND_TOPIC_V4 : ");
    Serial.println(COMMAND_TOPIC_V4);
    Serial.print("  STATE_V4");
    Serial.println(STATE_V4);
}
/**
 * ready state
 *  - all valve state close
 */
void readyState()
{
    // All Close
    // Red
    Serial.println("[] Ready State");
}

/***
 * check any zone is open
 */
bool isAnyValveOpen()
{
}

void CloseValve(int num, int time)
{
    char buffer[20];
    sprintf(buffer, "Close  %d , %d", num, time);
    Serial.println(buffer);
    int _valve = num - 1;
    valveStates[_valve] = 0;
    switch (num)
    {
    case 1:
        client.publish(STATE_TOPIC_V1, "OFF");
        RELAY1_OFF;
        break;
    case 2:
        client.publish(STATE_TOPIC_V2, "OFF");
        RELAY2_OFF;
        break;
    case 3:
        client.publish(STATE_TOPIC_V3, "OFF");
        RELAY3_OFF;
        break;
    case 4:
        client.publish(STATE_TOPIC_V4, "OFF");
        RELAY4_OFF;
        break;
    default:
        break;
    }
}

void OpenValve(int num, int time)
{
    char buffer[20];
    sprintf(buffer, "Open  %d , %d", num, time);
    Serial.println(buffer);
    int _valve = num - 1;
    valveStates[_valve] = 1;
    switch (num)
    {
    case 1:
        client.publish(STATE_TOPIC_V1, "ON");
        RELAY1_ON;
        break;
    case 2:
        client.publish(STATE_TOPIC_V2, "ON");
        RELAY2_ON;
        break;
    case 3:
        client.publish(STATE_TOPIC_V3, "ON");
        RELAY3_ON;
        break;
    case 4:
        client.publish(STATE_TOPIC_V4, "ON");
        RELAY4_ON;
        break;
    default:
        break;
    }
}
/**
 * @brief Control pump :
 */
void ClosePump(int num, int time)
{
    char buffer[20];
    sprintf(buffer, "Close  %d , %d", num, time);
    Serial.println(buffer);
}

/**
 * @brief Control valve
 */
void OpenPump(int num, int time)
{
    char buffer[20];
    sprintf(buffer, "  Open  %d , %d", num, time);
    Serial.println(buffer);
}

/**
 * @brief Command for open pump
 */
void pumpCommand(const String &payload, int valveNum, int valveTime)
{
    Serial.println();
    Serial.println("[handle] Pump Command");
    if (payload == "ON")
    {
        OpenPump(valveNum, valveTime);
    }
    else if (payload == "OFF")
    {
        ClosePump(valveNum, valveTime);
    }
}

/**
 * @brief Command for open valve
 */
void handleCommand(const String &payload, int valveNum, int valveTime)
{
    Serial.println();
    Serial.println("[handle] Valve Command");
    if (payload == "ON")
    {
        OpenValve(valveNum, valveTime);
    }
    else if (payload == "OFF")
    {
        CloseValve(valveNum, valveTime);
    }
}

/***
 * Process command from callback
 * - compare topic with commandTopic
 */
void processCommand(const char *topic, const String &commandTopic, const String &stateTopic, byte *payload, unsigned int length, int valveNum, int valveTime, void (*commandHandler)(const String &, int, int))
{
    Serial.println();
    Serial.println("[process] Command");

    if (String(topic) == commandTopic)
    {
        Serial.print("COMMAND_TOPIC : [");
        Serial.print(commandTopic);
        Serial.println(" ] ");

        String payloadStr = "";
        for (unsigned int i = 0; i < length; i++)
        {
            Serial.printf("Payload : %d ", i);
            payloadStr += (char)payload[i];
        }
        // call commandHandler
        Serial.println();
        if (payloadStr == "ON")
        {
            // return state  ON
            client.publish(stateTopic.c_str(), "ON");
            commandHandler(payloadStr, valveNum, valveTime);
        }
        else if (payloadStr == "OFF")
        {
            // return state  OFF
            client.publish(stateTopic.c_str(), "OFF");
            commandHandler(payloadStr, valveNum, valveTime);
        }
    }
    // Clear retain
    // client.publish(commandTopic.c_str(),"");
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.println();
    Serial.print("[Callback] Message arrived << [");
    Serial.print(topic);
    Serial.println("] ");
    // call process command
    processCommand(topic, COMMAND_TOPIC_V1, STATE_TOPIC_V1, payload, length, 1, 1000, handleCommand);
    processCommand(topic, COMMAND_TOPIC_V2, STATE_TOPIC_V2, payload, length, 2, 1000, handleCommand);
    processCommand(topic, COMMAND_TOPIC_V3, STATE_TOPIC_V3, payload, length, 3, 1000, handleCommand);
    processCommand(topic, COMMAND_TOPIC_V4, STATE_TOPIC_V4, payload, length, 4, 1000, handleCommand);
}

/**
 * @brief Reconnect to mqtt server
 */
bool reconnect()
{
    // Loop until we're reconnected
    Serial.println("[MQTT] reconnect");
    int try_num = 0;
    while (!client.connected() && try_num < 5)
    {
        Serial.print("  Attempting MQTT connection...");
        Serial.println(try_num);
        // Attempt to connect
        if (client.connect(ID, HA_USER, HA_PASS))
        {
            Serial.println("  client.connect() ... connected");
            Serial.print("  Publishing to device: ");
            Serial.println(ID);
            Serial.println('\n');
            // device sub to all command_topic QoS 0
            client.subscribe(COMMAND_TOPIC_V1, 0);
            client.subscribe(COMMAND_TOPIC_V2, 0);
            client.subscribe(COMMAND_TOPIC_V3, 0);
            client.subscribe(COMMAND_TOPIC_V4, 0);
        }
        else
        {
            Serial.println("   try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
        try_num++;
    }
    return client.connected();
}
