#include <lvgl.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <display/display.hpp>
#include <ui/ui.h>
#include "display/CST820.h"


/*Don't forget to set Sketchbook location in File/Preferences to the path of your UI project (the parent foder of this INO file)*/

/*Change to your screen resolution*/

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */

    display_setup();

    Serial.println( "Setup done" );
}

void loop()
{
    display_loop();
    delay(5);
}

