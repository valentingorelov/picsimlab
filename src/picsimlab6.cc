/* ########################################################################

   PICSimLab - Programmable IC Simulator Laboratory

   ########################################################################

   Copyright (c) : 2010-2026 Luis Claudio Gambôa Lopes <lcgamboa@yahoo.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

// Project Wizard

#include "picsimlab6.h"
#include "picsimlab1.h"
#include "picsimlab6_d.cc"
#include "picsimlab6_mplabx.h"
#include "picsimlab6_vspio.h"

#include "lib/picsimlab.h"

CPWindow6 Window6;

#define OP_ONLY_CREATE 1
#define OP_CREATE_AND_OPEN (OP_ONLY_CREATE + 2)
#define OP_OPEN_EXISTING 4

// Implementation

void CPWindow6::_EvOnCreate(CControl* control) {
    SetTitle(((PICSimLab.GetInstanceNumber() > 0)
                  ? ("PICSimLab[" + std::to_string(PICSimLab.GetInstanceNumber()) + "] - ")
                  : ("PICSimLab - ")) +
             "Project Wizard");
}

void CPWindow6::_EvOnShow(CControl* control) {
    bname = PICSimLab.GetBoard()->GetName();
    pname = PICSimLab.GetBoard()->GetProcessorName();

    label1.SetText("Board: " + bname);
    label2.SetText("Microcontroller: " + pname);

    combo1.SetItems(PICSimLab.GetBoard()->GetSupportedIDEs());
    combo1.SetText(combo1.GetItem(0));
    combo1_EvOnComboChange(NULL);

    edit1.SetText("Untitled");
}

void CPWindow6::button1_EvMouseButtonClick(CControl* control, const uint button, const uint x, const uint y,
                                           const uint state) {
    dirdialog1.SetType(lxFD_SAVE | lxFD_CHANGE_DIR);
    operation = OP_ONLY_CREATE;
    dirdialog1.Run();
}

void CPWindow6::button2_EvMouseButtonClick(CControl* control, const uint button, const uint x, const uint y,
                                           const uint state) {
    dirdialog1.SetType(lxFD_SAVE | lxFD_CHANGE_DIR);
    if (!ide.compare("PlatformIO IDE for VSCode")) {
        dirdialog1.SetDirName(lxGetDocumentsDir("picsimlab") + "/PlatformIO/Projects/");
    } else if (!ide.compare("MPLAB X IDE")) {
        dirdialog1.SetDirName(lxGetHomeDir() + "/MPLABXProjects/");
    }
    operation = OP_CREATE_AND_OPEN;
    dirdialog1.Run();
}

void CPWindow6::button3_EvMouseButtonClick(CControl* control, const uint button, const uint x, const uint y,
                                           const uint state) {
    WDestroy();
}

void CPWindow6::button4_EvMouseButtonClick(CControl* control, const uint button, const uint x, const uint y,
                                           const uint state) {
    operation = OP_OPEN_EXISTING;
    if (!ide.compare("PlatformIO IDE for VSCode")) {
        dirdialog1.SetDirName(lxGetDocumentsDir("picsimlab") + "/PlatformIO/Projects/");
    } else if (!ide.compare("MPLAB X IDE")) {
        dirdialog1.SetDirName(lxGetHomeDir() + "/MPLABXProjects/");
    }
    dirdialog1.Run();
}

void CPWindow6::combo1_EvOnComboChange(CControl* control) {
    ide = combo1.GetText();

    if (!ide.compare("N/A")) {
        combo1.SetEnable(0);
        combo2.SetEnable(0);
        combo3.SetEnable(0);
    } else {
        combo1.SetEnable(1);
        combo2.SetEnable(1);
        combo3.SetEnable(1);

        if (!ide.compare("PlatformIO IDE for VSCode")) {
            if (!pname.compare("ESP32") || !pname.compare("ESP32-C3")) {
                combo2.SetItems("Arduino,IDF,");
            } else if (!pname.compare("stm32f103c8t6") || !pname.compare("stm32f103rbt6")) {
                combo2.SetItems("Arduino,CMSIS,");

            } else if (!pname.compare("C51")) {
                combo2.SetItems("None,");
            } else {
                combo2.SetItems("Arduino,");
            }
        } else if (!ide.compare("MPLAB X IDE")) {
            combo2.SetItems("XC8,");
        } else {
            combo2.SetItems("N/A,");
        }
    }

    combo1.SetEnable((combo1.GetItemsCount() == 1) ? 0 : 1);

    combo2.SetText(combo2.GetItem(0));
    combo2_EvOnComboChange(NULL);
}

void CPWindow6::combo2_EvOnComboChange(CControl* control) {
    framework = combo2.GetText();

    combo3.SetItems("N/A,");

    if (!framework.compare("N/A")) {
        combo2.SetEnable(0);
        combo3.SetEnable(0);
        button4.SetEnable(0);
    } else {
        combo2.SetEnable(1);
        combo3.SetEnable(1);
        button4.SetEnable(1);

        if (!ide.compare("PlatformIO IDE for VSCode")) {
            if (!framework.compare("Arduino")) {
                combo3.SetItems("Blink,");
            } else if (!framework.compare("IDF")) {
                combo3.SetItems("Blink,");
            } else if (!framework.compare("CMSIS")) {
                combo3.SetItems("Blink,");
            } else if (!framework.compare("None")) {
                combo3.SetItems("Blink,");
            }
        } else if (!ide.compare("MPLAB X IDE")) {
            combo3.SetItems("Blink,");
        }
    }

    combo2.SetEnable((combo2.GetItemsCount() == 1) ? 0 : 1);

    combo3.SetText(combo3.GetItem(0));

    ctemplate = combo3.GetText();

    if (ctemplate.compare("N/A")) {
        combo3.SetEnable(1);
        edit1.SetEnable(1);
        button1.SetEnable(1);
        button2.SetEnable(1);
    } else {
        combo3.SetEnable(0);
        edit1.SetEnable(0);
        button1.SetEnable(0);
        button2.SetEnable(0);
    }

    combo3.SetEnable((combo3.GetItemsCount() == 1) ? 0 : 1);
}

void CPWindow6::dirdialog1_EvOnClose(int retId) {
    if (retId) {
        if (operation & OP_ONLY_CREATE) {
            lxString prjdir = dirdialog1.GetDirName();
            prjdir += lxString("/") + edit1.GetText() + "/";

            if (PICSimLab.SystemCmd(PSC_DIREXISTS, prjdir.utf8_str())) {
                PICSimLab.RegisterError(
                    "PICSimLab", (const char*)(lxString("Project dir ") + prjdir + " already exists!").utf8_str());
                return;
            } else {
                if (PICSimLab.SystemCmd(PSC_CREATEDIR, prjdir.utf8_str()) == 0) {
                    if (!ide.compare("PlatformIO IDE for VSCode")) {
                        lxString fzip = PICSimLab.GetSharePath() + "prj_wizard/platformio.zip";
                        PICSimLab.SystemCmd(PSC_UNZIPDIR, fzip.utf8_str(), (void*)((const char*)prjdir.utf8_str()));

                        // board selection
                        int gdb_debug = 1;
                        int pio_test = 1;
                        lxString pioboard = "";
                        lxString pioplatform = "";
                        lxString pioframework = "";
                        lxString ledpin = "";
                        lxString hwpin = "";
                        lxString env_extra = "";
                        lxString monitor_rst = "";
                        lxString ftype = "";
                        lxString tbreak = "";
                        if (!bname.compare("Arduino Uno")) {
                            pioboard = "uno";
                            pioplatform = "atmelavr";
                            pioframework = "arduino";
                            ledpin = "13";
                            hwpin = "19";
                            ftype = "hex";
                            tbreak = "setup";
                        } else if (!bname.compare("Arduino Nano")) {
                            pioboard = "nanoatmega328";
                            pioplatform = "atmelavr";
                            pioframework = "arduino";
                            ledpin = "13";
                            hwpin = "17";
                            ftype = "hex";
                            tbreak = "setup";
                        } else if (!bname.compare("Arduino Mega")) {
                            pioboard = "megaatmega2560";
                            pioplatform = "atmelavr";
                            pioframework = "arduino";
                            ledpin = "13";
                            hwpin = "26";
                            ftype = "hex";
                            tbreak = "setup";
                        } else if (!bname.compare("Franzininho DIY")) {
                            pioboard = "attiny85";
                            pioplatform = "atmelavr";
                            pioframework = "arduino";
                            ledpin = "1";
                            hwpin = "6";
                            env_extra = "board_build.f_cpu = 16000000L\nbuild_flags = -DCLOCK_SOURCE=6\n";
                            ftype = "hex";
                            tbreak = "setup";
                        } else if (!bname.compare("Blue Pill")) {
                            pioboard = "bluepill_f103c8";
                            pioplatform = "ststm32";
                            if (!framework.compare("Arduino")) {
                                pioframework = "arduino";
                                tbreak = "setup";
                            } else {
                                pioframework = "cmsis";
                                pio_test = 0;
                                tbreak = "main";
                            }
                            ledpin = "PC13";
                            hwpin = "2";
                            monitor_rst = "       monitor system_reset\n";
                            ftype = "bin";
                        } else if (!bname.compare("STM32 H103")) {
                            pioboard = "olimex_f103";
                            pioplatform = "ststm32";
                            if (!framework.compare("Arduino")) {
                                pioframework = "arduino";
                                tbreak = "setup";
                            } else {
                                pioframework = "cmsis";
                                pio_test = 0;
                                tbreak = "main";
                            }
                            ledpin = "PC12";
                            hwpin = "53";
                            monitor_rst = "       monitor system_reset\n";
                            ftype = "bin";
                        } else if (!bname.compare("ESP32-DevKitC")) {
                            pioboard = "esp32dev";
                            pioplatform = "espressif32";
                            if (!framework.compare("Arduino")) {
                                pioframework = "arduino";
                                tbreak = "setup";
                            } else {
                                pioframework = "espidf";
                                tbreak = "app_main";
                            }
                            ledpin = "2";
                            hwpin = "24";
                            monitor_rst = "       monitor system_reset\n";
                            env_extra = "board_build.flash_mode = dio\nboard_upload.flash_size = 4MB\n";
                            ftype = "bin";
                        } else if (!bname.compare("ESP32-C3-DevKitC-02")) {
                            pioboard = "esp32-c3-devkitc-02";
                            pioplatform = "espressif32";
                            if (!framework.compare("Arduino")) {
                                pioframework = "arduino";
                                tbreak = "setup";
                            } else {
                                pioframework = "espidf";
                                tbreak = "app_main";
                            }
                            ledpin = "2";
                            hwpin = "27";
                            monitor_rst = "       monitor system_reset\n";
                            env_extra = "board_build.flash_mode = dio\nboard_upload.flash_size = 4MB\n";
                            ftype = "bin";
                        } else if (!bname.compare("uCboard")) {
                            if (!pname.compare("C51")) {
                                pioboard = "Generic8051";
                                pioplatform = "intel_mcs51";
                                pioframework = "";
                                ledpin = "P3_2";
                                hwpin = "12";
                                tbreak = "main";
                            } else if (!pname.compare("STM8S103")) {
                                pioboard = "stm8sblue";
                                pioplatform = "ststm8";
                                pioframework = "arduino";
                                ledpin = "4";
                                hwpin = "12";
                                env_extra = "board_build.f_cpu = 4000000L\n";
                                tbreak = "setup";
                            }
                            gdb_debug = 0;
                            pio_test = 0;
                            ftype = "hex";
                        } else {
                            PICSimLab.RegisterError("PICSimLab",
                                                    (const char*)("Not supported board: " + bname).c_str());
                            PICSimLab.SystemCmd(PSC_REMOVEDIR, (const char*)prjdir.utf8_str());
                            WDestroy();
                            return;
                        }

                        // main
                        if (!framework.compare("Arduino")) {
                            FILE* fmain = fopen_UTF8((prjdir + "src/main.cpp").utf8_str(), "w");
                            if (fmain == NULL) {
                                PICSimLab.RegisterError("PICSimLab", (const char*)(lxString("File ") + prjdir +
                                                                                   "src/main.cpp can't be open!")
                                                                         .utf8_str());
                                return;
                            }
                            fprintf(fmain, blink_code, (const char*)ledpin.c_str());
                            fclose(fmain);
                            if (!pname.compare("STM8S103")) {
                                PICSimLab.SystemCmd(PSC_RENAMEFILE, (const char*)(prjdir + "src/main.cpp").utf8_str(),
                                                    (void*)((const char*)(prjdir + "src/main.c").utf8_str()));
                                PICSimLab.SystemCmd(PSC_REMOVEFILE,
                                                    (const char*)(prjdir + "test/test_main.cpp").utf8_str());
                            }
                        } else if (!framework.compare("IDF")) {
                            PICSimLab.SystemCmd(PSC_RENAMEFILE, (prjdir + (const char*)"test/test_main.cpp").utf8_str(),
                                                (void*)((const char*)(prjdir + "test/test_main.c").utf8_str()));
                            PICSimLab.SystemCmd(PSC_RENAMEFILE, (const char*)(prjdir + "src/main.cpp").utf8_str(),
                                                (void*)((const char*)(prjdir + "src/main.c").utf8_str()));
                            FILE* fmain = fopen_UTF8((prjdir + "src/main.c").utf8_str(), "w");
                            if (fmain == NULL) {
                                PICSimLab.RegisterError(
                                    "PICSimLab",
                                    (const char*)(lxString("File ") + prjdir + "src/main.c can't be open!").utf8_str());
                                return;
                            }
                            fprintf(fmain, blink_idf_code, (const char*)ledpin.c_str());
                            fclose(fmain);

                            FILE* fsdkcfg = fopen_UTF8((prjdir + "sdkconfig.defaults").utf8_str(), "w");
                            if (fsdkcfg == NULL) {
                                PICSimLab.RegisterError("PICSimLab", (const char*)(lxString("File ") + prjdir +
                                                                                   "sdkconfig.defaults can't be open!")
                                                                         .utf8_str());
                                return;
                            }
                            fprintf(fsdkcfg, "CONFIG_ESPTOOLPY_FLASHSIZE_4MB=y\n");
                            fclose(fsdkcfg);
                        } else if (!framework.compare("CMSIS")) {
                            PICSimLab.SystemCmd(PSC_RENAMEFILE, (const char*)(prjdir + "src/main.cpp").utf8_str(),
                                                (void*)((const char*)(prjdir + "src/main.c").utf8_str()));
                            FILE* fmain = fopen_UTF8((prjdir + "src/main.c").utf8_str(), "w");
                            if (fmain == NULL) {
                                PICSimLab.RegisterError(
                                    "PICSimLab",
                                    (const char*)(lxString("File ") + prjdir + "src/main.c can't be open!").utf8_str());
                                return;
                            }
                            fprintf(fmain, blink_cmsis_code, (const char*)ledpin.c_str());
                            fclose(fmain);

                            PICSimLab.SystemCmd(PSC_REMOVEFILE,
                                                (const char*)(prjdir + "test/test_main.cpp").utf8_str());
                        } else if (!framework.compare("None")) {
                            PICSimLab.SystemCmd(PSC_RENAMEFILE, (const char*)(prjdir + "src/main.cpp").utf8_str(),
                                                (void*)((const char*)(prjdir + "src/main.c").utf8_str()));
                            FILE* fmain = fopen_UTF8((prjdir + "src/main.c").utf8_str(), "w");
                            if (fmain == NULL) {
                                PICSimLab.RegisterError(
                                    "PICSimLab",
                                    (const char*)(lxString("File ") + prjdir + "src/main.c can't be open!").utf8_str());
                                return;
                            }
                            fprintf(fmain, blink_c51_code, (const char*)ledpin.c_str());
                            fclose(fmain);

                            PICSimLab.SystemCmd(PSC_REMOVEFILE,
                                                (const char*)(prjdir + "test/test_main.cpp").utf8_str());
                        }

                        // platformio.ini
                        FILE* fpio = fopen_UTF8((prjdir + "platformio.ini").utf8_str(), "w");
                        if (fpio == NULL) {
                            PICSimLab.RegisterError(
                                "PICSimLab",
                                (const char*)(lxString("File ") + prjdir + "platformio.ini can't be open!").utf8_str());
                            return;
                        }
                        fprintf(fpio, platformio_ini, (const char*)pioboard.c_str(), (const char*)pioplatform.c_str(),
                                (const char*)pioboard.c_str(), (const char*)pioframework.c_str(),
                                (const char*)env_extra.c_str(), (const char*)pioboard.c_str(),
                                (const char*)ftype.c_str());
                        if (gdb_debug) {
                            fprintf(fpio, platformio_ini_dbg, (const char*)tbreak.c_str(),
                                    (const char*)monitor_rst.c_str(), (const char*)monitor_rst.c_str());
                        } else {
                            fprintf(fpio, "debug_tool = custom  #no debug support\n");
                        }

                        fclose(fpio);

                        // test
                        if (pio_test) {
                            FILE* ftest = fopen_UTF8((prjdir + "test/test_custom_runner.py").utf8_str(), "w");
                            if (ftest == NULL) {
                                PICSimLab.RegisterError("PICSimLab",
                                                        (const char*)(lxString("File ") + prjdir +
                                                                      "test/test_custom_runner.py can't be open!")
                                                            .utf8_str());
                                return;
                            }
                            fprintf(ftest, blink_test, (const char*)hwpin.c_str());
                            fclose(ftest);
                        } else {
                            PICSimLab.SystemCmd(PSC_REMOVEFILE,
                                                (const char*)(prjdir + "test/test_custom_runner.py").utf8_str());
                        }

                        PICSimLab.GetBoard()->SetPWActiveProject((const char*)prjdir.utf8_str());
                        PICSimLab.GetBoard()->SetPWProjectType((const char*)ide.c_str());
                        Window1.menu1_Code_Open_Active_Project.SetEnable(1);

                        if (operation == OP_CREATE_AND_OPEN) {
                            OpenProject(prjdir, ide);
                        }
                    } else if (!ide.compare("MPLAB X IDE")) {
                        lxString fzip = PICSimLab.GetSharePath() + "prj_wizard/mplabx.zip";
                        PICSimLab.SystemCmd(PSC_UNZIPDIR, fzip.utf8_str(), (void*)((const char*)prjdir.utf8_str()));

                        char prj_name[512];

                        if (strlen(strrchr(prjdir.utf8_str(), '/')) > 2) {
                            strncpy(prj_name, strrchr(prjdir.utf8_str(), '/'), 511);
                        } else {
                            char stmp[512];
                            strncpy(stmp, prjdir.utf8_str(), 512);
                            stmp[strlen(stmp) - 1] = 0;
                            strncpy(prj_name, strrchr(stmp, '/') + 1, 511);
                        }

                        // board selection
                        lxString mplabx_cfg = "";
                        lxString mplabx_freq = "";
                        lxString mplabx_tris = "";
                        lxString mplabx_pin = "";
                        if (!bname.compare("PICGenios")) {
                            if (strstr(pname.c_str(), "PIC18F")) {
                                if (!pname.compare("PIC18F45K50")) {
                                    mplabx_cfg = "#pragma config WDTEN = OFF";
                                } else if (!pname.compare("PIC18F47K40")) {
                                    mplabx_cfg = "#pragma config WDTE = OFF";
                                } else {
                                    mplabx_cfg = "#pragma config WDT = OFF";
                                }
                                mplabx_tris = "TRISDbits.TRISD0";
                                mplabx_pin = "LATDbits.LD0";
                            } else {
                                mplabx_cfg = "#pragma config WDTE = OFF";
                                mplabx_tris = "TRISDbits.TRISD0";
                                mplabx_pin = "PORTDbits.RD0";
                            }
                            mplabx_freq = "8000000L";
                        } else if (!bname.compare("McLab2")) {
                            if (strstr(pname.c_str(), "PIC18F")) {
                                if (!pname.compare("PIC18F45K50")) {
                                    mplabx_cfg = "#pragma config WDTEN = OFF";
                                } else if (!pname.compare("PIC18F47K40")) {
                                    mplabx_cfg = "#pragma config WDTE = OFF";
                                } else {
                                    mplabx_cfg = "#pragma config WDT = OFF";
                                }
                                mplabx_tris = "TRISBbits.TRISB3";
                                mplabx_pin = "LATBbits.LB3";
                            } else {
                                mplabx_cfg = "#pragma config WDTE = OFF";
                                mplabx_tris = "TRISBbits.TRISB3";
                                mplabx_pin = "PORTBbits.RB3";
                            }
                            mplabx_freq = "8000000L";
                        } else if (!bname.compare("McLab1")) {
                            mplabx_cfg = "#pragma config WDTE = OFF";
                            mplabx_tris = "TRISBbits.TRISB0";
                            mplabx_pin = "PORTBbits.RB0";
                            mplabx_freq = "4000000L";
                        } else {
                            PICSimLab.RegisterError("PICSimLab",
                                                    (const char*)("Not supported board: " + bname).c_str());
                            PICSimLab.SystemCmd(PSC_REMOVEDIR, (const char*)prjdir.utf8_str());
                            WDestroy();
                            return;
                        }

                        // main
                        FILE* fmain = fopen_UTF8((prjdir + "main.c").utf8_str(), "w");
                        if (fmain == NULL) {
                            PICSimLab.RegisterError(
                                "PICSimLab",
                                (const char*)(lxString("File ") + prjdir + "main.c can't be open!").utf8_str());
                            return;
                        }
                        fprintf(fmain, blink_mplabx, (const char*)mplabx_cfg.c_str(), (const char*)mplabx_freq.c_str(),
                                (const char*)mplabx_tris.c_str(), (const char*)mplabx_pin.c_str(),
                                (const char*)mplabx_pin.c_str());
                        fclose(fmain);

                        // project
                        FILE* fproj = fopen_UTF8((prjdir + "nbproject/project.xml").utf8_str(), "w");
                        if (fproj == NULL) {
                            PICSimLab.RegisterError("PICSimLab", (const char*)(lxString("File ") + prjdir +
                                                                               "nbproject/project.xml can't be open!")
                                                                     .utf8_str());
                            return;
                        }
                        fprintf(fproj, project_xml, prj_name, time(NULL));
                        fclose(fproj);

                        // configurations
                        FILE* fconf = fopen_UTF8((prjdir + "nbproject/configurations.xml").utf8_str(), "w");
                        if (fconf == NULL) {
                            PICSimLab.RegisterError("PICSimLab",
                                                    (const char*)(lxString("File ") + prjdir +
                                                                  "nbproject/configurations.xml can't be open!")
                                                        .utf8_str());
                            return;
                        }
                        fprintf(fconf, configurations_xml, (const char*)pname.c_str());
                        fclose(fconf);

                        PICSimLab.GetBoard()->SetPWActiveProject((const char*)prjdir.utf8_str());
                        PICSimLab.GetBoard()->SetPWProjectType((const char*)ide.c_str());
                        Window1.menu1_Code_Open_Active_Project.SetEnable(1);

                        if (operation == OP_CREATE_AND_OPEN) {
                            OpenProject(prjdir, ide);
                        }

                    } else {
                        PICSimLab.RegisterError(
                            "PICSimLab",
                            (const char*)(lxString("IDE of type [") + ide + "] not supported!").utf8_str());
                        WDestroy();
                        return;
                    }
                } else {
                    PICSimLab.RegisterError(
                        "PICSimLab",
                        (const char*)(lxString("Project dir ") + prjdir + " can't be created!").utf8_str());
                    return;
                }
            }
        } else if (operation == OP_OPEN_EXISTING) {
            lxString prjdir = dirdialog1.GetDirName();
            PICSimLab.GetBoard()->SetPWActiveProject((const char*)prjdir.utf8_str());
            PICSimLab.GetBoard()->SetPWProjectType((const char*)ide.c_str());
            Window1.menu1_Code_Open_Active_Project.SetEnable(1);
            OpenProject(prjdir, ide);
        }
    }
    WDestroy();
}

void CPWindow6::filedialog1_EvOnClose(const int retId) {
    if (retId) {
        lxString type = PICSimLab.GetBoard()->GetPWProjectType();
        if (!type.compare("PlatformIO IDE for VSCode")) {
            PICSimLab.SetPWVscodePath((const char*)filedialog1.GetFileName().utf8_str());
        } else if (!type.compare("MPLAB X IDE")) {
            PICSimLab.SetPWMplabxPath((const char*)filedialog1.GetFileName().utf8_str());
        } else {
            PICSimLab.RegisterError("PICSimLab", "IDE type not supported!");
            return;
        }
        OpenProject(PICSimLab.GetBoard()->GetPWActiveProject(), PICSimLab.GetBoard()->GetPWProjectType());
    }
}

int CPWindow6::OpenProject(lxString path, lxString type) {
    if (!type.compare("PlatformIO IDE for VSCode")) {
        char vscode_path[1024];

        if (!PICSimLab.SystemCmd(PSC_DIREXISTS, path.utf8_str())) {
            PICSimLab.RegisterError("PICSimLab", (const char*)("Project dir not found!\n" + path).utf8_str());
            PICSimLab.GetBoard()->SetPWActiveProject(" ");
            Window1.menu1_Code_Open_Active_Project.SetEnable(0);
            return 1;
        }

        strncpy(vscode_path, PICSimLab.GetPWVscodePath().c_str(), 1023);

        if (!PICSimLab.SystemCmd(PSC_FILEEXISTS, vscode_path)) {
#ifdef _WIN_

            lxString vscode_user_path = "";
            strncpy(vscode_path, (const char*)lxGetUserDataDir("picsimlab"), 1023);
            printf("%s\n", (const char*)vscode_path);
            char* ptr = strstr(vscode_path, "AppData\\");
            if (ptr) {
                ptr[8] = 0;
                vscode_user_path = (lxString(vscode_path) + "Local\\Programs\\Microsoft VS Code\\Code.exe").utf8_str();
            }

            if (PICSimLab.SystemCmd(PSC_FILEEXISTS, (const char*)vscode_user_path.utf8_str())) {
                strncpy(vscode_path, vscode_user_path.utf8_str(), 1023);
            } else if (PICSimLab.SystemCmd(PSC_FILEEXISTS, "C:\\Program Files\\Microsoft VS Code\\Code.exe")) {
                strncpy(vscode_path, "C:\\Program Files\\Microsoft VS Code\\Code.exe", 1023);
            } else {
                if (Dialog_sz("VS code executable not found!\n Search on disk?", 400, 200)) {
                    filedialog1.SetFileName(vscode_path);
                    filedialog1.SetFilter(lxT("All Files (*.exe)|*.exe"));
                    filedialog1.Run();
                }
                return 1;
            }
#else

            if (PICSimLab.SystemCmd(PSC_FILEEXISTS, "/usr/bin/code")) {
                strncpy(vscode_path, "/usr/bin/code", 1023);
            } else if (PICSimLab.SystemCmd(PSC_FILEEXISTS, "/snap/bin/code")) {
                strncpy(vscode_path, "/snap/bin/code", 1023);
            } else {
                if (Dialog_sz("VS code executable not found!\n Search on disk?", 400, 200)) {
                    filedialog1.SetFileName(vscode_path);
                    filedialog1.SetFilter(lxT("All Files (*)|*"));
                    filedialog1.Run();
                }
                return 1;
            }

#endif
            PICSimLab.SetPWVscodePath(vscode_path);
        }

        printf("PICSimLab: Open project [%s]\n", (const char*)(lxString(vscode_path) + " \"" + path + "\"").utf8_str());
        lxExecute((lxString(vscode_path) + " \"" + path + "\""));

        return 0;
    } else if (!type.compare("MPLAB X IDE")) {
        char mplabx_path[1024];

        if (!PICSimLab.SystemCmd(PSC_DIREXISTS, path.utf8_str())) {
            PICSimLab.RegisterError("PICSimLab", (const char*)("Project dir not found!\n" + path).utf8_str());
            PICSimLab.GetBoard()->SetPWActiveProject(" ");
            Window1.menu1_Code_Open_Active_Project.SetEnable(0);
            return 1;
        }

        strncpy(mplabx_path, PICSimLab.GetPWMplabxPath().c_str(), 1023);

        if (!PICSimLab.SystemCmd(PSC_FILEEXISTS, mplabx_path)) {
#ifdef _WIN_
            if (PICSimLab.SystemCmd(PSC_FILEEXISTS,
                                    "C:/Program Files/Microchip/MPLABX/v6.20/mplab_platform/bin/mplab_ide64.exe")) {
                strncpy(mplabx_path, "C:/Program Files/Microchip/MPLABX/v6.20/mplab_platform/bin/mplab_ide64.exe",
                        1023);
            } else {
                if (Dialog_sz("MPLAB X IDE executable not found!\n Search on disk?", 400, 200)) {
                    filedialog1.SetDir("C:/Program Files/Microchip/MPLABX/");
                    filedialog1.SetFilter(lxT("All Files (*.exe)|*.exe"));
                    filedialog1.Run();
                }
                return 1;
            }
#else
            if (PICSimLab.SystemCmd(PSC_FILEEXISTS, "/opt/microchip/mplabx/v6.20/mplab_platform/bin/mplab_ide")) {
                strncpy(mplabx_path, "/opt/microchip/mplabx/v6.20/mplab_platform/bin/mplab_ide", 1023);
            } else {
                if (Dialog_sz("MPLAB X IDE executable not found!\n Search on disk?", 400, 200)) {
                    filedialog1.SetDir("/opt/microchip/mplabx/");
                    filedialog1.SetFilter(lxT("All Files (*)|*"));
                    filedialog1.Run();
                }
                return 1;
            }

#endif
            PICSimLab.SetPWMplabxPath(mplabx_path);
        }

        printf("PICSimLab: Open project [%s]\n",
               (const char*)(lxString(mplabx_path) + " --open \"" + path + "\"").utf8_str());
        lxExecute((lxString(mplabx_path) + " --open \"" + path + "\""));

        return 0;
    } else {
        PICSimLab.RegisterError("PICSimLab", "Project type not supported!");
        Window1.menu1_Code_Open_Active_Project.SetEnable(0);
        Window1.menu1_Code_Project_Wizard.SetEnable(0);
    }
    return -1;
}
