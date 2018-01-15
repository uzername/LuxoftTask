#ifndef MOREDEFS_H
#define MOREDEFS_H
//these bitmaps used in determining pawn additional cell movement
#define BM_movementperformed ((uint8_t)1)           /*0b00000001 . LSB*/
#define BM_gotpiecenorthorsouth ((uint8_t)2)        /*0b00000010*/
#define BM_gotpieceFarNorthorFarSouth ((uint8_t)4)  /*0b00000100*/
#define BM_positionclean ((uint8_t)7)               /*0b00000111*/

#endif // MOREDEFS_H
