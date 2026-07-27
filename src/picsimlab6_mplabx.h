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

#ifndef CPWINDOW6MPLABX
#define CPWINDOW6MPLABX

static const char project_xml[] =
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<project xmlns=\"http://www.netbeans.org/ns/project/1\">\n"
    "    <type>com.microchip.mplab.nbide.embedded.makeproject</type>\n"
    "    <configuration>\n"
    "        <data xmlns=\"http://www.netbeans.org/ns/make-project/1\">\n"
    "            <name>%s</name>\n"
    "            <creation-uuid>ea0a34a0-5fc7-4a39-9009-%012lx</creation-uuid>\n"
    "            <make-project-type>0</make-project-type>\n"
    "            <sourceEncoding>UTF-8</sourceEncoding>\n"
    "            <make-dep-projects/>\n"
    "            <sourceRootList/>\n"
    "            <confList>\n"
    "                <confElem>\n"
    "                    <name>default</name>\n"
    "                    <type>2</type>\n"
    "                </confElem>\n"
    "            </confList>\n"
    "            <formatting>\n"
    "                <project-formatting-style>false</project-formatting-style>\n"
    "            </formatting>\n"
    "        </data>\n"
    "    </configuration>\n"
    "</project>\n";

static const char configurations_xml[] =
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<configurationDescriptor version=\"65\">\n"
    "  <logicalFolder name=\"root\" displayName=\"root\" projectFiles=\"true\">\n"
    "    <logicalFolder name=\"HeaderFiles\"\n"
    "                   displayName=\"Header Files\"\n"
    "                   projectFiles=\"true\">\n"
    "    </logicalFolder>\n"
    "    <logicalFolder name=\"ExternalFiles\"\n"
    "                   displayName=\"Important Files\"\n"
    "                   projectFiles=\"true\">\n"
    "      <itemPath>Makefile</itemPath>\n"
    "    </logicalFolder>\n"
    "    <logicalFolder name=\"LinkerScript\"\n"
    "                   displayName=\"Linker Files\"\n"
    "                   projectFiles=\"true\">\n"
    "    </logicalFolder>\n"
    "    <logicalFolder name=\"SourceFiles\"\n"
    "                   displayName=\"Source Files\"\n"
    "                   projectFiles=\"true\">\n"
    "      <itemPath>main.c</itemPath>\n"
    "    </logicalFolder>\n"
    "  </logicalFolder>\n"
    "  <projectmakefile>Makefile</projectmakefile>\n"
    "  <confs>\n"
    "    <conf name=\"default\" type=\"2\">\n"
    "      <toolsSet>\n"
    "        <developmentServer>localhost</developmentServer>\n"
    "        <targetDevice>%s</targetDevice>\n"
    "        <targetHeader></targetHeader>\n"
    "        <targetPluginBoard></targetPluginBoard>\n"
    "        <platformTool>picsimlab</platformTool>\n"
    "        <languageToolchain>XC8</languageToolchain>\n"
    "        <platform>2</platform>\n"
    "      </toolsSet>\n"
    "    </conf>\n"
    "  </confs>\n"
    "</configurationDescriptor>\n";

static const char blink_mplabx[] =
    "/*\n"
    "  Simple example to blinking an LED using MPLAB X IDE.\n"
    "   To upload to PICSimLab:\n"
    "     1- Check if PICSimLab is running;\n"
    "     2- Toggle the PICSimLab Debug button;\n"
    "     3- Use the menu [Debug > Debug Main Project] in MPLABX IDE.\n"
    "*/\n"
    "\n"
    "\n"
    "#pragma config WDT = OFF \n"
    "\n"
    "#define _XTAL_FREQ 8000000L \n"
    "\n"
    "#include <xc.h>\n"
    "\n"
    "void main(void) {\n"
    "    TRISDbits.RD0 = 0;\n"
    "\n"
    "    while (1) {\n"
    "        LATDbits.LD0 = 1;\n"
    "        __delay_ms(500);\n"
    "        LATDbits.LD0 = 0;\n"
    "        __delay_ms(500);\n"
    "    }\n"
    "}\n";

#endif /*#CPWINDOW6MPLABX*/