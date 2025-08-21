#ifndef MAIN_H
#define MAIN_H

#include "config.h"

// Function prototypes
void System_Init(void);
void System_Run(void);
void System_ErrorHandler(error_code_t error);
void system_reset(void);

// Maintenance functions
void Enter_MaintenanceMode(void);
void Exit_MaintenanceMode(void);

#endif // MAIN_H
