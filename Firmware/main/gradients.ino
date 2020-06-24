/*
  Copyright (c) 2019 Andy England
  The Infinite Pyramid
*/
//Custom color gradients, each gradient contains 16 colors, more information on the CRGB object at https://github.com/FastLED/FastLED/wiki/Pixel-reference

CRGBPalette16 currentPalette = PartyColors_p;
TBlendType    currentBlending = LINEARBLEND;

extern const TProgmemPalette16 redWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

extern const TProgmemPalette16 watermelonPalette_p PROGMEM =
{
  CRGB::Black,
  CRGB::Black,
  CRGB::Green,
  CRGB::Green,
  CRGB::Black,
  CRGB::Black,
  CRGB::Purple,
  CRGB::Purple,
  CRGB::Black,
  CRGB::Black,
  CRGB::Green,
  CRGB::Green,
  CRGB::Black,
  CRGB::Black,
  CRGB::Purple,
  CRGB::Purple
};

extern const TProgmemPalette16 unstripedWatermelonPalette_p PROGMEM =
{
  CRGB::Green,
  CRGB::Green,
  CRGB::Orange,
  CRGB::Yellow,
  CRGB::Purple,
  CRGB::Purple,
  CRGB::Orange,
  CRGB::Yellow,
  CRGB::Green,
  CRGB::Green,
  CRGB::Orange,
  CRGB::Yellow,
  CRGB::Purple,
  CRGB::Purple,
  CRGB::Orange,
  CRGB::Yellow
};

extern const TProgmemPalette16 whitePalette_p PROGMEM =
{
  CRGB::Black,
  CRGB::Black, // 'white' is too bright compared to red and blue
  CRGB::Black,
  CRGB::White,
  CRGB::Black,
  CRGB::Black, // 'white' is too bright compared to red and blue
  CRGB::Black,
  CRGB::White,
  CRGB::Black,
  CRGB::Black, // 'white' is too bright compared to red and blue
  CRGB::Black,
  CRGB::White,
  CRGB::Black,
  CRGB::Black, // 'white' is too bright compared to red and blue
  CRGB::Black,
  CRGB::White,
};
