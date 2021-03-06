//==============================================================================
// Copyright (c) 2010-2020 Advanced Micro Devices, Inc. All rights reserved.
/// \author AMD Developer Tools Team
/// \file
/// \brief  Device info utils class
//==============================================================================

#ifdef _WIN32
    #include <Windows.h>
#endif
#ifdef _LINUX
    #include <dlfcn.h>
#endif

#include <cassert>
#include "DeviceInfoUtils.h"

using namespace std;

static GDT_GfxCardInfo gs_cardInfo[] =
{
    { GDT_TAHITI_XT,  0x6798, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Tahiti", "AMD Radeon R9 200 / HD 7900 Series" },
    { GDT_TAHITI_XT,  0x6799, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Tahiti", "AMD Radeon HD 7900 Series" },
    { GDT_TAHITI_PRO, 0x679A, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Tahiti", "AMD Radeon HD 7900 Series" },
    { GDT_TAHITI_PRO, 0x679B, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Tahiti", "AMD Radeon HD 7900 Series" },
    { GDT_TAHITI_PRO, 0x679E, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Tahiti", "AMD Radeon HD 7800 Series" },
    { GDT_TAHITI_XT,  0x6780, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Tahiti", "AMD FirePro W9000" },
    { GDT_TAHITI_PRO, 0x6784, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Tahiti", "ATI FirePro V (FireGL V) Graphics Adapter" },
    { GDT_TAHITI_XT,  0x6788, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Tahiti", "ATI FirePro V (FireGL V) Graphics Adapter" },
    { GDT_TAHITI_PRO, 0x678A, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Tahiti", "AMD FirePro W8000" },

    { GDT_PITCAIRN_XT,  0x6818, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Pitcairn", "AMD Radeon HD 7800 Series" },
    { GDT_PITCAIRN_PRO, 0x6819, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Pitcairn", "AMD Radeon HD 7800 Series" },
    { GDT_PITCAIRN_XT,  0x6808, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Pitcairn", "AMD FirePro W7000" },
    { GDT_PITCAIRN_XT,  0x6809, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Pitcairn", "ATI FirePro W5000" },
    { GDT_PITCAIRN_XT,  0x684C, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Pitcairn", "ATI FirePro V(FireGL V) Graphics Adapter" },
    { GDT_PITCAIRN_XT,  0x6800, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Pitcairn", "AMD Radeon HD 7970M" },
    { GDT_PITCAIRN_PRO, 0x6801, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Pitcairn", "AMD Radeon(TM) HD8970M" },
    { GDT_PITCAIRN_XT,  0x6806, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Pitcairn", "AMD Radeon (TM) R9 M290X" },
    { GDT_PITCAIRN_XT,  0x6810, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Pitcairn", "AMD Radeon R9 200 Series" },
    { GDT_PITCAIRN_XT,  0x6810, 0x81, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Pitcairn", "AMD Radeon (TM) R9 370 Series" },
    { GDT_PITCAIRN_PRO, 0x6811, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Pitcairn", "AMD Radeon R9 200 Series" },
    { GDT_PITCAIRN_PRO, 0x6811, 0x81, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Pitcairn", "AMD Radeon (TM) R7 370 Series" },

    { GDT_CAPEVERDE_XT,  0x6820, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon R9 M275X" },
    { GDT_CAPEVERDE_XT,  0x6820, 0x81, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon (TM) R9 M375" },
    { GDT_CAPEVERDE_XT,  0x6820, 0x83, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon (TM) R9 M375X" },
    { GDT_CAPEVERDE_XT,  0x6821, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon R9 M200X Series" },
    { GDT_CAPEVERDE_XT,  0x6821, 0x83, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon R9 (TM) M370X" },
    { GDT_CAPEVERDE_XT,  0x6821, 0x87, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon (TM) R7 M380" },
    { GDT_CAPEVERDE_PRO, 0x6822, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon E8860" },
    { GDT_CAPEVERDE_PRO, 0x6823, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon R9 M200X Series" },
    { GDT_CAPEVERDE_XT,  0x6825, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon HD 7800M Series" },
    { GDT_CAPEVERDE_PRO, 0x6826, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon HD 7700M Series" },
    { GDT_CAPEVERDE_PRO, 0x6827, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon HD 7800M Series" },
    { GDT_CAPEVERDE_XT,  0x682B, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon HD 8800M Series" },
    { GDT_CAPEVERDE_XT,  0x682B, 0x87, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon (TM) R9 M360" },
    { GDT_CAPEVERDE_XT,  0x682D, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon HD 7700M Series" },
    { GDT_CAPEVERDE_PRO, 0x682F, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon HD 7700M Series" },

    { GDT_CAPEVERDE_XT,  0x6828, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD FirePro W600" },
    { GDT_CAPEVERDE_PRO, 0x682C, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD FirePro W4100" },
    { GDT_CAPEVERDE_XT,  0x6830, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon 7800M Series" },
    { GDT_CAPEVERDE_XT,  0x6831, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon 7700M Series" },
    { GDT_CAPEVERDE_PRO, 0x6835, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon R7 Series / HD 9000 Series" },
    { GDT_CAPEVERDE_XT,  0x6837, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon HD 7700 Series" },
    { GDT_CAPEVERDE_XT,  0x683D, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon HD 7700 Series" },
    { GDT_CAPEVERDE_PRO, 0x683F, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Capeverde", "AMD Radeon HD 7700 Series" },

    // Oland
    { GDT_OLAND, 0x6608, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD FirePro W2100" },
    { GDT_OLAND, 0x6610, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon R7 200 Series" },
    { GDT_OLAND, 0x6610, 0x81, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon (TM) R7 350" },
    { GDT_OLAND, 0x6610, 0x83, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon (TM) R5 340" },
    { GDT_OLAND, 0x6610, 0x87, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon R7 200 Series" },
    { GDT_OLAND, 0x6611, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon R7 200 Series" },
    { GDT_OLAND, 0x6611, 0x87, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon R7 200 Series" },
    { GDT_OLAND, 0x6613, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon R7 200 Series" },
    { GDT_OLAND, 0x6617, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon R7 240 Series" },
    { GDT_OLAND, 0x6617, 0x87, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon R7 200 Series" },
    { GDT_OLAND, 0x6617, 0xC7, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon R7 240 Series" },

    // Mars (Mobile Oland)
    { GDT_OLAND, 0x6600, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon HD 8600/8700M" },
    { GDT_OLAND, 0x6600, 0x81, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon (TM) R7 M370" },
    { GDT_OLAND, 0x6601, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon (TM) HD 8500M/8700M" },
    { GDT_OLAND, 0x6604, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon R7 M265 Series" },
    { GDT_OLAND, 0x6604, 0x81, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon (TM) R7 M350" },
    { GDT_OLAND, 0x6605, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon R7 M260 Series" },
    { GDT_OLAND, 0x6605, 0x81, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon (TM) R7 M340" },
    { GDT_OLAND, 0x6606, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon HD 8790M" },
    { GDT_OLAND, 0x6607, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Oland", "AMD Radeon R5 M240" },

    // Hainan
    { GDT_HAINAN, 0x6660, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Hainan", "AMD Radeon HD 8600M Series" },
    { GDT_HAINAN, 0x6660, 0x81, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Hainan", "AMD Radeon (TM) R5 M335" },
    { GDT_HAINAN, 0x6660, 0x83, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Hainan", "AMD Radeon (TM) R5 M330" },
    { GDT_HAINAN, 0x6663, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Hainan", "AMD Radeon HD 8500M Series" },
    { GDT_HAINAN, 0x6663, 0x83, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Hainan", "AMD Radeon (TM) R5 M320" },
    { GDT_HAINAN, 0x6664, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Hainan", "AMD Radeon R5 M200 Series" },
    { GDT_HAINAN, 0x6665, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Hainan", "AMD Radeon R5 M230 Series" },
    { GDT_HAINAN, 0x6665, 0x83, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Hainan", "AMD Radeon (TM) R5 M320" },
    { GDT_HAINAN, 0x6665, 0xC3, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Hainan", "AMD Radeon R5 M435" },
    { GDT_HAINAN, 0x6666, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Hainan", "AMD Radeon R5 M200 Series" },
    { GDT_HAINAN, 0x6667, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Hainan", "AMD Radeon R5 M200 Series" },
    { GDT_HAINAN, 0x666F, 0x00, GDT_HW_GENERATION_SOUTHERNISLAND, false, "Hainan", "AMD Radeon HD 8500M" },

    // Bonaire
    { GDT_BONAIRE, 0x6649, 0x00, GDT_HW_GENERATION_SEAISLAND, false, "Bonaire", "AMD FirePro W5100" },
    { GDT_BONAIRE, 0x6658, 0x00, GDT_HW_GENERATION_SEAISLAND, false, "Bonaire", "AMD Radeon R7 200 Series" },
    { GDT_BONAIRE, 0x665C, 0x00, GDT_HW_GENERATION_SEAISLAND, false, "Bonaire", "AMD Radeon HD 7700 Series" },
    { GDT_BONAIRE, 0x665D, 0x00, GDT_HW_GENERATION_SEAISLAND, false, "Bonaire", "AMD Radeon R7 200 Series" },
    { GDT_BONAIRE, 0x665F, 0x81, GDT_HW_GENERATION_SEAISLAND, false, "Bonaire", "AMD Radeon (TM) R7 360 Series" },
    { GDT_BONAIRE, 0x665F, 0x81, GDT_HW_GENERATION_SEAISLAND, false, "Bonaire", "AMD Radeon (TM) R7 360 Series" },

    // Saturn (mobile Bonaire)
    { GDT_BONAIRE, 0x6640, 0x00, GDT_HW_GENERATION_SEAISLAND, false, "Bonaire", "AMD Radeon HD 8950" },
    { GDT_BONAIRE, 0x6640, 0x80, GDT_HW_GENERATION_SEAISLAND, false, "Bonaire", "AMD Radeon (TM) R9 M380" },
    { GDT_BONAIRE, 0x6646, 0x00, GDT_HW_GENERATION_SEAISLAND, false, "Bonaire", "AMD Radeon R9 M280X" },
    { GDT_BONAIRE, 0x6646, 0x80, GDT_HW_GENERATION_SEAISLAND, false, "Bonaire", "AMD Radeon (TM) R9 M385" },
    { GDT_BONAIRE, 0x6647, 0x00, GDT_HW_GENERATION_SEAISLAND, false, "Bonaire", "AMD Radeon R9 M200X Series" },
    { GDT_BONAIRE, 0x6647, 0x80, GDT_HW_GENERATION_SEAISLAND, false, "Bonaire", "AMD Radeon (TM) R9 M380" },

    // Hawaii
    { GDT_HAWAII, 0x67A0, 0x00, GDT_HW_GENERATION_SEAISLAND, false, "Hawaii", "AMD FirePro W9100" },
    { GDT_HAWAII, 0x67A1, 0x00, GDT_HW_GENERATION_SEAISLAND, false, "Hawaii", "AMD FirePro W8100" },
    { GDT_HAWAII, 0x67B0, 0x00, GDT_HW_GENERATION_SEAISLAND, false, "Hawaii", "AMD Radeon R9 200 Series" },
    { GDT_HAWAII, 0x67B0, 0x80, GDT_HW_GENERATION_SEAISLAND, false, "Hawaii", "AMD Radeon (TM) R9 390 Series" },
    { GDT_HAWAII, 0x67B1, 0x00, GDT_HW_GENERATION_SEAISLAND, false, "Hawaii", "AMD Radeon R9 200 Series" },
    { GDT_HAWAII, 0x67B1, 0x80, GDT_HW_GENERATION_SEAISLAND, false, "Hawaii", "AMD Radeon (TM) R9 390 Series" },
    { GDT_HAWAII, 0x67B9, 0x00, GDT_HW_GENERATION_SEAISLAND, false, "Hawaii", "AMD Radeon R9 200 Series" },

    // Kaveri -- will probably need multiple entries in g_deviceInfo for these
    { GDT_SPECTRE_LITE, 0x1309, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R7 Graphics" },
    { GDT_SPECTRE_LITE, 0x130A, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R6 Graphics" },
    { GDT_SPECTRE,      0x130C, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R7 Graphics" },
    { GDT_SPECTRE_LITE, 0x130D, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R6 Graphics" },
    { GDT_SPECTRE_SL,   0x130E, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R5 Graphics" },
    { GDT_SPECTRE,      0x130F, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R7 Graphics" },
    { GDT_SPECTRE,      0x130F, 0xD4, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R7 Graphics" },
    { GDT_SPECTRE,      0x130F, 0xD5, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R7 Graphics" },
    { GDT_SPECTRE,      0x130F, 0xD6, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R7 Graphics" },
    { GDT_SPECTRE,      0x130F, 0xD7, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R7 Graphics" },
    { GDT_SPECTRE_LITE, 0x1313, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R7 Graphics" },
    { GDT_SPECTRE_LITE, 0x1313, 0xD4, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R7 Graphics" },
    { GDT_SPECTRE_LITE, 0x1313, 0xD5, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R7 Graphics" },
    { GDT_SPECTRE_LITE, 0x1313, 0xD6, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R7 Graphics" },
    { GDT_SPECTRE_SL,   0x1315, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R5 Graphics" },
    { GDT_SPECTRE_SL,   0x1315, 0xD4, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R5 Graphics" },
    { GDT_SPECTRE_SL,   0x1315, 0xD5, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R5 Graphics" },
    { GDT_SPECTRE_SL,   0x1315, 0xD6, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R5 Graphics" },
    { GDT_SPECTRE_SL,   0x1315, 0xD7, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R5 Graphics" },
    { GDT_SPECTRE_SL,   0x1318, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R5 Graphics" },
    { GDT_SPECTRE,      0x131C, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R7 Graphics" },
    { GDT_SPECTRE_LITE, 0x131D, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R6 Graphics" },
    { GDT_SPOOKY,       0x130B, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R4 Graphics" },
    { GDT_SPOOKY,       0x1316, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Spooky", "AMD Radeon(TM) R5 Graphics" },
    { GDT_SPOOKY,       0x131B, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Spectre", "AMD Radeon(TM) R4 Graphics" },

    // Kabini
    { GDT_KALINDI, 0x9830, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Kalindi", "AMD Radeon HD 8400 / R3 Series" },
    { GDT_KALINDI, 0x9831, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Kalindi", "AMD Radeon(TM) HD 8400E" },
    { GDT_KALINDI, 0x9832, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Kalindi", "AMD Radeon HD 8330" },
    { GDT_KALINDI, 0x9833, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Kalindi", "AMD Radeon(TM) HD 8330E" },
    { GDT_KALINDI, 0x9834, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Kalindi", "AMD Radeon HD 8210" },
    { GDT_KALINDI, 0x9835, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Kalindi", "AMD Radeon(TM) HD 8210E" },
    { GDT_KALINDI, 0x9836, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Kalindi", "AMD Radeon HD 8200 / R3 Series" },
    { GDT_KALINDI, 0x9837, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Kalindi", "AMD Radeon(TM) HD 8280E" },
    { GDT_KALINDI, 0x9838, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Kalindi", "AMD Radeon HD 8200 / R3 series" },

    // Temash
    { GDT_KALINDI, 0x9839, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Kalindi", "AMD Radeon HD 8180" },
    { GDT_KALINDI, 0x983D, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Kalindi", "AMD Radeon HD 8250" },

    // Beema
    { GDT_KALINDI, 0x9850, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R3 Graphics" },
    { GDT_KALINDI, 0x9850, 0x03, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R3 Graphics" },
    { GDT_KALINDI, 0x9850, 0x40, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R2 Graphics" },
    { GDT_KALINDI, 0x9850, 0x45, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R3 Graphics" },
    { GDT_KALINDI, 0x9851, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R4 Graphics" },
    { GDT_KALINDI, 0x9851, 0x01, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R5E Graphics" },
    { GDT_KALINDI, 0x9851, 0x05, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R5 Graphics" },
    { GDT_KALINDI, 0x9851, 0x06, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R5E Graphics" },
    { GDT_KALINDI, 0x9851, 0x40, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R4 Graphics" },
    { GDT_KALINDI, 0x9851, 0x45, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R5 Graphics" },
    { GDT_KALINDI, 0x9852, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R2 Graphics" },
    { GDT_KALINDI, 0x9852, 0x40, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) E1 Graphics" },
    { GDT_KALINDI, 0x9853, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R2 Graphics" },
    { GDT_KALINDI, 0x9853, 0x01, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R4E Graphics" },
    { GDT_KALINDI, 0x9853, 0x03, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R2 Graphics" },
    { GDT_KALINDI, 0x9853, 0x05, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R1E Graphics" },
    { GDT_KALINDI, 0x9853, 0x06, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R1E Graphics" },
    { GDT_KALINDI, 0x9853, 0x40, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R2 Graphics" },

    // Mullins
    { GDT_KALINDI, 0x9853, 0x07, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon R1E Graphics" },
    { GDT_KALINDI, 0x9853, 0x08, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon R1E Graphics" },
    { GDT_KALINDI, 0x9854, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R3 Graphics" },
    { GDT_KALINDI, 0x9854, 0x01, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R3E Graphics" },
    { GDT_KALINDI, 0x9854, 0x02, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R3 Graphics" },
    { GDT_KALINDI, 0x9854, 0x05, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R2 Graphics" },
    { GDT_KALINDI, 0x9854, 0x06, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R4 Graphics" },
    { GDT_KALINDI, 0x9854, 0x07, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R3 Graphics" },
    { GDT_KALINDI, 0x9855, 0x02, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R6 Graphics" },
    { GDT_KALINDI, 0x9855, 0x05, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R4 Graphics" },
    { GDT_KALINDI, 0x9856, 0x07, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R1E Graphics" },
    { GDT_KALINDI, 0x9856, 0x00, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R2 Graphics" },
    { GDT_KALINDI, 0x9856, 0x01, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R2E Graphics" },
    { GDT_KALINDI, 0x9856, 0x02, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R2 Graphics" },
    { GDT_KALINDI, 0x9856, 0x05, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R1E Graphics" },
    { GDT_KALINDI, 0x9856, 0x06, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R2 Graphics" },
    { GDT_KALINDI, 0x9856, 0x07, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon(TM) R1E Graphics" },
    { GDT_KALINDI, 0x9856, 0x08, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon R1E Graphics" },
    { GDT_KALINDI, 0x9856, 0x13, GDT_HW_GENERATION_SEAISLAND, true, "Mullins", "AMD Radeon R1E Graphics" },

    // Iceland/Topaz
    { GDT_ICELAND, 0x6900, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Iceland", "AMD Radeon R7 M260" },
    { GDT_ICELAND, 0x6900, 0x81, GDT_HW_GENERATION_VOLCANICISLAND, false, "Iceland", "AMD Radeon (TM) R7 M360" },
    { GDT_ICELAND, 0x6900, 0x83, GDT_HW_GENERATION_VOLCANICISLAND, false, "Iceland", "AMD Radeon (TM) R7 M340" },
    { GDT_ICELAND, 0x6900, 0xC1, GDT_HW_GENERATION_VOLCANICISLAND, false, "Iceland", "AMD Radeon R5 M465 Series" },
    { GDT_ICELAND, 0x6900, 0xC3, GDT_HW_GENERATION_VOLCANICISLAND, false, "Iceland", "AMD Radeon R5 M445 Series" },
    { GDT_ICELAND, 0x6900, 0xD1, GDT_HW_GENERATION_VOLCANICISLAND, false, "Iceland", "Radeon 530 Series" },
    { GDT_ICELAND, 0x6900, 0xD3, GDT_HW_GENERATION_VOLCANICISLAND, false, "Iceland", "Radeon 530 Series" },
    { GDT_ICELAND, 0x6901, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Iceland", "AMD Radeon R5 M255" },
    { GDT_ICELAND, 0x6902, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Iceland", "AMD Radeon Series" },
    { GDT_ICELAND, 0x6907, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Iceland", "AMD Radeon R5 M255" },
    { GDT_ICELAND, 0x6907, 0x87, GDT_HW_GENERATION_VOLCANICISLAND, false, "Iceland", "AMD Radeon (TM) R5 M315" },

    // Tonga
    { GDT_TONGA, 0x6920, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD RADEON R9 M395X" },
    { GDT_TONGA, 0x6920, 0x01, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD RADEON R9 M390X" },
    { GDT_TONGA, 0x6921, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD Radeon (TM) R9 M390X" },
    { GDT_TONGA, 0x6929, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD FirePro S7150" },
    { GDT_TONGA, 0x6929, 0x01, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD FirePro S7100X" },
    { GDT_TONGA, 0x692B, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD FirePro W7100" },
    { GDT_TONGA, 0x692F, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD MxGPU" },
    { GDT_TONGA, 0x692F, 0x01, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD MxGPU" },
    { GDT_TONGA, 0x6930, 0xF0, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD MxGPU" },
    { GDT_TONGA, 0x6938, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD Radeon R9 200 Series" },
    { GDT_TONGA, 0x6938, 0xF1, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD Radeon (TM) R9 380 Series" },
    { GDT_TONGA, 0x6938, 0xF0, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD Radeon R9 200 Series" },
    { GDT_TONGA, 0x6939, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD Radeon R9 200 Series" },
    { GDT_TONGA, 0x6939, 0xF0, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD Radeon R9 200 Series" },
    { GDT_TONGA, 0x6939, 0xF1, GDT_HW_GENERATION_VOLCANICISLAND, false, "Tonga", "AMD Radeon (TM) R9 380 Series" },

    // Carrizo
    { GDT_CARRIZO, 0x9874, 0xC4, GDT_HW_GENERATION_VOLCANICISLAND, true, "Carrizo", "AMD Radeon R7 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xC5, GDT_HW_GENERATION_VOLCANICISLAND, true, "Carrizo", "AMD Radeon R6 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xC6, GDT_HW_GENERATION_VOLCANICISLAND, true, "Carrizo", "AMD Radeon R6 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xC7, GDT_HW_GENERATION_VOLCANICISLAND, true, "Carrizo", "AMD Radeon R5 Graphics" },
    { GDT_CARRIZO_EMB, 0x9874, 0x81, GDT_HW_GENERATION_VOLCANICISLAND, true, "Carrizo", "AMD Radeon R6 Graphics" },
    { GDT_CARRIZO_EMB, 0x9874, 0x84, GDT_HW_GENERATION_VOLCANICISLAND, true, "Carrizo", "AMD Radeon R7 Graphics" },
    { GDT_CARRIZO_EMB, 0x9874, 0x85, GDT_HW_GENERATION_VOLCANICISLAND, true, "Carrizo", "AMD Radeon R6 Graphics" },
    { GDT_CARRIZO_EMB, 0x9874, 0x87, GDT_HW_GENERATION_VOLCANICISLAND, true, "Carrizo", "AMD Radeon R5 Graphics" },
    { GDT_CARRIZO_EMB, 0x9874, 0x88, GDT_HW_GENERATION_VOLCANICISLAND, true, "Carrizo", "AMD Radeon R7E Graphics" },
    { GDT_CARRIZO_EMB, 0x9874, 0x89, GDT_HW_GENERATION_VOLCANICISLAND, true, "Carrizo", "AMD Radeon R6E Graphics" },
    { GDT_CARRIZO, 0x9874, 0xC8, GDT_HW_GENERATION_VOLCANICISLAND, true, "Bristol Ridge", "AMD Radeon R7 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xC9, GDT_HW_GENERATION_VOLCANICISLAND, true, "Bristol Ridge", "AMD Radeon R7 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xCA, GDT_HW_GENERATION_VOLCANICISLAND, true, "Bristol Ridge", "AMD Radeon R5 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xCB, GDT_HW_GENERATION_VOLCANICISLAND, true, "Bristol Ridge", "AMD Radeon R5 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xCC, GDT_HW_GENERATION_VOLCANICISLAND, true, "Bristol Ridge", "AMD Radeon R7 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xCD, GDT_HW_GENERATION_VOLCANICISLAND, true, "Bristol Ridge", "AMD Radeon R7 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xCE, GDT_HW_GENERATION_VOLCANICISLAND, true, "Bristol Ridge", "AMD Radeon R5 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xE1, GDT_HW_GENERATION_VOLCANICISLAND, true, "Bristol Ridge", "AMD Radeon R7 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xE2, GDT_HW_GENERATION_VOLCANICISLAND, true, "Bristol Ridge", "AMD Radeon R7 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xE3, GDT_HW_GENERATION_VOLCANICISLAND, true, "Bristol Ridge", "AMD Radeon R7 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xE4, GDT_HW_GENERATION_VOLCANICISLAND, true, "Bristol Ridge", "AMD Radeon R7 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xE5, GDT_HW_GENERATION_VOLCANICISLAND, true, "Bristol Ridge", "AMD Radeon R5 Graphics" },
    { GDT_CARRIZO, 0x9874, 0xE6, GDT_HW_GENERATION_VOLCANICISLAND, true, "Bristol Ridge", "AMD Radeon R5 Graphics" },

    // Fiji
    { GDT_FIJI, 0x7300, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Fiji", "AMD Radeon (TM) Graphics Processor" },
    { GDT_FIJI, 0x7300, 0xC0, GDT_HW_GENERATION_VOLCANICISLAND, false, "Fiji", "AMD Radeon Graphics Processor" },
    { GDT_FIJI, 0x7300, 0xC1, GDT_HW_GENERATION_VOLCANICISLAND, false, "Fiji", "AMD FirePro (TM) S9300 x2" },
    { GDT_FIJI, 0x7300, 0xC8, GDT_HW_GENERATION_VOLCANICISLAND, false, "Fiji", "AMD Radeon (TM) R9 Fury Series" },
    { GDT_FIJI, 0x7300, 0xC9, GDT_HW_GENERATION_VOLCANICISLAND, false, "Fiji", "Radeon (TM) Pro Duo" },
    { GDT_FIJI, 0x7300, 0xCA, GDT_HW_GENERATION_VOLCANICISLAND, false, "Fiji", "AMD Radeon (TM) R9 Fury Series" },
    { GDT_FIJI, 0x7300, 0xCB, GDT_HW_GENERATION_VOLCANICISLAND, false, "Fiji", "AMD Radeon (TM) R9 Fury Series" },
    { GDT_FIJI, 0x730F, 0xC9, GDT_HW_GENERATION_VOLCANICISLAND, false, "Fiji", "AMD MxGPU" },

    // Stoney
    { GDT_STONEY, 0x98E4, 0x80, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R5E Graphics" },
    { GDT_STONEY, 0x98E4, 0x81, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R4E Graphics" },
    { GDT_STONEY, 0x98E4, 0x83, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R2E Graphics" },
    { GDT_STONEY, 0x98E4, 0x84, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R2E Graphics" },
    { GDT_STONEY, 0x98E4, 0x86, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R1E Graphics" },
    { GDT_STONEY, 0x98E4, 0xC0, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R4 Graphics" },
    { GDT_STONEY, 0x98E4, 0xC1, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R5 Graphics" },
    { GDT_STONEY, 0x98E4, 0xC2, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R4 Graphics" },
    { GDT_STONEY, 0x98E4, 0xC4, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R5 Graphics" },
    { GDT_STONEY, 0x98E4, 0xC6, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R5 Graphics" },
    { GDT_STONEY, 0x98E4, 0xC8, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R4 Graphics" },
    { GDT_STONEY, 0x98E4, 0xC9, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R4 Graphics" },
    { GDT_STONEY, 0x98E4, 0xCA, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R5 Graphics" },
    { GDT_STONEY, 0x98E4, 0xD0, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R2 Graphics" },
    { GDT_STONEY, 0x98E4, 0xD1, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R2 Graphics" },
    { GDT_STONEY, 0x98E4, 0xD2, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R2 Graphics" },
    { GDT_STONEY, 0x98E4, 0xD4, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R2 Graphics" },
    { GDT_STONEY, 0x98E4, 0xD9, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R5 Graphics" },
    { GDT_STONEY, 0x98E4, 0xDA, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R5 Graphics" },
    { GDT_STONEY, 0x98E4, 0xDB, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R3 Graphics" },
    { GDT_STONEY, 0x98E4, 0xE1, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R3 Graphics" },
    { GDT_STONEY, 0x98E4, 0xE2, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R3 Graphics" },
    { GDT_STONEY, 0x98E4, 0xE9, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R4 Graphics" },
    { GDT_STONEY, 0x98E4, 0xEA, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R4 Graphics" },
    { GDT_STONEY, 0x98E4, 0xEB, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R4 Graphics" },
    { GDT_STONEY, 0x98E4, 0xEB, GDT_HW_GENERATION_VOLCANICISLAND, true, "Stoney", "AMD Radeon(TM) R3 Graphics" },

    // Ellesmere
    { GDT_ELLESMERE, 0x67C0, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon (TM) Pro WX 7100 Graphics" },
    { GDT_ELLESMERE, 0x67C0, 0x80, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "AMD Radeon (TM) E9550" },
    { GDT_ELLESMERE, 0x67C1, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "67C1:00" },
    { GDT_ELLESMERE, 0x67C2, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "67C2:00" },
    { GDT_ELLESMERE, 0x67C2, 0x01, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "AMD Radeon (TM) Pro V7350x2" },
    { GDT_ELLESMERE, 0x67C2, 0x02, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "AMD Radeon (TM) Pro V7300X" },
    { GDT_ELLESMERE, 0x67C2, 0x03, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "67C2:03" },
    { GDT_ELLESMERE, 0x67C4, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "AMD Radeon (TM) Pro WX 7100 Graphics" },
    { GDT_ELLESMERE, 0x67C4, 0x80, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "67C4:80" },
    { GDT_ELLESMERE, 0x67C7, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon (TM) Pro WX 5100 Graphics" },
    { GDT_ELLESMERE, 0x67C7, 0x80, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "67C7:80" },
    { GDT_ELLESMERE, 0x67D0, 0x01, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "AMD Radeon (TM) Pro V7350x2" },
    { GDT_ELLESMERE, 0x67D0, 0x02, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "AMD Radeon (TM) Pro V7300X" },
    { GDT_ELLESMERE, 0x67DF, 0x04, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "67DF:04" },
    { GDT_ELLESMERE, 0x67DF, 0x05, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "67DF:05" },
    { GDT_ELLESMERE, 0x67DF, 0xC4, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon (TM) RX 480 Graphics" },
    { GDT_ELLESMERE, 0x67DF, 0xC5, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon (TM) RX 470 Graphics" },
    { GDT_ELLESMERE, 0x67DF, 0xC7, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon (TM) RX 480 Graphics" },
    { GDT_ELLESMERE, 0x67DF, 0xCF, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon (TM) RX 470 Graphics" },
    { GDT_ELLESMERE, 0x67DF, 0xFF, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon RX 470 Series" },
    { GDT_ELLESMERE, 0x67DF, 0xC0, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon Pro 580X" },
    { GDT_ELLESMERE, 0x67DF, 0xC1, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon RX 580 Series" },
    { GDT_ELLESMERE, 0x67DF, 0xC2, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon RX 570 Series" },
    { GDT_ELLESMERE, 0x67DF, 0xC3, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon RX 580 Series" },
    { GDT_ELLESMERE, 0x67DF, 0xC6, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon RX 570 Series" },
    { GDT_ELLESMERE, 0x67DF, 0xC7, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon (TM) RX 480 Graphics"},
    { GDT_ELLESMERE, 0x67DF, 0xCC, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "67DF:CC" },
    { GDT_ELLESMERE, 0x67DF, 0xCD, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "67DF:CD" },
    { GDT_ELLESMERE, 0x67DF, 0xCF, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon (TM) RX 470 Graphics"},
    { GDT_ELLESMERE, 0x67DF, 0xD7, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon(TM) RX 470 Graphics" },
    { GDT_ELLESMERE, 0x67DF, 0xE0, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon RX 470 Series" },
    { GDT_ELLESMERE, 0x67DF, 0xE1, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon RX 590 Series" },
    { GDT_ELLESMERE, 0x67DF, 0xE3, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon RX Series" },
    { GDT_ELLESMERE, 0x67DF, 0xE7, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon RX 580 Series" },
    { GDT_ELLESMERE, 0x67DF, 0xEB, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon Pro 580X"},
    { GDT_ELLESMERE, 0x67DF, 0xEF, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon RX 570 Series" },
    { GDT_ELLESMERE, 0x67DF, 0xF7, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "67DF:F7" },
    { GDT_ELLESMERE, 0x67DF, 0xFF, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "Radeon RX 470 Series"},
    { GDT_ELLESMERE, 0x6FDF, 0xEF, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "6FDF:EF" },
    { GDT_ELLESMERE, 0x6FDF, 0xFF, GDT_HW_GENERATION_VOLCANICISLAND, false, "Ellesmere", "6FDF:FF" },

    // Baffin
    { GDT_BAFFIN, 0x67E0, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon (TM) Pro WX Series" },
    { GDT_BAFFIN, 0x67E3, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon (TM) Pro WX 4100" },
    { GDT_BAFFIN, 0x67E8, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon (TM) Pro WX Series" },
    { GDT_BAFFIN, 0x67E8, 0x01, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon (TM) Pro WX Series" },
    { GDT_BAFFIN, 0x67E8, 0x80, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "AMD Radeon (TM) E9260" },
    { GDT_BAFFIN, 0x67EB, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon (TM) Pro V5300X" },
    { GDT_BAFFIN, 0x67EF, 0xC0, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon RX 560 Series" },
    { GDT_BAFFIN, 0x67EF, 0xC1, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon RX 560 Series" },
    { GDT_BAFFIN, 0x67EF, 0xC5, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon RX 560 Series" },
    { GDT_BAFFIN, 0x67EF, 0xC7, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon 550 Series" },
    { GDT_BAFFIN, 0x67EF, 0xCF, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon(TM) RX 460 Graphics" },
    { GDT_BAFFIN, 0x67EF, 0xEF, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon 550 Series" },
    { GDT_BAFFIN, 0x67FF, 0xC0, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "AMD Radeon Pro 465" },
    { GDT_BAFFIN, 0x67FF, 0xC1, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon RX 560 Series" },
    { GDT_BAFFIN, 0x67EF, 0xC2, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon Pro Series" },
    { GDT_BAFFIN, 0x67EF, 0xC3, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "67EF:C3" },
    { GDT_BAFFIN, 0x67EF, 0xE2, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "RX 560X" },
    { GDT_BAFFIN, 0x67EF, 0xE3, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon Pro Series" },
    { GDT_BAFFIN, 0x67EF, 0xE5, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon RX 560 Series" },
    { GDT_BAFFIN, 0x67EF, 0xE7, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon RX 560 Series" },
    { GDT_BAFFIN, 0x67EF, 0xE0, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon RX 560 Series" },
    { GDT_BAFFIN, 0x67EF, 0xFF, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon(TM) RX 460 Graphics" },
    { GDT_BAFFIN, 0x67FF, 0x08, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "67FF:08" },
    { GDT_BAFFIN, 0x67FF, 0xCF, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon RX 560 Series" },
    { GDT_BAFFIN, 0x67FF, 0xEF, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon RX 560 Series" },
    { GDT_BAFFIN, 0x67FF, 0xFF, GDT_HW_GENERATION_VOLCANICISLAND, false, "Baffin", "Radeon RX550/550 Series" },

    // GFX8_0_4
    { GDT_GFX8_0_4, 0x6980, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "Radeon Pro WX 3100" },
    { GDT_GFX8_0_4, 0x6981, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "AMD Radeon Pro WX 3200 Series" },
    { GDT_GFX8_0_4, 0x6981, 0x01, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "AMD Radeon Pro WX 3200 Series" },
    { GDT_GFX8_0_4, 0x6981, 0x10, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "AMD Radeon Pro WX 3200 Series" },
    { GDT_GFX8_0_4, 0x6981, 0xC0, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "6981:C0" },
    { GDT_GFX8_0_4, 0x6984, 0x80, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "6984:80" },
    { GDT_GFX8_0_4, 0x6985, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "AMD Radeon Pro WX 3100" },
    { GDT_GFX8_0_4, 0x6986, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "AMD Radeon Pro WX 2100" },
    { GDT_GFX8_0_4, 0x6987, 0x80, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "AMD Embedded Radeon E9171" },
    { GDT_GFX8_0_4, 0x6987, 0xC0, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "Radeon 550X Series" },
    { GDT_GFX8_0_4, 0x6987, 0xC3, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "Radeon 540X Series" },
    { GDT_GFX8_0_4, 0x6995, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "AMD Radeon Pro WX 2100" },
    { GDT_GFX8_0_4, 0x6997, 0x00, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "Radeon Pro WX 2100" },
    { GDT_GFX8_0_4, 0x699F, 0x81, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "AMD Embedded Radeon E9170 Series" },
    { GDT_GFX8_0_4, 0x699F, 0xC0, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "Radeon 500 Series" },
    { GDT_GFX8_0_4, 0x699F, 0xC1, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "Radeon 540 Series" },
    { GDT_GFX8_0_4, 0x699F, 0xC3, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "Radeon 500 Series" },
    { GDT_GFX8_0_4, 0x699F, 0xC5, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "699F:C5" },
    { GDT_GFX8_0_4, 0x699F, 0xC7, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "Radeon RX550/550 Series" },
    { GDT_GFX8_0_4, 0x699F, 0xCF, GDT_HW_GENERATION_VOLCANICISLAND, false, "gfx804", "699F:CF" },

    // VegaM
    { GDT_VEGAM1, 0x694C, 0xC0, GDT_HW_GENERATION_VOLCANICISLAND, true, "gfx804", "Radeon RX Vega M GH Graphics" },
    { GDT_VEGAM2, 0x694E, 0xC0, GDT_HW_GENERATION_VOLCANICISLAND, true, "gfx804", "Radeon RX Vega M GL Graphics" },

    // GFX9_0_0
    { GDT_GFX9_0_0, 0x6860, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon Instinct MI25" },
    { GDT_GFX9_0_0, 0x6860, 0x01, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon Instinct MI25" },
    { GDT_GFX9_0_0, 0x6860, 0x02, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon Instinct MI25" },
    { GDT_GFX9_0_0, 0x6860, 0x03, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon Pro V340" },
    { GDT_GFX9_0_0, 0x6860, 0x04, GDT_HW_GENERATION_GFX9, false, "gfx900", "6860:04" },
    { GDT_GFX9_0_0, 0x6860, 0x07, GDT_HW_GENERATION_GFX9, false, "gfx900", "6860:07" },
    { GDT_GFX9_0_0, 0x6860, 0xC0, GDT_HW_GENERATION_GFX9, false, "gfx900", "6860:C0" },
    { GDT_GFX9_0_0, 0x6861, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon (TM) Pro WX 9100" },
    { GDT_GFX9_0_0, 0x6862, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon Pro SSG" },
    { GDT_GFX9_0_0, 0x6863, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon Vega Frontier Edition" },
    { GDT_GFX9_0_0, 0x6864, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx900", "6864:00" },
    { GDT_GFX9_0_0, 0x6864, 0x03, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon Pro V340" },
    { GDT_GFX9_0_0, 0x6864, 0x04, GDT_HW_GENERATION_GFX9, false, "gfx900", "6864:04" },
    { GDT_GFX9_0_0, 0x6864, 0x05, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon Pro V340" },
    { GDT_GFX9_0_0, 0x6867, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx900", "6867:00" },
    { GDT_GFX9_0_0, 0x6868, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon (TM) PRO WX 8200" },
    { GDT_GFX9_0_0, 0x6869, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx900", "6869:00" },
    { GDT_GFX9_0_0, 0x686A, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx900", "686A:00" },
    { GDT_GFX9_0_0, 0x686B, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx900", "686B:00" },
    { GDT_GFX9_0_0, 0x686C, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon Instinct MI25 MxGPU" },
    { GDT_GFX9_0_0, 0x686C, 0x01, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon Instinct MI25 MxGPU" },
    { GDT_GFX9_0_0, 0x686C, 0x02, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon Instinct MI25 MxGPU" },
    { GDT_GFX9_0_0, 0x686C, 0x03, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon Pro V340 MxGPU" },
    { GDT_GFX9_0_0, 0x686C, 0x04, GDT_HW_GENERATION_GFX9, false, "gfx900", "686C:04" },
    { GDT_GFX9_0_0, 0x686C, 0x05, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon Pro V340 MxGPU" },
    { GDT_GFX9_0_0, 0x686C, 0xC1, GDT_HW_GENERATION_GFX9, false, "gfx900", "686C:C1" },
    { GDT_GFX9_0_0, 0x686D, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx900", "686D:00" },
    { GDT_GFX9_0_0, 0x686E, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx900", "686E:00" },
    { GDT_GFX9_0_0, 0x687F, 0x01, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon RX Vega" },
    { GDT_GFX9_0_0, 0x687F, 0xC0, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon RX Vega" },
    { GDT_GFX9_0_0, 0x687F, 0xC1, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon RX Vega" },
    { GDT_GFX9_0_0, 0x687F, 0xC3, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon RX Vega" },
    { GDT_GFX9_0_0, 0x687F, 0xC4, GDT_HW_GENERATION_GFX9, false, "gfx900", "687F:C4" },
    { GDT_GFX9_0_0, 0x687F, 0xC7, GDT_HW_GENERATION_GFX9, false, "gfx900", "Radeon RX Vega" },

    // GFX9_0_2
    { GDT_GFX9_0_2, 0x15DD, 0x00, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD 15DD Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0x81, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 11 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0x82, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0x83, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0x84, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 6 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0x85, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0x86, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD 15DD Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0x87, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD 15DD Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0x88, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xC1, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) RX Vega 11 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xC2, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xC3, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) RX Vega 10 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xC4, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xC5, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xC6, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) RX Vega 11 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xC7, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD 15DD Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xC8, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xC9, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) RX Vega 11 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xCA, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xCB, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xCC, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 6 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xCD, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD 15DD Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xCE, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xD0, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 10 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xD1, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xD2, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD 15DD Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xD3, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 11 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xD4, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD 15DD Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xD5, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xD6, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 11 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xD7, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xD8, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xD9, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 6 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xE1, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15DD, 0xE2, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0x00, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) RX Vega 8 Graphics WS" },
    { GDT_GFX9_0_2, 0x15D8, 0x91, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0x92, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0x93, GDT_HW_GENERATION_GFX9, true, "gfx902", "15D8:93" },
    { GDT_GFX9_0_2, 0x15D8, 0x94, GDT_HW_GENERATION_GFX9, true, "gfx902", "15D8:94" },
    { GDT_GFX9_0_2, 0x15D8, 0x95, GDT_HW_GENERATION_GFX9, true, "gfx902", "15D8:95" },
    { GDT_GFX9_0_2, 0x15D8, 0xA1, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) RX Vega 10 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xA2, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xA3, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 6 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xA4, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xB1, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 10 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xB2, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xB3, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 6 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xB4, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xC1, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) RX Vega 10 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xC2, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xC3, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 6 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xC4, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xC5, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xC8, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) RX Vega 11 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xC9, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xCA, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) RX Vega 11 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xCB, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xCC, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xCD, GDT_HW_GENERATION_GFX9, true, "gfx902", "15D8:CD" },
    { GDT_GFX9_0_2, 0x15D8, 0xCE, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xCF, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xD1, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 10 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xD2, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xD3, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 6 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xD4, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xD8, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 11 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xD9, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 8 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xDA, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 11 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xDB, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xDC, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xDD, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics" },
    { GDT_GFX9_0_2, 0x15D8, 0xDE, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics"},
    { GDT_GFX9_0_2, 0x15D8, 0xDF, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics"},
    { GDT_GFX9_0_2, 0x15D8, 0xE1, GDT_HW_GENERATION_GFX9, true, "gfx902", "15D8:E1" },
    { GDT_GFX9_0_2, 0x15D8, 0xE2, GDT_HW_GENERATION_GFX9, true, "gfx902", "15D8:E2" },
    { GDT_GFX9_0_2, 0x15D8, 0xE3, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics"},
    { GDT_GFX9_0_2, 0x15D8, 0xE4, GDT_HW_GENERATION_GFX9, true, "gfx902", "AMD Radeon(TM) Vega 3 Graphics"},
    { GDT_GFX9_0_2, 0x15D8, 0xE9, GDT_HW_GENERATION_GFX9, true, "gfx902", "15D8:E9" },
    { GDT_GFX9_0_2, 0x15D8, 0xEA, GDT_HW_GENERATION_GFX9, true, "gfx902", "15D8:EA" },
    { GDT_GFX9_0_2, 0x15D8, 0xEB, GDT_HW_GENERATION_GFX9, true, "gfx902", "15D8:EB" },
    { GDT_GFX9_0_2, 0x15D9, 0x91, GDT_HW_GENERATION_GFX9, true, "gfx902", "15D9:91" },
    { GDT_GFX9_0_2, 0x15D9, 0x92, GDT_HW_GENERATION_GFX9, true, "gfx902", "15D9:92" },
    { GDT_GFX9_0_2, 0x15D9, 0xC1, GDT_HW_GENERATION_GFX9, true, "gfx902", "15D9:C1" },
    { GDT_GFX9_0_2, 0x15D9, 0xC2, GDT_HW_GENERATION_GFX9, true, "gfx902", "15D9:C2" },
    { GDT_GFX9_0_2, 0x15D9, 0xC3, GDT_HW_GENERATION_GFX9, true, "gfx902", "15D9:C3" },

    // GFX9_0_6
    { GDT_GFX9_0_6, 0x66A3, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx906", "66A3:00" },
    { GDT_GFX9_0_6, 0x66A7, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx906", "66A7:00" },
    { GDT_GFX9_0_6, 0x66AF, 0xC0, GDT_HW_GENERATION_GFX9, false, "gfx906", "66AF:C0" },
    { GDT_GFX9_0_6, 0x66AF, 0xC1, GDT_HW_GENERATION_GFX9, false, "gfx906", "AMD Radeon VII" },
    { GDT_GFX9_0_6, 0x66AF, 0xCF, GDT_HW_GENERATION_GFX9, false, "gfx906", "66AF:CF" },
    { GDT_GFX9_0_6, 0x66A0, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx906", "66A0:00" },
    { GDT_GFX9_0_6, 0x66A1, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx906", "66A1:00" },
    { GDT_GFX9_0_6, 0x66A1, 0x02, GDT_HW_GENERATION_GFX9, false, "gfx906", "66A1:02" },
    { GDT_GFX9_0_6, 0x66A1, 0x03, GDT_HW_GENERATION_GFX9, false, "gfx906", "66A1:03" },
    { GDT_GFX9_0_6, 0x66A1, 0x06, GDT_HW_GENERATION_GFX9, false, "gfx906", "66A1:06" },
    { GDT_GFX9_0_6, 0x66A2, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx906", "66A2:00" },
    { GDT_GFX9_0_6, 0x66A2, 0x02, GDT_HW_GENERATION_GFX9, false, "gfx906", "66A2:02" },
    { GDT_GFX9_0_6, 0x66A4, 0x00, GDT_HW_GENERATION_GFX9, false, "gfx906", "66A4:00" },
    { GDT_GFX9_0_6, 0x66AF, 0xC3, GDT_HW_GENERATION_GFX9, false, "gfx906", "66AF:C3" },
    { GDT_GFX9_0_6, 0x66AF, 0xC7, GDT_HW_GENERATION_GFX9, false, "gfx906", "66AF:C7" },

    // GFX9_0_9
    { GDT_GFX9_0_9, 0x1636, 0x00, GDT_HW_GENERATION_GFX9, true, "gfx909", "1636:00" },
    { GDT_GFX9_0_9, 0x1636, 0x80, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0x81, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0x82, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0x83, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0x84, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xC1, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xC2, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xC3, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xC4, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xC5, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xC6, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xC7, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xC8, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xC9, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xCA, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xCB, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xCC, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xCD, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xCE, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xD1, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xD2, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xD3, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xD4, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xD5, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xD6, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xD7, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xD8, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xD9, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xDA, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xDB, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xDC, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xDD, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xDE, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xE1, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xE2, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },
    { GDT_GFX9_0_9, 0x1636, 0xE3, GDT_HW_GENERATION_GFX9, true, "gfx909", "AMD Radeon(TM) Graphics" },

    // GFX10_1_0
    { GDT_GFX10_1_0, 0x7310, 0x00, GDT_HW_GENERATION_GFX10, false, "gfx1010", "7310:00" },
    { GDT_GFX10_1_0, 0x7312, 0x00, GDT_HW_GENERATION_GFX10, false, "gfx1010", "7312:00" },
    { GDT_GFX10_1_0, 0x7318, 0x40, GDT_HW_GENERATION_GFX10, false, "gfx1010", "7318:40" },
    { GDT_GFX10_1_0, 0x7319, 0x40, GDT_HW_GENERATION_GFX10, false, "gfx1010", "7319:40" },
    { GDT_GFX10_1_0, 0x731A, 0x40, GDT_HW_GENERATION_GFX10, false, "gfx1010", "731A:40" },
    { GDT_GFX10_1_0, 0x731B, 0x40, GDT_HW_GENERATION_GFX10, false, "gfx1010", "731B:40" },
    { GDT_GFX10_1_0, 0x731F, 0xC0, GDT_HW_GENERATION_GFX10, false, "gfx1010", "AMD Radeon RX 5700 XT 50th Anniversary" },
    { GDT_GFX10_1_0, 0x731F, 0xC1, GDT_HW_GENERATION_GFX10, false, "gfx1010", "AMD Radeon RX 5700 XT" },
    { GDT_GFX10_1_0, 0x731F, 0xC2, GDT_HW_GENERATION_GFX10, false, "gfx1010", "AMD Radeon RX 5600M" },
    { GDT_GFX10_1_0, 0x731F, 0xC3, GDT_HW_GENERATION_GFX10, false, "gfx1010", "AMD Radeon RX 5700M" },
    { GDT_GFX10_1_0_XL, 0x731F, 0xC4, GDT_HW_GENERATION_GFX10, false, "gfx1010", "AMD Radeon RX 5700" },
    { GDT_GFX10_1_0, 0x731F, 0xC5, GDT_HW_GENERATION_GFX10, false, "gfx1010", "AMD Radeon RX 5700 XT" },
    { GDT_GFX10_1_0, 0x731F, 0xC7, GDT_HW_GENERATION_GFX10, false, "gfx1010", "731F:C7" },
    { GDT_GFX10_1_0, 0x731F, 0xCA, GDT_HW_GENERATION_GFX10, false, "gfx1010", "AMD Radeon RX 5600 XT" },
    { GDT_GFX10_1_0, 0x731F, 0xCB, GDT_HW_GENERATION_GFX10, false, "gfx1010", "731F:CB" },
    { GDT_GFX10_1_0, 0x731F, 0xE1, GDT_HW_GENERATION_GFX10, false, "gfx1010", "731F:E1" },
    { GDT_GFX10_1_0, 0x731F, 0xE3, GDT_HW_GENERATION_GFX10, false, "gfx1010", "731F:E3" },
    { GDT_GFX10_1_0, 0x731F, 0xE7, GDT_HW_GENERATION_GFX10, false, "gfx1010", "731F:E7" },

    // GFX10_1_2
    { GDT_GFX10_1_2, 0x7340, 0x00, GDT_HW_GENERATION_GFX10, false, "gfx1012", "7340:00" },
    { GDT_GFX10_1_2, 0x7340, 0x41, GDT_HW_GENERATION_GFX10, false, "gfx1012", "7340:41" },
    { GDT_GFX10_1_2, 0x7340, 0x43, GDT_HW_GENERATION_GFX10, false, "gfx1012", "7340:43" },
    { GDT_GFX10_1_2, 0x7340, 0x47, GDT_HW_GENERATION_GFX10, false, "gfx1012", "7340:47" },
    { GDT_GFX10_1_2, 0x7340, 0xC1, GDT_HW_GENERATION_GFX10, false, "gfx1012", "Radeon RX 5500M" },
    { GDT_GFX10_1_2, 0x7340, 0xC3, GDT_HW_GENERATION_GFX10, false, "gfx1012", "Radeon RX 5300M" },
    { GDT_GFX10_1_2, 0x7340, 0xC5, GDT_HW_GENERATION_GFX10, false, "gfx1012", "Radeon RX 5500 XT" },
    { GDT_GFX10_1_2, 0x7340, 0xC7, GDT_HW_GENERATION_GFX10, false, "gfx1012", "Radeon RX 5500" },
    { GDT_GFX10_1_2, 0x7340, 0xCF, GDT_HW_GENERATION_GFX10, false, "gfx1012", "7340:CF" },
    { GDT_GFX10_1_2, 0x7340, 0x70, GDT_HW_GENERATION_GFX10, false, "gfx1012", "7340:70" },
    { GDT_GFX10_1_2, 0x7340, 0xF2, GDT_HW_GENERATION_GFX10, false, "gfx1012", "7340:F2" },
    { GDT_GFX10_1_2, 0x7340, 0xF3, GDT_HW_GENERATION_GFX10, false, "gfx1012", "7340:F3" },
    { GDT_GFX10_1_2, 0x7341, 0x00, GDT_HW_GENERATION_GFX10, false, "gfx1012", "7341:00" },
    { GDT_GFX10_1_2, 0x7343, 0x00, GDT_HW_GENERATION_GFX10, false, "gfx1012", "7343:00" },
    { GDT_GFX10_1_2, 0x7347, 0x00, GDT_HW_GENERATION_GFX10, false, "gfx1012", "7347:00" },
    { GDT_GFX10_1_2, 0x734F, 0x00, GDT_HW_GENERATION_GFX10, false, "gfx1012", "734F:00" },
};

static GDT_DeviceInfo gs_deviceInfo[] =
{
    { 2, 10, 1, 8, 2, 64, 2, 7, 4, true }, // GDT_TAHITI_PRO
    { 2, 10, 1, 8, 2, 64, 2, 8, 4, true }, // GDT_TAHITI_XT
    { 2, 10, 1, 8, 2, 64, 2, 4, 4, true }, // GDT_PITCAIRN_PRO
    { 2, 10, 1, 8, 2, 64, 2, 5, 4, true }, // GDT_PITCAIRN_XT
    { 1, 10, 1, 8, 1, 64, 2, 4, 4, true }, // GDT_CAPEVERDE_PRO
    { 1, 10, 1, 8, 1, 64, 2, 5, 4, true }, // GDT_CAPEVERDE_XT
    { 1, 10, 1, 8, 1, 64, 1, 6, 4, true }, // GDT_OLAND
    { 1, 10, 1, 8, 1, 64, 1, 5, 4, true }, // GDT_HAINAN
    { 2, 10, 1, 8, 2, 64, 1, 7, 4, true }, // GDT_BONAIRE
    { 4, 10, 1, 8, 4, 64, 1, 11, 4, true }, // GDT_HAWAII
    { 1, 10, 1, 8, 1, 64, 1, 2, 4, true }, // GDT_KALINDI
    { 1, 10, 1, 8, 1, 64, 1, 8, 4, true }, // GDT_SPECTRE
    { 1, 10, 1, 8, 1, 64, 1, 4, 4, true }, // GDT_SPECTRE_SL
    { 1, 10, 1, 8, 1, 64, 1, 6, 4, true }, // GDT_SPECTRE_LITE
    { 1, 10, 1, 8, 1, 64, 1, 3, 4, true }, // GDT_SPOOKY
    { 1, 10, 1, 8, 1, 64, 1, 6, 4, true }, // GDT_ICELAND
    { 4, 10, 1, 8, 4, 64, 1, 8, 4, true }, // GDT_TONGA
    { 1, 10, 1, 8, 1, 64, 1, 8, 4, true }, // GDT_CARRIZO
    { 1, 10, 1, 8, 1, 64, 1, 3, 4, true }, // GDT_CARRIZO_EM
    { 4, 10, 1, 8, 4, 64, 1, 16, 4, true }, // GDT_FIJI
    { 1, 10, 1, 8, 1, 64, 1, 3, 4, true }, // GDT_STONEY
    { 4, 8, 1, 8, 4, 64, 1, 9, 4, true }, // GDT_ELLESMERE
    { 2, 8, 1, 8, 2, 64, 1, 8, 4, true }, // GDT_BAFFIN
    { 2, 8, 1, 8, 2, 64, 1, 5, 4, true }, // GDT_GFX8_0_4
    { 4, 8, 1, 8, 4, 64, 1, 6, 4, true }, // GDT_VEGAM1
    { 4, 8, 1, 8, 4, 64, 1, 5, 4, true }, // GDT_VEGAM2
    { 4, 10, 1, 8, 4, 64, 1, 16, 4, true }, // GDT_GFX9_0_0
    { 1, 10, 1, 8, 1, 64, 1, 11, 4, true }, // GDT_GFX9_0_2
    { 4, 10, 1, 8, 4, 64, 1, 16, 4, true }, // GDT_GFX9_0_6
    { 1, 10, 1, 8, 1, 64, 1, 8, 4, true }, // GDT_GFX9_0_9
    { 2, 20, 1, 16, 4, 64, 2, 10, 2, true }, // GDT_GFX10_1_0
    { 2, 20, 1, 16, 4, 64, 2, 9, 2, true }, // GDT_GFX10_1_0_XL
    { 1, 20, 1, 16, 4, 64, 2, 12, 2, true }, // GDT_GFX10_1_2
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, false }, // GDT_ASIC_PLACEHOLDER_1
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, false }, // GDT_ASIC_PLACEHOLDER_2
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, false }, // GDT_ASIC_PLACEHOLDER_3
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, false }, // GDT_ASIC_PLACEHOLDER_4
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, false }, // GDT_ASIC_PLACEHOLDER_5
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, false }, // GDT_ASIC_PLACEHOLDER_6
};

static size_t gs_cardInfoSize = sizeof(gs_cardInfo) / sizeof(GDT_GfxCardInfo);
static size_t gs_deviceInfoSize = sizeof(gs_deviceInfo) / sizeof(GDT_DeviceInfo);

AMDTDeviceInfoUtils* AMDTDeviceInfoUtils::ms_pInstance = nullptr;
AMDTDeviceInfoManager* AMDTDeviceInfoManager::ms_pInstance = nullptr;

AMDTDeviceInfoManager::AMDTDeviceInfoManager()
{
    AMDTDeviceInfoUtils* pDeviceInfoUtils = AMDTDeviceInfoUtils::Instance();

    for (size_t i = 0; i < gs_cardInfoSize; ++i)
    {
        pDeviceInfoUtils->AddDevice(gs_cardInfo[i]);
    }

    for (size_t i = 0; i < gs_deviceInfoSize; ++i)
    {
        pDeviceInfoUtils->AddDeviceInfo(static_cast<GDT_HW_ASIC_TYPE>(i), gs_deviceInfo[i]);
    }

    CallInitInternalDeviceInfo();
}

#ifdef _WIN32
    extern "C" IMAGE_DOS_HEADER __ImageBase;
    #define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

void AMDTDeviceInfoManager::CallInitInternalDeviceInfo() const
{
    static const char* initFuncName = "InitInternalDeviceInfo";
    static bool initFuncCalled = false;

    if (!initFuncCalled)
    {
        typedef void(*DeviceInfoUtilsReadyFunc)();
        DeviceInfoUtilsReadyFunc func = nullptr;
#ifdef _WIN32
        func = (DeviceInfoUtilsReadyFunc)GetProcAddress(HINST_THISCOMPONENT, initFuncName);
#endif
#ifdef _LINUX
        func = (DeviceInfoUtilsReadyFunc)dlsym(nullptr, initFuncName);
#endif

        if (nullptr != func)
        {
            func();
            initFuncCalled = true;
        }
    }
}

AMDTDeviceInfoUtils* AMDTDeviceInfoUtils::Instance()
{
    if (nullptr == ms_pInstance)
    {
        ms_pInstance = new AMDTDeviceInfoUtils();
        AMDTDeviceInfoManager::Instance();
    }
    return ms_pInstance;
}

bool AMDTDeviceInfoUtils::GetDeviceInfo(size_t deviceID, size_t revisionID, GDT_DeviceInfo& deviceInfo) const
{
    bool found = false;

    auto matches = m_deviceIDMap.equal_range(deviceID);
    for (auto it = matches.first; it != matches.second && !found; ++it)
    {
        size_t thisRevId = (*it).second.m_revID;

        if (REVISION_ID_ANY == revisionID || thisRevId == revisionID)
        {
            for (auto itr = m_asicTypeDeviceInfoMap.find((*it).second.m_asicType); itr != m_asicTypeDeviceInfoMap.end() && !found; ++itr)
            {
                deviceInfo = itr->second;

                if (deviceInfo.m_deviceInfoValid)
                {
                    found = true;
                }
            }
        }
    }

    return found;
}

/// NOTE: this might not return the correct GDT_DeviceInfo instance, since some devices with the same CAL name might have different GDT_DeviceInfo instances
bool AMDTDeviceInfoUtils::GetDeviceInfo(const char* szCALDeviceName, GDT_DeviceInfo& deviceInfo) const
{
    std::string strDeviceName = TranslateDeviceName(szCALDeviceName);
    auto matches = m_deviceNameMap.equal_range(strDeviceName.c_str());

    if (matches.first != matches.second)
    {
        auto deviceIt = m_asicTypeDeviceInfoMap.find(matches.first->second.m_asicType);

        if (m_asicTypeDeviceInfoMap.end() != deviceIt)
        {
            deviceInfo = deviceIt->second;

            if (deviceInfo.m_deviceInfoValid)
            {
                return true;
            }
        }
    }

    return false;
}

bool AMDTDeviceInfoUtils::GetDeviceInfo(size_t deviceID, size_t revisionID, GDT_GfxCardInfo& cardInfo) const
{
    bool found = false;

    auto matches = m_deviceIDMap.equal_range(deviceID);
    for (auto it = matches.first; it != matches.second && !found; ++it)
    {
        size_t thisRevId = (*it).second.m_revID;

        if (REVISION_ID_ANY == revisionID || thisRevId == revisionID)
        {
            cardInfo = (*it).second;
            found = true;
        }
    }

    return found;
}

bool AMDTDeviceInfoUtils::GetDeviceInfo(const char* szCALDeviceName, vector<GDT_GfxCardInfo>& cardList) const
{
    std::string strDeviceName = TranslateDeviceName(szCALDeviceName);

    cardList.clear();
    auto matches = m_deviceNameMap.equal_range(strDeviceName.c_str());

    for (auto it = matches.first; it != matches.second; ++it)
    {
        cardList.push_back((*it).second);
    }

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::GetDeviceInfoMarketingName(const char* szMarketingDeviceName, vector<GDT_GfxCardInfo>& cardList) const
{
    cardList.clear();
    auto matches = m_deviceMarketingNameMap.equal_range(szMarketingDeviceName);

    for (auto it = matches.first; it != matches.second; ++it)
    {
        cardList.push_back((*it).second);
    }

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::IsAPU(const char* szCALDeviceName, bool& bIsAPU) const
{
    std::string strDeviceName = TranslateDeviceName(szCALDeviceName);
    auto matches = m_deviceNameMap.equal_range(strDeviceName.c_str());

    if (matches.first != matches.second)
    {
        bIsAPU = matches.first->second.m_bAPU;
        return true;
    }
    else
    {
        return false;
    }
}

bool AMDTDeviceInfoUtils::IsAPU(size_t deviceID, bool& isAPU) const
{
    auto matches = m_deviceIDMap.equal_range(deviceID);

    if (matches.first != matches.second)
    {
        isAPU = matches.first->second.m_bAPU;
        return true;
    }
    else
    {
        return false;
    }
}

bool AMDTDeviceInfoUtils::IsXFamily(size_t deviceID, GDT_HW_GENERATION generation, bool& isXFamily) const
{
    GDT_HW_GENERATION gen = GDT_HW_GENERATION_NONE;

    if (GetHardwareGeneration(deviceID, gen))
    {
        isXFamily = gen == generation;
        return true;
    }
    else
    {
        return false;
    }
}

bool AMDTDeviceInfoUtils::IsGfx10Family(size_t deviceID, bool& isGfx10) const
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_GFX10, isGfx10);
}

bool AMDTDeviceInfoUtils::IsGfx9Family(size_t deviceID, bool& isGfx9) const
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_GFX9, isGfx9);
}

bool AMDTDeviceInfoUtils::IsVIFamily(size_t deviceID, bool& isVI) const
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_VOLCANICISLAND, isVI);
}

bool AMDTDeviceInfoUtils::IsCIFamily(size_t deviceID, bool& isCI) const
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_SEAISLAND, isCI);
}

bool AMDTDeviceInfoUtils::IsSIFamily(size_t deviceID, bool& isSI) const
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_SOUTHERNISLAND, isSI);
}

bool AMDTDeviceInfoUtils::GetHardwareGeneration(size_t deviceID, GDT_HW_GENERATION& gen) const
{
    // revId not needed here, since all revs will have the same hardware family
    auto matches = m_deviceIDMap.equal_range(deviceID);

    if (matches.first != matches.second)
    {
        gen = matches.first->second.m_generation;
        return true;
    }
    else
    {
        return false;
    }
}

bool AMDTDeviceInfoUtils::GetHardwareGeneration(const char* szCALDeviceName, GDT_HW_GENERATION& gen) const
{
    std::string strDeviceName = TranslateDeviceName(szCALDeviceName);
    auto matches = m_deviceNameMap.equal_range(strDeviceName.c_str());

    if (matches.first != matches.second)
    {
        gen = matches.first->second.m_generation;
        return true;
    }
    else
    {
        return false;
    }
}

void AMDTDeviceInfoUtils::GetAllCards(std::vector<GDT_GfxCardInfo>& cardList) const
{
    cardList.clear();
    cardList.reserve(gs_cardInfoSize);

    for(size_t i = 0ULL; i < gs_cardInfoSize; ++i)
    {
        cardList.push_back(gs_cardInfo[i]);
    }
}

bool AMDTDeviceInfoUtils::GetAllCardsWithName(const char* szCALDeviceName, std::vector<GDT_GfxCardInfo>& cardList) const
{
    return GetDeviceInfo(szCALDeviceName, cardList);
}

bool AMDTDeviceInfoUtils::GetAllCardsInHardwareGeneration(GDT_HW_GENERATION gen, std::vector<GDT_GfxCardInfo>& cardList) const
{
    cardList.clear();
    auto matches = m_deviceHwGenerationMap.equal_range(gen);

    for (auto it = matches.first; it != matches.second; ++it)
    {
        cardList.push_back((*it).second);
    }

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::GetAllCardsWithDeviceId(size_t deviceID, std::vector<GDT_GfxCardInfo>& cardList) const
{
    cardList.clear();
    auto matches = m_deviceIDMap.equal_range(deviceID);

    for (auto it = matches.first; it != matches.second; ++it)
    {
        cardList.push_back((*it).second);
    }

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::GetAllCardsWithAsicType(GDT_HW_ASIC_TYPE asicType, std::vector<GDT_GfxCardInfo>& cardList) const
{
    cardList.clear();
    auto matches = m_asicTypeCardInfoMap.equal_range(asicType);

    for (auto it = matches.first; it != matches.second; ++it)
    {
        cardList.push_back((*it).second);
    }

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::GetHardwareGenerationDisplayName(GDT_HW_GENERATION gen, std::string& strGenerationDisplayName) const
{
    static const std::string s_SI_FAMILY_NAME    = "Graphics IP v6";
    static const std::string s_CI_FAMILY_NAME    = "Graphics IP v7";
    static const std::string s_VI_FAMILY_NAME    = "Graphics IP v8";
    static const std::string s_GFX9_FAMILY_NAME  = "Vega";
    static const std::string s_GFX10_FAMILY_NAME = "RDNA";

    bool retVal = true;

    switch (gen)
    {
        case GDT_HW_GENERATION_SOUTHERNISLAND:
            strGenerationDisplayName = s_SI_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_SEAISLAND:
            strGenerationDisplayName = s_CI_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_VOLCANICISLAND:
            strGenerationDisplayName = s_VI_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_GFX9:
            strGenerationDisplayName = s_GFX9_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_GFX10:
            strGenerationDisplayName = s_GFX10_FAMILY_NAME;
            break;

        default:
            strGenerationDisplayName.clear();
            assert(false);
            retVal = false;
            break;
    }

    return retVal;
}

std::string AMDTDeviceInfoUtils::TranslateDeviceName(const char* strDeviceName) const
{
    std::string retVal(strDeviceName);

    if (retVal.compare("gfx901") == 0) // some gfx900 boards are identified as gfx901 by some drivers
    {
        retVal = "gfx900";
    }

    if (retVal.compare("gfx903") == 0) // some gfx902 APUs are identified as gfx903 by some drivers
    {
        retVal = "gfx902";
    }

    if (nullptr != m_pDeviceNameTranslatorFunction)
    {
        retVal = m_pDeviceNameTranslatorFunction(retVal.c_str());
    }

    return retVal;
}

bool AMDTDeviceInfoUtils::GfxIPVerToHwGeneration(size_t gfxIPVer, GDT_HW_GENERATION& hwGen) const
{
    hwGen = static_cast<GDT_HW_GENERATION>(gfxIPVer - ms_gfxToGdtHwGenConversionFactor);

    bool retVal = hwGen >= GDT_HW_GENERATION_FIRST_AMD && hwGen < GDT_HW_GENERATION_LAST;

    if (!retVal)
    {
        hwGen = GDT_HW_GENERATION_NONE;
    }

    return retVal;
}

bool AMDTDeviceInfoUtils::HwGenerationToGfxIPVer(GDT_HW_GENERATION hwGen, size_t& gfxIPVer) const
{
    gfxIPVer = 0;

    bool retVal = hwGen >= GDT_HW_GENERATION_FIRST_AMD && hwGen < GDT_HW_GENERATION_LAST;

    if (retVal)
    {
        gfxIPVer = static_cast<size_t>(hwGen) + ms_gfxToGdtHwGenConversionFactor;
    }

    return retVal;
}

void AMDTDeviceInfoUtils::AddDeviceInfo(GDT_HW_ASIC_TYPE asicType, const GDT_DeviceInfo& deviceInfo)
{
    if (m_asicTypeDeviceInfoMap.end() == m_asicTypeDeviceInfoMap.find(asicType))
    {
        m_asicTypeDeviceInfoMap.insert(ASICTypeDeviceInfoMapPair(asicType, deviceInfo));
    }
    else
    {
        m_asicTypeDeviceInfoMap[asicType] = deviceInfo;
    }
}

void AMDTDeviceInfoUtils::AddDevice(const GDT_GfxCardInfo& cardInfo)
{
    m_deviceIDMap.insert(DeviceIDMapPair(cardInfo.m_deviceID, cardInfo));
    m_asicTypeCardInfoMap.insert(ASICTypeCardInfoMapPair(cardInfo.m_asicType, cardInfo));
    m_deviceNameMap.insert(DeviceNameMapPair(cardInfo.m_szCALName, cardInfo));
    m_deviceMarketingNameMap.insert(DeviceNameMapPair(cardInfo.m_szMarketingName, cardInfo));
    m_deviceHwGenerationMap.insert(DeviceHWGenerationMapPair(cardInfo.m_generation, cardInfo));
}

void AMDTDeviceInfoUtils::RemoveDevice(const GDT_GfxCardInfo& cardInfo)
{
    for (auto it = m_deviceIDMap.begin(); it != m_deviceIDMap.end(); ++it)
    {
        if (it->first == cardInfo.m_deviceID && it->second.m_revID == cardInfo.m_revID)
        {
            m_deviceIDMap.erase(it);
            break;
        }
    }

    for (auto it = m_asicTypeCardInfoMap.begin(); it != m_asicTypeCardInfoMap.end(); ++it)
    {
        if (it->second.m_deviceID == cardInfo.m_deviceID && it->second.m_revID == cardInfo.m_revID)
        {
            m_asicTypeCardInfoMap.erase(it);
            break;
        }
    }

    for (auto it = m_deviceNameMap.begin(); it != m_deviceNameMap.end(); ++it)
    {
        if (it->second.m_deviceID == cardInfo.m_deviceID && it->second.m_revID == cardInfo.m_revID)
        {
            m_deviceNameMap.erase(it);
            break;
        }
    }

    for (auto it = m_deviceMarketingNameMap.begin(); it != m_deviceMarketingNameMap.end(); ++it)
    {
        if (it->second.m_deviceID == cardInfo.m_deviceID && it->second.m_revID == cardInfo.m_revID)
        {
            m_deviceMarketingNameMap.erase(it);
            break;
        }
    }

    for (auto it = m_deviceHwGenerationMap.begin(); it != m_deviceHwGenerationMap.end(); ++it)
    {
        if (it->first == cardInfo.m_generation && it->second.m_deviceID == cardInfo.m_deviceID && it->second.m_revID == cardInfo.m_revID)
        {
            m_deviceHwGenerationMap.erase(it);
            break;
        }
    }
}

void AMDTDeviceInfoUtils::SetDeviceNameTranslator(DeviceNameTranslatorFunction deviceNametranslatorFunction)
{
    m_pDeviceNameTranslatorFunction = deviceNametranslatorFunction;
}
