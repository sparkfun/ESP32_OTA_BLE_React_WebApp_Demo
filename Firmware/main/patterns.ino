/*
  Copyright (c) 2019 Andy England
  The Infinite Pyramid
*/
uint8_t colorBuffer[40][6][1];

double gammaCorrect(uint8_t *valueToCorrect, float gammaVal = 2.2)
{
  *valueToCorrect = 255 * pow(*valueToCorrect / 255.0, gammaVal);
}

void fadeAll(int scale = 250)
{
  if (scale == -1)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i].nscale8(random(220, 255));
    }
  }
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].nscale8(scale);
  }
}

//Patterns: create patterns here, each function should be a single frame, or in most cases, a full loop
void sparkle ()//single frame
{
  for (int i = 0; i < NUM_LEDS; i++)//loop through all LED's, occasionally picking a random color for an LED
  {
    int randomNumber = random(25);
    colorIndex = random(255);//Pick a random color
    if (randomNumber == 1)//If the random number we picked is 1 (1 in 25 chance), then light up an LED with the random color
    {
      leds[i] = ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
    }
  }
  FastLED.show();
  fadeAll(230);//Fade all LED's by 230/255
  delay(15);
}

void slowerSparkle ()//single frame
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    int randomNumber = random(150);
    colorIndex = random(255);//pick a random color
    if (randomNumber == 1)
    {
      leds[i] = ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
    }
  }
  FastLED.show();
  fadeAll(250);
  delay(15);
}

void hueSparkle ()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    int randomNumber = random(25);
    if (randomNumber == 1)
    {
      leds[i] = ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
    }
  }
  colorIndex++;//increment the colorIndex each frame to slowly step through the gradient
  FastLED.show();
  fadeAll(230);
  delay(15);
}

void hueSlowerSparkle ()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    int randomNumber = random(150);
    if (randomNumber == 1)
    {
      leds[i] = ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
    }
  }
  colorIndex++;
  FastLED.show();
  fadeAll(250);
  delay(15);
}

void sparkleSolid ()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    int randomNumber = random(200);//light an LED up rarely
    if (randomNumber == 1)
    {
      leds[i] = ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
    }
  }
  FastLED.show();
  frameDelay++;
  if (frameDelay % 2 == 0)//fade every other frame
  {
    fadeAll(253);
  }
  if (frameDelay > 64)//rarely increment color
  {
    colorIndex++;
    fadeAll(253);
    frameDelay = 0;
  }
  delay(15);
}

void solid ()//
{
  for (int led = 0; led < NUM_LEDS; led++)
  {
    leds[led] = ColorFromPalette( currentPalette, colorIndex + led, 180, currentBlending);//write to all LED's with a colorIndex, offset the colorIndex by the LED number so each LED is a different color
  }
  frameDelay++;
  if (frameDelay > 2)//increment the colorIndex every third frame
  {
    colorIndex++;
    frameDelay = 0;
  }
  FastLED.show();
  delay(25);
}

void solidSlow ()
{
  for (int led = 0; led < NUM_LEDS; led++)
  {
    leds[led] = ColorFromPalette( currentPalette, colorIndex + led, 180, currentBlending);
  }
  colorIndex++;
  FastLED.show();
  delay(25);
}

void trail ()
{
  for (int led = 0; led < NUM_PER_SIDE; led++)
  {
    for (int side = 0; side < 6; side++)
    {
      leds[led + (side * NUM_PER_SIDE)] =  ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
    }
    colorIndex++;
    FastLED.show();
    fadeAll(184);
    delay(15);
  }
  for (int led = NUM_PER_SIDE     ; led < NUM_PER_SIDE; led++)
  {
    for (int side = 0; side < 6; side++)
    {
      leds[led + (side * NUM_PER_SIDE)] =  ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
    }
    colorIndex++;
    FastLED.show();
    fadeAll(184);
    delay(15);
  }
}

