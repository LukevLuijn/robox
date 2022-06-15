// ======================================
// Filename:    bke_locations.h
// Project:     ROBOX Control UI
// Subsystem:   BKE control
// Author:      Luke van Luijn
// Data:        11-06-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_BKE_LOCATIONS_H
#define ROBOX_CONTROL_UI_BKE_LOCATIONS_H

#define N_BOARD_LOCATIONS 9
#define BOARD_START 0

#define BOARD_LOC_00 -7.3f, -161.1f, 38.5f
#define BOARD_LOC_01  7.0f, -175.7f, 39.0f
#define BOARD_LOC_02 60.1f, -161.1f, 40.0f
#define BOARD_LOC_03 16.2f, -137.8f, 40.5f
#define BOARD_LOC_04 37.1f, -142.4f, 41.0f
#define BOARD_LOC_05 59.8f, -137.8f, 42.0f
#define BOARD_LOC_06 21.7f, -115.6f, 42.0f
#define BOARD_LOC_07 38.1f, -119.0f, 43.0f
#define BOARD_LOC_08 53.6f, -115.6f, 44.5f

#define N_STORAGE_X_LOCATIONS 5
#define STORAGE_X_START BOARD_START + N_BOARD_LOCATIONS

#define STORAGE_X_00 -42.9f, -124.6f, 38.0f
#define STORAGE_X_01 -27.1f, -117.7f, 39.0f
#define STORAGE_X_02 -16.4f, -105.0f, 41.0f
#define STORAGE_X_03 -11.4f,  -87.2f, 44.0f
#define STORAGE_X_04 -12.3f,  -62.6f, 47.0f

#define N_STORAGE_O_LOCATIONS 5
#define STORAGE_O_START STORAGE_X_START + N_STORAGE_X_LOCATIONS

#define STORAGE_O_00 42.5f, -234.3f, 43.0f
#define STORAGE_O_01 26.6f, -241.3f, 45.0f
#define STORAGE_O_02 15.8f, -254.0f, 47.0f
#define STORAGE_O_03 10.7f, -271.9f, 48.0f
#define STORAGE_O_04 11.5f, -296.3f, 51.0f

#define N_TOTAL_LOCATIONS N_BOARD_LOCATIONS + N_STORAGE_X_LOCATIONS + N_STORAGE_O_LOCATIONS


#endif//ROBOX_CONTROL_UI_BKE_LOCATIONS_H
