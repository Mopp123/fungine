#pragma once

// Definitions ----------------------------------	Definitions
#define byte unsigned char

// Defaults -------------------------------------	Defaults
#define DEFAULT__WINDOW_WIDTH 1024
#define DEFAULT__WINDOW_HEIGHT 768
#define DEFAULT__WINDOW_SWAP_INTERVAL 0

#define DEFAULT__OPENGL_VERSION_MAJOR 4
#define DEFAULT__OPENGL_VERSION_MINOR 0

// Component related ----------------------------	Component related
#define COMPONENT__INVALID_ID -1

// Resource related ----------------------------	Resource related
#define RESOURCE__INVALID_ID -1

// Macros ---------------------------------------	Macros

// Helps us to detect if we accidentally add something to a list indefinately
#define add_to_dynamic_list(list, x) list.push_back(x); printf("*Increased dynamic list size. Current list length: %d\n", list.size());