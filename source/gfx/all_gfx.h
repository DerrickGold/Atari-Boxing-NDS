//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H


// Sprite files : 
extern const unsigned char bsprite1_Sprite[28672] __attribute__ ((aligned (4))) ;  // Pal : bsprite1_Pal
extern const unsigned char bsprite2_Sprite[28672] __attribute__ ((aligned (4))) ;  // Pal : bsprite2_Pal

// Background files : 
extern const int base_Info[3]; // BgMode, Width, Height
extern const unsigned short base_Map[768] __attribute__ ((aligned (4))) ;  // Pal : base_Pal
extern const unsigned char base_Tiles[64] __attribute__ ((aligned (4))) ;  // Pal : base_Pal

extern const int ropes_Info[3]; // BgMode, Width, Height
extern const unsigned short ropes_Map[768] __attribute__ ((aligned (4))) ;  // Pal : ropes_Pal
extern const unsigned char ropes_Tiles[1408] __attribute__ ((aligned (4))) ;  // Pal : ropes_Pal


// Palette files : 
extern const unsigned short bsprite1_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short bsprite2_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short base_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short ropes_Pal[2] __attribute__ ((aligned (4))) ;


#endif

