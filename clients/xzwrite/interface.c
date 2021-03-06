/*
 * The xzwrite interface structure.  All top level widgets except toplevel
 * are popup shells.
 *
 * toplevel - the top level shell
 * 	icon - the top level "Z" icon
 * 
 * sendWindow - the popup shell for the editor/destlist
 * 	sendForm - the form holding the edit tree and dest tree
 * 	sendClose - button to close sendWindow
 * 
 * 	editPane - the pane holding editor widgets
 * 		editTitle - the label holding the zephyr triple
 * 		editForm - the box holding editor command buttons
 * 			editSend - button to send message
 * 			editClear - button to clear editor
 * 			editPrev - button to yank previous
 * 			editNext - button to yank next
 * 	 	editor - the text editor
 * 
 * 	destForm - the form holding the destinations list/button
 * 		destScroll - the scrollbar holding the list
 * 			destList - the destination list
 * 
 * menuWindow - the popup shell for the menu
 * 	menuForm - the form holding the menu list/button
 * 		menuClose - the Close Window button for the dest list
 * 		signature - button to change signature
 *              clearOnSend
 * 		closeOnSend
 * 		pings
 * 		verbose
 * 		authentic
 * 		yankDest
 * 		addGlobals
 * 		classInst
 * 		exitProgram
 *
 * getStringWindow - the popup shell for dialog boxes (GetString.c)
 * 	getStringForm - the form containing the dialog widgets
 * 		getStringTitle - the title label width
 * 		getStringEdit - the text editor
 * 		getStringAccept - the accept button
 * 		getStringCancel - the cancel button
 */

#include "xzwrite.h"
#include "GetString.h"

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Viewport.h>

#include <zephyr/zephyr.h>	/* for ZGetFD() */

#define XVCMW XtVaCreateManagedWidget

/* Action Procedure declarations */
static void Quit(), SendMessage(), OpenSend(), CloseSend(),
     ClearEditor(), YankPrev(), YankNext(), YankStore(), AlignParagraph(),
     DeleteDest(), HighlightDest(), SelectDest(), OpenMenu(),
     ToggleOption(), Signature(), CloseMenu(), CreateDest();

static void set_editor_width(), set_sendclose_width();

static XtActionsRec actionTable[] = {
     /* sendWindow actions */
     {"OpenSend", (XtActionProc) OpenSend},
     {"CloseSend", (XtActionProc) CloseSend},
     
     /* Editor actions */
     {"Quit", (XtActionProc) Quit},
     {"SendMessage", (XtActionProc) SendMessage},
     {"ClearEditor", (XtActionProc) ClearEditor},
     {"YankStore", (XtActionProc) YankStore},
     {"YankPrev", (XtActionProc) YankPrev},
     {"YankNext", (XtActionProc) YankNext},
     {"AlignParagraph", (XtActionProc) AlignParagraph},

     /* Destination list actions */
     {"SelectDest", (XtActionProc) SelectDest},
     {"DeleteDest", (XtActionProc) DeleteDest},
     {"CreateDest", (XtActionProc) CreateDest},
     {"HighlightDest", (XtActionProc) HighlightDest},

     /* Menu actions */
     {"OpenMenu", (XtActionProc) OpenMenu},
     {"ToggleOption", (XtActionProc) ToggleOption},
     {"Signature", (XtActionProc) Signature},
     {"CloseMenu", (XtActionProc) CloseMenu},
};

extern unsigned int num_options, num_resources;
extern String fallback_resources[];
extern XrmOptionDescRec app_options[];
extern XtResource app_resources[];

XtAppContext app_con;
Defaults defs;

/* Widgets */
Widget toplevel, icon, getString;
Widget sendWindow, sendForm, sendClose;
Widget destForm, destScroll, destList;
Widget editPane, editTitle, editForm, editSend, editClear,
     editPrev, editNext, editor;
Widget menuWindow, menuForm, menuClose, signature, 
     clearOnSend, closeOnSend, pings, verbose, authentic,
     yankDest, addGlobals, classInst, commandMask, exitProgram;

void go()
{
     XtAppMainLoop(app_con);
}

