/**
 * @file    OpticsMaterials.c
 * @brief   Optics materials refractive indices and properties
 *
 * 
 */


#define _GNU_SOURCE

/* ================================================================== */
/* ================================================================== */
/*  MODULE INFO                                                       */
/* ================================================================== */
/* ================================================================== */

// module default short name
// all CLI calls to this module functions will be <shortname>.<funcname>
// if set to "", then calls use <funcname>
#define MODULE_SHORTNAME_DEFAULT "optmat"

// Module short description
#define MODULE_DESCRIPTION       "Optics materials properties"






/* ================================================================== */
/* ================================================================== */
/*  HEADER FILES                                                      */
/* ================================================================== */
/* ================================================================== */


#include "CommandLineInterface/CLIcore.h"



//
// Forward declarations are required to connect CLI calls to functions
// If functions are in separate .c files, include here the corresponding .h files
//
//#include "create_example_image.h"
//#include "stream_process_loop_simple.h"






/* ================================================================== */
/* ================================================================== */
/*  INITIALIZE LIBRARY                                                */
/* ================================================================== */
/* ================================================================== */

// Module initialization macro in CLIcore.h
// macro argument defines module name for bindings
//
INIT_MODULE_LIB(OpticsMaterials)



/**
 * @brief Initialize module CLI
 *
 * CLI entries are registered: CLI call names are connected to CLI functions.\n
 * Any other initialization is performed\n
 *
 */
static errno_t init_module_CLI()
{

    //CLI_CMD_CONNECT("func1", "create_image_with_value");

	//create_example_image_addCLIcmd();
	//stream_process_loop_simple_addCLIcmd;

    // optional: add atexit functions here

    return RETURN_SUCCESS;
}








