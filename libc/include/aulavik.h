/*====------------------ aulavik.h - Aulavik API header ------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef _AULAVIK_H
#define _AULAVIK_H 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int aulavik_main(int argc, char *argv[]);

_Noreturn void _start(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _AULAVIK_H */
