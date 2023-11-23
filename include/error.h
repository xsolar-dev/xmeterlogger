#ifndef __ERROR_H__
#define __ERROR_H__

#include <sys/cdefs.h>
#include <features.h>
#include <errno.h>

// __BEGIN_CDECLS
#define	ENOERR		    0
#define EGENERR         1   /* Loi chung chung  */
#define ESYSERR         2   /* Loi he thong     */
#define ESVRERR         3   /* Loi may chu      */
#define EQUERR          4   /* Loi hang doi     */


// __END_CDECLS

#endif //