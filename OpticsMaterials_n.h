/**
 * @file    OPTICSMATERIALS_n.h
 *
 */

#ifndef _OPTICSMATERIALS_N_H
#define _OPTICSMATERIALS_N_H

int OpticsMaterials_code(char *name);

char* OpticsMaterials_name(int code);

double OpticsMaterials_n(int material, double lambda);

double OpticsMaterials_pha_lambda( int material, double z, double lambda );


#endif