void solidTrail()
{
  for (int led = 0; led < NUM_PER_SIDE; led++)
  {
    for (int side = 0; side < 6; side++)
    {
      leds[led + (side * NUM_PER_SIDE)] =  ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
    }
    FastLED.show();
    fadeAll(230);
    delay(15);

  }
  colorIndex++;
}

void slowTrail()
{
  for (int led = 0; led < NUM_PER_SIDE; led++)
  {
    for (int side = 0; side < 6; side++)
    {
      leds[led + (side * NUM_PER_SIDE)] =  ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
    }
    for (int i = 0; i < 3; i++) {
      FastLED.show();
      fadeAll(240);
      delay(15);
    }
  }
  colorIndex++;
}

void centerOut()
{
  for (int led = 0; led < NUM_PER_SIDE / 2; led++)
  {
    for (int side = 0; side < 6; side++)
    {
      leds[led + (side * NUM_PER_SIDE)] =  ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
      leds[NUM_PER_SIDE - led - 1 + (side * NUM_PER_SIDE)] =  ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
    }
    for (int i = 0; i < 2; i++) {
      FastLED.show();
      fadeAll(200);
      delay(15);
    }
  }
  for (int led = NUM_PER_SIDE / 2; led > 0; led--)
  {
    for (int side = 0; side < 6; side++)
    {
      leds[led + (side * NUM_PER_SIDE)] =  ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
      leds[NUM_PER_SIDE - led - 1 + (side * NUM_PER_SIDE)] =  ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
    }
    for (int i = 0; i < 2; i++) {
      FastLED.show();
      fadeAll(200);
      delay(15);
    }
  }
  colorIndex++;
}

void centerOutStagger()
{
  for (int led = 0; led < NUM_PER_SIDE / 2; led++)
  {
    for (int side = 0; side < 6; side++)
    {
      leds[led + (side * NUM_PER_SIDE)] =  ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
      leds[NUM_PER_SIDE - led - 1 + (side * NUM_PER_SIDE)] =  ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
    }
    for (int i = 0; i < 2; i++) {
      FastLED.show();
      fadeAll(200);
      delay(15);
    }
  }
  for (int led = NUM_PER_SIDE / 2; led > 0; led--)
  {
    for (int side = 0; side < 6; side++)
    {
      leds[led + (side * NUM_PER_SIDE)] =  ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
      leds[NUM_PER_SIDE - led - 1 + (side * NUM_PER_SIDE)] =  ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
    }
    for (int i = 0; i < 2; i++) {
      FastLED.show();
      fadeAll(200);
      delay(15);
    }
  }
  colorIndex++;
}

void audioBuffer()
{
  computeFFT();
  fftAvg();
  uint8_t temp;
  for (int side = 0; side < NUM_SIDES; side++)
  {
    temp = avgOfBins[side] * ((0.7 * side) + 0.45);//avgOfBins[side];//
    gammaCorrect(&temp, 1.7);
    colorBuffer[0][side][0] = temp;
    //colorBuffer[0][side][1] = round(map(temp, 0, 255, 0, NUM_PER_SIDE)); //maybe use for center out
    for (int currentPosition = 0; currentPosition < NUM_PER_SIDE; currentPosition++)
    {
      int ledNum = currentPosition + (side * NUM_PER_SIDE);
      CRGB tempColor = ColorFromPalette(currentPalette, colorBuffer[currentPosition][side][0] + (side * 42.5), colorBuffer[currentPosition][side][0], currentBlending);
      if (tempColor.getLuma() > leds[ledNum].getLuma()) //Compare brightness
      {
        leds[ledNum] = tempColor;
      }
    }
    for (int bufferPosition = NUM_PER_SIDE; bufferPosition >= 0; bufferPosition--)
    {
      colorBuffer[bufferPosition][side][0] = colorBuffer[bufferPosition - 1][side][0];
      //colorBuffer[bufferPosition][side][1] = colorBuffer[bufferPosition - 1][side][1];
    }
  }
  FastLED.show();
  fadeAll(127);
}

