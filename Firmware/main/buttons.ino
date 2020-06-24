#include <AceButton.h>
using namespace ace_button;

// Number of buttons
const uint8_t NUM_BUTTONS = 2;
uint8_t buttonPins[NUM_BUTTONS] = {2, 27};
AceButton buttons[NUM_BUTTONS];

void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState);

void initializeButtons() {
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {

    // Button uses the built-in pull up register.
    pinMode(buttonPins[i], INPUT_PULLDOWN);

    // initialize the corresponding AceButton
    buttons[i].init(buttonPins[i], LOW, i);
  }

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);

  Serial.println(F("setup(): ready"));
}

void checkButton() {
  // Should be called every 4-5ms or faster, for the default debouncing time
  // of ~20ms.
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].check();
  }
}

// The event handler for the button.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {

  // Print out a message for all events.
  Serial.print(F("handleEvent(): eventType: "));
  Serial.print(eventType);
  Serial.print(F("; buttonState: "));
  Serial.println(buttonState);

  int id = button->getId();
  if (id == 0)
  {
    id = -1;
  }

  switch (eventType) {
    case AceButton::kEventClicked:
      pattern += id;
      pattern %= NUM_PATTERNS;
      break;
    case AceButton::kEventDoubleClicked:
      gradient += id;
      gradient %= NUM_GRADIENTS;
      break;
    case AceButton::kEventLongPressed:
      audioScale += (id / 2.0);
      break;
    case AceButton::kEventRepeatPressed:
      audioScale += (id / 2.0);
      break;
  }
}

/*
void checkValuesInRange()
{
  if (audioScale < 0.55)
  {
    audioScale = 0.50;
  }
  else if (audioScale > 200)
  {
    audioScale = 200;
  }
}*/