void build_interface(argc, argv)
   int *argc;
   char **argv;
{
     /* Set XFILESEARCHPATH to find xzwrite's resource file */
     /* XXX This is gross XXX */
     {
	  char *path1, *path2;
	  
	  path1 = (char *) getenv("XFILESEARCHPATH");
	  if (! path1) path1 = "";
	  path2 = (char *) malloc(strlen(path1) +
#ifdef HAVE_PUTENV
				  strlen("XFILESEARCHPATH=") +
#endif
				  strlen(DATADIR) + 12);
	  if (path2 != NULL) {
#ifdef HAVE_PUTENV
	       sprintf(path2, "XFILESEARCHPATH=%s:%s/zephyr/%%N", path1,
		       DATADIR);
	       putenv(path2);
#else
	       sprintf(path2, "%s:%s/zephyr/%%N", path1, DATADIR);
	       setenv("XFILESEARCHPATH", path2, 1);
	       free(path2);
#endif
	  }
     }
     
     toplevel = XtVaAppInitialize(&app_con, "XZwrite", app_options,
#if XtSpecificationRelease > 4
				  num_options, argc, argv,
#else
				  num_options, (Cardinal *) argc, argv,
#endif
				  fallback_resources, NULL);

     XtVaGetApplicationResources(toplevel, (XtPointer) &defs, app_resources,
				 num_resources, NULL);

     XtAppAddActions(app_con, actionTable, XtNumber(actionTable));

     /* Create the icon */
     icon = XVCMW("icon", commandWidgetClass, toplevel, NULL);

     /* Create the menu */
     menuWindow = XtVaCreatePopupShell("menuWindow", transientShellWidgetClass,
				       toplevel, NULL);
     menuForm = XVCMW("menuForm", formWidgetClass, menuWindow, NULL);
     menuClose = XVCMW("menuClose", commandWidgetClass, menuForm, NULL);
     signature = XVCMW("signature", commandWidgetClass, menuForm, NULL);
     clearOnSend = XVCMW("clearOnSend", toggleWidgetClass, menuForm, NULL);
     closeOnSend = XVCMW("closeOnSend", toggleWidgetClass, menuForm, NULL);
     pings = XVCMW("pings", toggleWidgetClass, menuForm, NULL);
     verbose = XVCMW("verbose", toggleWidgetClass, menuForm, NULL);
     authentic = XVCMW("authentic", toggleWidgetClass, menuForm, NULL);
     yankDest = XVCMW("yankDest", toggleWidgetClass, menuForm, NULL);
     addGlobals = XVCMW("addGlobals", toggleWidgetClass, menuForm, NULL);
     classInst = XVCMW("classInst", toggleWidgetClass, menuForm, NULL);
     exitProgram = XVCMW("exitProgram", commandWidgetClass, menuForm, NULL);

     /* Create the editor/destination list */
     sendWindow = XtVaCreatePopupShell("sendWindow", transientShellWidgetClass,
				       toplevel, NULL);
     sendForm = XVCMW("sendForm", formWidgetClass, sendWindow, NULL);
     sendClose = XVCMW("sendClose", commandWidgetClass, sendForm, NULL);
     
     editPane = XVCMW("editPane", panedWidgetClass, sendForm, NULL);
     editTitle = XVCMW("editTitle", labelWidgetClass, editPane, NULL);
     editForm = XVCMW("editForm", formWidgetClass, editPane, NULL);
     editSend = XVCMW("editSend", commandWidgetClass, editForm, NULL);
     editClear = XVCMW("editClear", commandWidgetClass, editForm, NULL);
     editPrev = XVCMW("editPrev", commandWidgetClass, editForm, NULL);
     editNext = XVCMW("editNext", commandWidgetClass, editForm, NULL);
     editor = XVCMW("editor", asciiTextWidgetClass, editPane, NULL);

     destForm = XVCMW("destForm", formWidgetClass, sendForm, NULL);
     destScroll = XVCMW("destScroll", viewportWidgetClass, destForm, NULL);
     destList = XVCMW("destList", listWidgetClass, destScroll, NULL);

     XtSetKeyboardFocus(sendForm, editor);
     getString = InitGetString(toplevel, "getStringWindow");

     XtAppAddInput(app_con, ZGetFD(), (XtPointer)XtInputReadMask, zeph_dispatch, NULL);

     if (defs.track_logins) {
	  XtAppAddWorkProc(app_con, (XtWorkProc)login_scan_work, NULL);
     }

     set_editor_width();
     set_sendclose_width();
     XtRealizeWidget(toplevel);
}

/* Action Procedures */     
     
/* ARGSUSED */
static void Quit(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     XtDestroyApplicationContext(app_con);
     ZCancelSubscriptions(0);
     exit(0);
}

/* ARGSUSED */
static void OpenSend(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     XtPopup(sendWindow, XtGrabNone);
}

/* ARGSUSED */
static void CloseSend(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     XtPopdown(sendWindow);
}

/* ARGSUSED */
static void SendMessage(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     send_message();
}

/* ARGSUSED */
static void ClearEditor(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     edit_clear();
}

/* ARGSUSED */
static void YankStore(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     edit_yank_store();
}

/* ARGSUSED */
static void YankPrev(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     edit_yank_prev();
}

/* ARGSUSED */
static void YankNext(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     edit_yank_next();
}

/* ARGSUSED */
static void AlignParagraph(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
}

/* ARGSUSED */
static void SelectDest(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     select_dest();
}

/* ARGSUSED */
static void DeleteDest(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     delete_dest();
}

/* ARGSUSED */
static void HighlightDest(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
}

/* ARGSUSED */
static void CreateDest(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     create_dest();
}

/* ARGSUSED */
static void OpenMenu(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     XtPopup(menuWindow, XtGrabNone);
}

/* ARGSUSED */
static void ToggleOption(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     menu_toggle(w);
}

/* ARGSUSED */
static void Signature(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     menu_signature();
}

/* ARGSUSED */
static void CloseMenu(w, e, p, n)
   Widget w;
   XEvent *e;
   String *p;
   Cardinal *n;
{
     XtPopdown(menuWindow);
}

static void set_editor_width ()
{
  int w, c; char m = 'm';
  XFontStruct *fs = (XFontStruct *) NULL;

  c = defs.columns;
  /* get the font structure. */
  XtVaGetValues(editor, XtNfont, &fs, NULL);
  if (c < 1 || fs == (XFontStruct *) NULL) return;

  /* set the editor width */
  w = c * XTextWidth(fs, &m, 1);
  XtVaSetValues(editor, XtNwidth, (Dimension)w, NULL);
  
  /* set the destList to have 3/8ths the width of the editor */
  /* an other idea would be to make it have 3/8ths as many characters,
     which makes a difference when the editor and destList are in
     different fonts.
     however, I prefer this way. */
  XtVaSetValues(destForm, XtNwidth, (Dimension)(w*3/8), NULL);
}

static void set_sendclose_width ()
{
  /* make the Close Window button the width of the form */
  Dimension wi = 0;
  XtRealizeWidget (sendWindow);
  XtVaGetValues(sendForm, XtNwidth, &wi, NULL);
  XtUnrealizeWidget (sendWindow);
  XtVaSetValues(sendClose, XtNwidth, wi, NULL);
}
