#define DOOR_SENSOR A0
#define LED D7

// time to wait before alarm switches states
#define DEBOUNCE_SECONDS 10

// whether or not door open is sensed: 0 = no, 1 = yes
int opened = 0;

// the alarm state: 0 = off, 1 = on
int alarmState = 0;

// the last time we switched alarm states
int lastStateSwitchTime = 0;

void setup() {

    pinMode(LED, OUTPUT);
    pinMode(DOOR_SENSOR, INPUT);

    Particle.publish("online");

    Particle.variable("opened", &opened, INT);

    Particle.variable("alarmState", &alarmState, INT);
}

void loop() {
    opened = hasBeenOpened();
    if (opened) {
        digitalWrite(LED, HIGH);

        if (alarmState == 0) {
            // only alarm if we're past the debounce interval
            int now = Time.now();
            if (now - lastStateSwitchTime > DEBOUNCE_SECONDS) {
                alarmState = 1;
                lastStateSwitchTime = now;
                Particle.publish("aUtopia-door-alert-fc2", "on", 60, PRIVATE);
            }
        }
    } else {
        digitalWrite(LED, LOW);

        //
        // Alarm off
        //
        if (alarmState == 1) {
            // only alarm if we're past the debounce interval
            int now = Time.now();
            if (now - lastStateSwitchTime > DEBOUNCE_SECONDS) {
                alarmState = 0;
                lastStateSwitchTime = now;
            }
        }
    }
}

// determine if we're open or not
boolean hasBeenOpened() {
    if (digitalRead(DOOR_SENSOR) == HIGH) {
        return true;
    } else {
        return false;
    }
}
