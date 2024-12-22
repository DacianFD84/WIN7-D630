#define BUZZER_PIN 2  // Pin connected to the passive buzzer

// Days of the week
const char* daysOfWeek[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

// Event schedule
const char* schedule[][3] = {
  {"06:00", "Rising Bell", "0,1,2,3,4,5,6"},   // All days
  {"06:01", "Go to Class", "0,1,2,3,4"},       // Only weekdays (Monday to Friday)
  {"06:02", "Break Time", "0,1,2,3,4"},        // Only weekdays
  {"06:03", "Return from Break", "0,1,2,3,4"}, // Only weekdays
};

const int scheduleSize = sizeof(schedule) / sizeof(schedule[0]);

int startMinute = 359; // Initial time: 5:59 AM (in minutes from midnight)
bool eventTriggered[scheduleSize] = {false}; // Array to track triggered events for the current minute

// Function to check if the current day is active for a given event
bool isDayActive(int currentDay, const char* activeDays) {
  String days(activeDays);
  return days.indexOf(String(currentDay)) != -1;
}

// Function to trigger alarm with the passive buzzer
void triggerAlarm(const char* eventName, String dateTime, String stopTime) {
  Serial.println("Event Started: " + String(eventName) + " at " + dateTime);

  // Play a tone on the buzzer for 10 seconds
  unsigned long alarmStart = millis();
  while (millis() - alarmStart < 10000) {  // 10 seconds duration
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(500);  // 1 kHz = 1000 microseconds per cycle (500 HIGH, 500 LOW)
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(500);
  }

  Serial.println("Event Stopped: " + String(eventName) + " at " + stopTime);
}

void resetEventTracking() {
  for (int i = 0; i < scheduleSize; i++) {
    eventTriggered[i] = false;
  }
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT); // Set buzzer pin as output
  Serial.begin(9600);
  Serial.println("Starting Alarm System Simulation...");
}

void loop() {
  static unsigned long lastUpdate = millis();
  static int currentMinute = startMinute;
  static int currentSecond = 0;

  unsigned long now = millis();
  if (now - lastUpdate >= 1000) {  // Update every second
    lastUpdate = now;
    currentSecond++;

    if (currentSecond >= 60) {
      currentSecond = 0;
      currentMinute++;
      resetEventTracking(); // Reset event tracking at the start of each new minute
    }

    if (currentMinute >= 1440) {
      currentMinute = 0; // Reset to midnight
    }

    int simulatedHour = currentMinute / 60;
    int simulatedMinute = currentMinute % 60;
    int simulatedDay = (currentMinute / 1440) % 7;

    // Get the current time and milliseconds
    unsigned long currentMillis = millis();  // Milliseconds since the program started
    int millisInCurrentSecond = currentMillis % 1000;  // Get milliseconds in current second

    String currentTime = (simulatedHour < 10 ? "0" : "") + String(simulatedHour) + ":" +
                         (simulatedMinute < 10 ? "0" : "") + String(simulatedMinute) + ":" +
                         (currentSecond < 10 ? "0" : "") + String(currentSecond) + "." +
                         (millisInCurrentSecond < 100 ? "0" : "") + (millisInCurrentSecond < 10 ? "0" : "") + String(millisInCurrentSecond);

    String dateTime = "Simulated Time: " + currentTime + " (" + daysOfWeek[simulatedDay] + ")";

    // Debugging time simulation
    Serial.println("Current Time: " + dateTime);

    // Check for events
    for (int i = 0; i < scheduleSize; i++) {
      String eventTime = String(schedule[i][0]);
      String eventDay = String(schedule[i][2]);

      if (currentTime.substring(0, 5) == eventTime && isDayActive(simulatedDay, eventDay.c_str()) && !eventTriggered[i]) {
        eventTriggered[i] = true; // Mark the event as triggered
        triggerAlarm(schedule[i][1], dateTime, currentTime);
      }
    }
  }
}
