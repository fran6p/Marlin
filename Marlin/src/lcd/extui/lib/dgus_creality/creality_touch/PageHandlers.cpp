#include "../../../../../inc/MarlinConfigPre.h"

#include "../DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"

#include "../../../../../module/temperature.h"
#include "../../../../../module/motion.h"
#include "../../../../../module/planner.h"
#include "../../../../../feature/pause.h"

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
#include "../../../../../feature/runout.h"
#endif

#include "../../../../../module/settings.h"

#include "../../../../ultralcd.h"
#include "../../../ui_api.h"

#include "PageHandlers.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

// Definitions of page handlers

void MainMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    // Nothing
}

void ControlMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (var.VP) {
        case VP_BUTTON_PREPAREENTERKEY:
            switch(buttonValue) {
                case 5: // About
                    // Automatically handled
                    break;

                case 7: // Reset to factory settings
                    settings.reset();
                    settings.save();
                    break;

                case 9: // Back button
                    // TODO: should navigate automatically
                    break;
            }
            break;

        case VP_BUTTON_ADJUSTENTERKEY:
            ScreenHandler.HandleLEDToggle();
            break;
    }
}

void LevelingModeHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (var.VP) {
        case VP_BUTTON_BEDLEVELKEY:
            switch (buttonValue) {
                case 1:
                    ExtUI::injectCommands_P("G28\nG0 Z0");
                break;

                case 2:
                    // Increase Z-offset
                    ExtUI::smartAdjustAxis_steps(ExtUI::mmToWholeSteps(0.01, ExtUI::axis_t::Z), ExtUI::axis_t::Z, true);;
                    ScreenHandler.ForceCompleteUpdate();
                    settings.save();
                    break;

                case 3:
                    // Decrease Z-offset
                    ExtUI::smartAdjustAxis_steps(ExtUI::mmToWholeSteps(-0.01, ExtUI::axis_t::Z), ExtUI::axis_t::Z, true);;
                    ScreenHandler.ForceCompleteUpdate();
                    settings.save();
                    break;
            }

            break;

        case VP_BUTTON_MAINENTERKEY:
            // Go to leveling screen
            ExtUI::injectCommands_P("G28\nG29");
            break;
    }
}

void LevelingHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (var.VP) {
        case VP_BUTTON_BEDLEVELKEY:
            ScreenHandler.GotoScreen(DGUSLCD_SCREEN_ZOFFSET_LEVEL);

            break;
    }
}

void TempMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (var.VP) {
        case VP_BUTTON_ADJUSTENTERKEY:
            switch (buttonValue) {
                case 3:
                    ScreenHandler.HandleFanToggle();
                break;
            }

            break;
    }
}

void PrepareMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (var.VP) {
        case VP_BUTTON_PREPAREENTERKEY:
            // Disable steppers
            ScreenHandler.HandleMotorLockUnlock(var, &buttonValue);
        break;

        case VP_BUTTON_COOLDOWN:
            ScreenHandler.HandleAllHeatersOff(var, &buttonValue);
            break;

        case VP_BUTTON_TEMPCONTROL:
            switch (buttonValue) {
                case 5:
                    thermalManager.setTargetHotend(ui.material_preset[0].hotend_temp, 0);
                    thermalManager.setTargetBed(ui.material_preset[0].bed_temp);

                    break;

                case 6:
                    thermalManager.setTargetHotend(ui.material_preset[1].hotend_temp, 0);
                    thermalManager.setTargetBed(ui.material_preset[1].bed_temp);
                    break;
            }
            break;
    }

    ScreenHandler.ForceCompleteUpdate();
}

void TuneMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (var.VP) {
        case VP_BUTTON_ADJUSTENTERKEY:
            switch (buttonValue) {
                case 2:
                    ScreenHandler.GotoScreen(ExtUI::isPrintingFromMediaPaused() ? DGUSLCD_SCREEN_PRINT_PAUSED : DGUSLCD_SCREEN_PRINT_RUNNING);
                    break;

                case 3:
                    ScreenHandler.HandleFanToggle();
                break;

                case 4:
                    ScreenHandler.HandleLEDToggle();
                break;
            }
    }
}

void PrintRunningMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (var.VP) {
        case VP_BUTTON_ADJUSTENTERKEY:
            switch (buttonValue) {
                case 1:
                    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_TUNING);
                    break;
            }
        break;
    }
}

void PrintPausedMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (var.VP) {
        case VP_BUTTON_RESUMEPRINTKEY:
#if ENABLED(FILAMENT_RUNOUT_SENSOR)
            runout.reset();
#endif

            if (!ScreenHandler.HandlePendingUserConfirmation()) {
                ExtUI::resumePrint();
                ScreenHandler.GotoScreen(DGUSLCD_SCREEN_PRINT_RUNNING);
            }
            break;

        case VP_BUTTON_ADJUSTENTERKEY:
            switch (buttonValue) {
                case 1:
                    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_TUNING);
                    break;
            }
        break;
    }
}

void PrintPauseDialogHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (var.VP){
        case VP_BUTTON_PAUSEPRINTKEY:
            switch (buttonValue) {
                case 2:
                    ExtUI::pausePrint();
                    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_PRINT_PAUSED);
                    break;

                case 3:
                    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_PRINT_RUNNING);
                    break;
            }
            break;
    }
}

void FilamentRunoutHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (var.VP){
        case VP_BUTTON_RESUMEPRINTKEY:
            ExtUI::resumePrint();
            ScreenHandler.GotoScreen(DGUSLCD_SCREEN_PRINT_RUNNING);
        break;

        case VP_BUTTON_STOPPRINTKEY:
            ExtUI::stopPrint();
            ScreenHandler.GotoScreen(DGUSLCD_SCREEN_MAIN);
        break;
    }
}

void StopConfirmScreenHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (var.VP){
        case VP_BUTTON_STOPPRINTKEY:
            switch (buttonValue) {
                case 2:
                    ExtUI::stopPrint();
                    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_MAIN);
                break;

                case 3:
                    ScreenHandler.GotoScreen(ExtUI::isPrintingFromMediaPaused() ? DGUSLCD_SCREEN_PRINT_PAUSED : DGUSLCD_SCREEN_PRINT_RUNNING);
                break;
            }
        break;
    }
}

void PreheatSettingsScreenHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    switch (var.VP){
        case VP_BUTTON_PREPAREENTERKEY:
            // Save button, save settings and go back
            settings.save();
            ScreenHandler.GotoScreen(DGUSLCD_SCREEN_TEMP);
        break;

        case VP_BUTTON_COOLDOWN:
            // Back button, discard settings
            settings.load();
            break;
    }
}

 void change_filament_with_temp(PGM_P command, const uint16_t celsius) {
    thermalManager.setTargetHotend(celsius, ExtUI::H0);

    char cmd[32];
    sprintf_P(cmd, command, ScreenHandler.feed_amount);
    
    SERIAL_ECHOPAIR("Injecting command: ", cmd);
    ExtUI::injectCommands(cmd);

    SERIAL_ECHOPGM_P("- waiting for queue");
    queue.advance();

    SERIAL_ECHOPGM_P("- done");
}

void FeedHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
    if (var.VP != VP_BUTTON_HEATLOADSTARTKEY) return;

    // Common for load/unload -> determine minimum temperature
    uint16_t celsius = ExtUI::getActualTemp_celsius(ExtUI::H0);
    if (celsius < PREHEAT_1_TEMP_HOTEND) {
        celsius = PREHEAT_1_TEMP_HOTEND;
    }

    switch (buttonValue) {
        case 1:
            dgusdisplay.WriteVariable(VP_FEED_PROGRESS, static_cast<int16_t>(10));

            change_filament_with_temp(PSTR("M701 L%f"), celsius);

            dgusdisplay.WriteVariable(VP_FEED_PROGRESS, static_cast<int16_t>(0));
        break;

        case 2:
            dgusdisplay.WriteVariable(VP_FEED_PROGRESS, static_cast<int16_t>(10));

            change_filament_with_temp(PSTR("M702 U%f"), celsius);

            dgusdisplay.WriteVariable(VP_FEED_PROGRESS, static_cast<int16_t>(0));
        break;
    }

    ScreenHandler.ForceCompleteUpdate();
}

void MoveHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  if (var.VP != VP_BUTTON_MOVEKEY) return;

  switch (buttonValue) {
    case 1:
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_MOVE10MM);
      break;
    case 2:
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_MOVE1MM);
      break;
    case 3:
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_MOVE01MM);
      break;
    case 4:
      ExtUI::injectCommands_P("G28");
      break;
  }
}

// Register the page handlers
#define PAGE_HANDLER(SCRID, HDLRPTR) { .ScreenID=SCRID, .Handler=HDLRPTR },
const struct PageHandler PageHandlers[] PROGMEM = {
    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_MAIN, MainMenuHandler)

    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_CONTROL, ControlMenuHandler)

    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_ZOFFSET_LEVEL, LevelingModeHandler)
    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_LEVELING, LevelingHandler)

    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_TEMP, TempMenuHandler)
    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_TEMP_PLA, PreheatSettingsScreenHandler)
    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_TEMP_ABS, PreheatSettingsScreenHandler)

    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_TUNING, TuneMenuHandler)
    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_FEED, FeedHandler)
    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_MOVE01MM, MoveHandler)
    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_MOVE1MM, MoveHandler)
    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_MOVE10MM, MoveHandler)

    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_FILAMENTRUNOUT1, FilamentRunoutHandler)

    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_DIALOG_STOP, StopConfirmScreenHandler)

    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_PRINT_RUNNING, PrintRunningMenuHandler)
    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_PRINT_PAUSED, PrintPausedMenuHandler)

    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_DIALOG_PAUSE, PrintPauseDialogHandler)

    PAGE_HANDLER(DGUSLCD_Screens::DGUSLCD_SCREEN_PREPARE, PrepareMenuHandler)

    // Terminating
    PAGE_HANDLER(static_cast<DGUSLCD_Screens>(0) ,0)
};

void DGUSCrealityDisplay_HandleReturnKeyEvent(DGUS_VP_Variable &var, void *val_ptr) {
  const struct PageHandler *map = PageHandlers;
  const uint16_t *ret;
  const DGUSLCD_Screens current_screen = DGUSScreenHandler::getCurrentScreen();

  while ((ret = (uint16_t*) pgm_read_ptr(&(map->Handler)))) {
    if ((map->ScreenID) == current_screen) {
        unsigned short button_value = *static_cast<unsigned short*>(val_ptr);
        button_value = (button_value & 0xffU) << 8U | (button_value >> 8U);

        SERIAL_ECHOPAIR("Invoking handler for screen ", current_screen);
        SERIAL_ECHOLNPAIR("with VP=", var.VP, " value=", button_value);

        map->Handler(var, button_value);
        return;
    }

    map++;
  }
}

#endif
