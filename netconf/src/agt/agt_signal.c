/*  FILE: agt_signal.c

    Handle interrupt signals for the agent
                
*********************************************************************
*                                                                   *
*                  C H A N G E   H I S T O R Y                      *
*                                                                   *
*********************************************************************

date         init     comment
----------------------------------------------------------------------
22jan07      abb      begun

*********************************************************************
*                                                                   *
*                     I N C L U D E    F I L E S                    *
*                                                                   *
*********************************************************************/
#include  <stdio.h>
#include  <stdlib.h>
#include  <signal.h>
#include  <unistd.h>
#include  <sys/time.h>

#ifndef _H_procdefs
#include  "procdefs.h"
#endif

#ifndef _H_agt
#include "agt.h"
#endif

#ifndef _H_agt_signal
#include "agt_signal.h"
#endif

#ifndef _H_agt_timer
#include "agt_timer.h"
#endif

#ifndef _H_ncx
#include "ncx.h"
#endif

#ifndef _H_status
#include  "status.h"
#endif


/********************************************************************
*                                                                   *
*                       C O N S T A N T S                           *
*                                                                   *
*********************************************************************/

#ifdef DEBUG
#define AGT_SIGNAL_DEBUG 1
#endif

/* don't rely on GNU extension being defined */
typedef void (*sighandler_t)(int signum);

/********************************************************************
*                                                                   *
*                       V A R I A B L E S                            *
*                                                                   *
*********************************************************************/

static boolean agt_signal_init_done = FALSE;
static sighandler_t sh_int, sh_hup, sh_term, sh_pipe, sh_alarm;

/********************************************************************
* FUNCTION agt_signal_init
*
* Initialize the agt_signal module
*
* INPUTS:
*   none
* RETURNS:
*   NO_ERR if all okay, the minimum spare requests will be malloced
*********************************************************************/
void
    agt_signal_init (void)
{
    if (!agt_signal_init_done) {
        /* setup agt_signal_handler */
        sh_int = signal(SIGINT, agt_signal_handler);
        sh_hup = signal(SIGHUP, agt_signal_handler);
        sh_term = signal(SIGTERM, agt_signal_handler);
        sh_pipe = signal(SIGPIPE, agt_signal_handler);
        sh_alarm = signal(SIGALRM, agt_signal_handler);
        agt_signal_init_done = TRUE;
    }

} /* agt_signal_init */


/********************************************************************
* FUNCTION agt_signal_cleanup
*
* Cleanup the agt_signal module.
*
*********************************************************************/
void 
    agt_signal_cleanup (void)
{
    if (agt_signal_init_done) {
        /* restore previous handlers */
        signal(SIGINT, sh_int);
        signal(SIGHUP, sh_hup);
        signal(SIGTERM, sh_term);
        signal(SIGPIPE, sh_pipe);
        signal(SIGALRM, sh_alarm);
        agt_signal_init_done = FALSE;
    }

} /* agt_signal_cleanup */


/********************************************************************
* FUNCTION agt_signal_handler
*
* Handle an incoming interrupt signal
*
* INPUTS:
*   intr == interrupt numer
*
*********************************************************************/
void 
    agt_signal_handler (int intr)
{
    switch (intr) {
    case SIGINT:
        /* control-C */
        agt_request_shutdown(NCX_SHUT_EXIT);
        break;
    case SIGHUP:
        /* hang up treated as reset */
        agt_request_shutdown(NCX_SHUT_RESET);
        break;
    case SIGTERM:
        /* kill -1 */
        agt_request_shutdown(NCX_SHUT_EXIT);
        break;
    case SIGPIPE:
        /* broken pipe ignored */
        break;
    case SIGALRM:
        /* dispatch to the agent timer handler */
        agt_timer_handler();
        break;
    default:
        SET_ERROR(ERR_INTERNAL_VAL);
    }

} /* agt_signal_handler */

/* END file agt_signal.c */


