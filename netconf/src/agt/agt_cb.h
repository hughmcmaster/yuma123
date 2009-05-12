#ifndef _H_agt_cb
#define _H_agt_cb

/*  FILE: agt_cb.h
*********************************************************************
*								    *
*			 P U R P O S E				    *
*								    *
*********************************************************************

    NCX Agent Data Model callback handler

*********************************************************************
*								    *
*		   C H A N G E	 H I S T O R Y			    *
*								    *
*********************************************************************

date	     init     comment
----------------------------------------------------------------------
16-apr-07    abb      Begun; split out from agt_ps.h
01-aug-08    abb      Remove NCX specific stuff; YANG only now
*/

#ifndef _H_agt
#include "agt.h"
#endif

#ifndef _H_op
#include "op.h"
#endif

#ifndef _H_rpc
#include "rpc.h"
#endif

#ifndef _H_ses
#include "ses.h"
#endif

#ifndef _H_status
#include "status.h"
#endif

#ifndef _H_val
#include "val.h"
#endif

/********************************************************************
*								    *
*			 C O N S T A N T S			    *
*								    *
*********************************************************************/
/* symbolic tags for agt_cb_register_callback 'forall' boolean */
#define FORALL  TRUE
#define FORONE  FALSE


/********************************************************************
*								    *
*			     T Y P E S				    *
*								    *
*********************************************************************/

/* Callback function for agent object handler 
 * Used to provide a callback sub-mode for
 * a specific named object
 * 
 * INPUTS:
 *   scb == session control block
 *   msg == incoming rpc_msg_t in progress
 *   cbtyp == reason for the callback
 *   editop == the parent edit-config operation type, which
 *             is also used for all other callbacks
 *             that operate on objects
 *   newobj == container object holding the proposed changes to
 *           apply to the current config, depending on
 *           the editop value. Will not be NULL.
 *   curobj == current container values from the <running> 
 *           or <candidate> configuration, if any. Could be NULL
 *           for create and other operations.
 *
 * RETURNS:
 *    status:
 */
typedef status_t 
    (*agt_cb_fn_t) (ses_cb_t  *scb,
		    rpc_msg_t *msg,
		    agt_cbtyp_t cbtyp,
		    op_editop_t  editop,
		    val_value_t  *newval,
		    val_value_t  *curval);


/* set of agent object callback functions */
typedef struct agt_cb_fnset_t_ {
    agt_cb_fn_t    cbfn[AGT_NUM_CB];
} agt_cb_fnset_t;



/********************************************************************
*								    *
*			F U N C T I O N S			    *
*								    *
*********************************************************************/
extern void
    agt_cb_init (void);

extern void
    agt_cb_cleanup (void);

/* use the same fn for all callback phases 
 * all phases will be invoked
 */
extern status_t 
    agt_cb_register_callback (const xmlChar *modname,
			      const xmlChar *defpath,
			      const xmlChar *version,
			      const agt_cb_fn_t cbfn);

/* setup array of callbacks, could be different or NULL
 * to skip that phase
 */
extern status_t 
    agt_cb_register_callbacks (const xmlChar *modname,
			       const xmlChar *defpath,
			       const xmlChar *version,
			       const agt_cb_fnset_t *cbfnset);


extern void
    agt_cb_unregister_callbacks (const xmlChar *modname,
				 const xmlChar *defpath);


#endif	    /* _H_agt_cb */