void centerAudioBuffer()
{
  computeFFT();
  fftAvg();
  uint8_t forwardValue;
  uint8_t backwardValue;
  for (int side = 0; side < NUM_SIDES; side++)
  {
    forwardValue = vReal[LOWEST_HZ_BIN + (side * 2)] * ((0.7 * side) + 0.45);//avgOfBins[side];//
    backwardValue = vReal[LOWEST_HZ_BIN + 1 +   (side * 2)] * ((0.7 * side) + 0.45);//avgOfBins[side];//
    gammaCorrect(&forwardValue, 1.7);
    gammaCorrect(&backwardValue, 1.7);
    colorBuffer[18][side][0] = forwardValue;
    colorBuffer[19][side][0] = (forwardValue + backwardValue ) / 2;
    colorBuffer[20][side][0] = backwardValue;
    //colorBuffer[0][side][1] = round(map(temp, 0, 255, 0, NUM_PER_SIDE)); //maybe use for center out
    for (int currentPosition = 0; currentPosition < NUM_PER_SIDE; currentPosition++)
    {
      int ledNum = currentPosition + (side * NUM_PER_SIDE);
      CRGB tempColor = ColorFromPalette(currentPalette, colorBuffer[currentPosition][side][0] + (side * 42.5), colorBuffer[currentPosition][side][0], currentBlending);
      if (tempColor.getLuma() > leds[ledNum].getLuma()) //Compare brightness
      {
        leds[ledNum] = tempColor;
      }
    }
    for (int bufferPosition = NUM_PER_SIDE; bufferPosition >= 20; bufferPosition--)
    {
      colorBuffer[bufferPosition][side][0] = colorBuffer[bufferPosition - 1][side][0] * 0.9;
      //colorBuffer[bufferPosition][side][1] = colorBuffer[bufferPosition - 1][side][1];
    }
    for (int bufferPosition = 0; bufferPosition <= 18; bufferPosition++)
    {
      colorBuffer[bufferPosition][side][0] = colorBuffer[bufferPosition + 1][side][0] * 0.9;
      //colorBuffer[bufferPosition][side][1] = colorBuffer[bufferPosition - 1][side][1];
    }
  }
  FastLED.show();
  fadeAll(64);
}


void simpleAudio()
{
  computeFFT();
  uint8_t temp;
  for (int currentPosition = 0; currentPosition < NUM_LEDS; currentPosition++)
  {
    temp = vReal[LOWEST_HZ_BIN + currentPosition];
    CRGB tempColor = ColorFromPalette(currentPalette, temp, temp, currentBlending);
    if (tempColor.getLuma() > leds[currentPosition].getLuma()) //Compare brightness
    {
      leds[currentPosition] = tempColor;
    }
  }
  FastLED.show();
  fadeAll(127);
}

/*void batteryDisplay()
{
  uint8_t percentageToLight = (lipo.soc() * NUM_PER_SIDE) / 100.0;
  for (uint8_t side = 0; side < NUM_SIDES; side++)
  {
    for (uint8_t ledNum = 0; ledNum < percentageToLight; ledNum++)
    {
    }
  }
}*/

void patternChange()
{
  double timeStamp = millis();
  if (timeStamp - lastTimeStamp > 50)
  {
    pattern++;
  }
  lastTimeStamp = millis();

  switch (gradient)
  { //add a new case for each new gradient
    case 0:
      currentPalette = RainbowColors_p;
      break;
    case 1:
      currentPalette = PartyColors_p;
      break;
    case 2:
      currentPalette = LavaColors_p;
      break;
    case 3:
      currentPalette = HeatColors_p;
      break;
    case 4:
      currentPalette = ForestColors_p;
      break;
    case 5:
      currentPalette = CloudColors_p;
      break;
    case 6:
      currentPalette = OceanColors_p;
      break;
    case 7:
      currentPalette = redWhiteBluePalette_p;
      break;
    case 8:
      currentPalette = watermelonPalette_p;
      break;
    case 9:
      currentPalette = unstripedWatermelonPalette_p;
      break;
    case 10:
      currentPalette = whitePalette_p;
      break;
    default:
      gradient = 0;
      break;

  }
}
