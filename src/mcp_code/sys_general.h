/*
 * Gather and return information about the system
 */

#ifndef __SYS_GENERAL_H
#define __SYS_GENERAL_H

//#include "version.h"
//#include "types.h"
#include <stdbool.h>
#include <mcp/types.h>



/*
 * Structure to describe the hardware
 */
// typedef struct s_sys_info {
//     unsigned short mcp_version;     /* Current version of the MCP kernel */
//     unsigned short mcp_rev;         /* Current revision, or sub-version of the MCP kernel */
//     unsigned short mcp_build;       /* Current vuild # of the MCP kernel */
//     unsigned short model;           /* Code to say what model of machine this is */
//     const char * model_name;        /* Human readable name of the model of the computer */
//     unsigned short cpu;             /* Code to say which CPU is running */
//     const char * cpu_name;          /* Human readable name for the CPU */
//     unsigned int cpu_clock_khz;     /* Speed of the CPU clock in KHz */
//     unsigned long fpga_model;       /* FPGA model number */
//     unsigned short fpga_version;    /* FPGA version */
//     unsigned short fpga_subver;     /* FPGA sub-version */
//     long system_ram_size;           /* The number of bytes of system RAM on the board */
//     bool has_floppy;                /* TRUE if the board has a floppy drive installed */
//     bool has_hard_drive;            /* TRUE if the board has a PATA device installed */
//     bool has_expansion_card;        /* TRUE if an expansion card is installed on the device */
//     bool has_ethernet;              /* TRUE if an ethernet port is present */
//     unsigned short screens;         /* How many screens are on this computer */
// } t_sys_info, *p_sys_info;

/*
 * Fill out a s_sys_info structure with the information about the current system
 *
 * Inputs:
 * info = pointer to a s_sys_info structure to fill out
 */
extern void sys_get_information(p_sys_info info);

#endif
