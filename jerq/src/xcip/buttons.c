#include "cip.h"

extern RUsure ();
extern Point align();
extern struct thing * editThing();
extern int currentBrush, copyFlag, thingSelected, editDepth;
extern Rectangle brushes[];
extern int gridState, videoState, buttonState;
extern Word * saveBase;
extern Rectangle saveScreenmap;

int charPos = 0;

#ifdef MPX
char *resizeText[] = {
	" Open window ", 
	" Quit editor ", 
	NULL};
Menu resizeMenu = {resizeText};
#endif

Texture16 menucursor = {
	0xFFC0, 0x8040, 0x8040, 0x8040, 0xFFC0, 0xFFC0, 0xFE00, 0xFEF0,
	0x80E0, 0x80F0, 0x80B8, 0xFE1C, 0x800E, 0x8047, 0x8042, 0xFFC0};


Texture16 textCursor = {
	0x0000, 0x0000, 0x0000, 0xEE97, 0x4892, 0x4862, 0x4C62, 0x4C62,
	0x4862, 0x4892, 0x4E92, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};


  struct thing *
doMouseButtons(t,offset) 
  register struct thing *t;
  Point offset;
{
  Point m;
  register Rectangle *b;
  register struct thing *t1;

  spaceRemaining (0);		/* Show number of items left */

#ifdef MPX
  if (CORRECTSIZE) {
    if (P->state & MOVED ) {
      /* Window has been moved so must reset global variables. */
      Xmin = P->layer->rect.origin.x + BORDER;
      Ymin = P->layer->rect.origin.y + BORDER;

      drawOffset.x = Xmin;
      drawOffset.y = YPIC;
      saveBase      = P->layer->base;
      saveScreenmap = P->layer->rect;
      brushInit();
      P->state &= ~(MOVED|RESHAPED);

    } else if (P->state & RESHAPED) {
      /* Redraw screen if layer has been reshaped */
      redrawLayer(t, offset);
    }

    if ((own () & MOUSE) && ptinrect (MOUSE_XY, brushes[PIC])) {
      if (P->cursor != &crossHairs) {
	cursswitch (&crossHairs);
      }
    }
    else {
      if (P->cursor == &crossHairs) {
	cursswitch ((Texture16 *)0);
      }
    }

  } else {
    /* Not the correct size.  Don't go to main menu until layer is full size */
    cursswitch(&menucursor);

    while ( !button3() ) {
      sleep(2);
#ifdef DMD630
      if( (P->state & RESHAPED) && !(P->state & MOVED) ) {
#else
      if( (P->state & RESHAPED) ) {
#endif
	drawToolIcon();
        P->state &= ~RESHAPED;
      }
    }

    cursswitch ((Word *)NULL);

    switch ( menuhit(&resizeMenu, 3) ) {
	case 0: /* Open window */
	  {
	    resizeLayer(t,offset);
	    break;
	  }
	case 1: /* Exit editor */
	  {
	    if (RUsure ())  
		exit();
	    break;
	  }
    }
    return (t);

  }

#endif

  /* Add drawing frame absolute position to offset. */
  offset = add( drawOffset, offset );

  wait( MOUSE|KBD );
  m = sub(MOUSE_XY,offset);


  /* BUTTON 1 PRESSED */

  if ( button1() ) {

    /* Delete empty text string as no longer editing it */
    if( thingSelected && (t->type==TEXT) )
      if( t->otherValues.text.s[0] == '\0' )
        t = deleteThing(t);

    b = select(t, add(m,offset), offset);
    if ( b == &brushes[PIC]) {

      /* Pressed inside picture drawing area */
      copyFlag=0;
      t1 = selectThing(m,t);
      if (thingSelected==1) {
	drawSelectionLines(t,offset);
      }
      if (t1 != TNULL) {
	t = t1;
	thingSelected = 1;
	changeBrush(-1);
	if (t->type==MACRO||t->type==TEXT) {
	  changeButtons(MOVEbuttons);
	} else {
	  changeButtons(EDITbuttons);
	}

	flashThing(t,offset);

	if(t->type==TEXT) {
	  message("Edit text at marked spot\r");
	  putMessage("  ctrl-w deletes previous word\r");
	  putMessage("  ctrl-u deletes all previous chars");
	  charPos = moveTextSpot(t,offset);
	} else {
	  initMessage();
	  while( button1() )
	    nap(2);
	}

	drawSelectionLines(t,offset);
	flashThing(t,offset);

      } else {

	/* Nothing selected in picture area */
	thingSelected = 0;
	changeBrush(-1);
	changeButtons( INITbuttons );
	initMessage();
      }

    } else {

	/* Pressed in brush, edit level or some outside picture area */
      if (thingSelected==1) {
	drawSelectionLines(t,offset);
	thingSelected = 0;
      }
      if (b != (Rectangle *) NULL ) {
	/* Brush area selected */
	copyFlag=0;
	thingSelected = 0;
	switch( currentBrush ) {
	case TEXT:
	case BOX:
	case SPLINE:
	case LINE:
		changeButtons(GLOBbuttons);
		break;
	default:
		changeButtons(DRAWbuttons);
		break;
	}
      }
    }
    for (; button1(); nap(2))
	; 


  /* BUTTON 2 PRESSED */

  } else if (button2()) {
      if (thingSelected) { 
	if (copyFlag) {
	  drawSelectionLines (t, offset);
	  t = insert( copyThing( t, align(m), offset, 1 ), t );
	  drawSelectionLines (t, offset);
	  /* clear button 2 - only one copy per click */
	  for ( ; button2(); nap(2)) ;
	} else {
	  t = editThing(m,offset,t);
	}

      } else {
	/* No thing is selected */
	if ((currentBrush>=0)&&(ptinrect(add(m,offset),brushes[PIC]))) {
	    if (currentBrush==SPLINE) changeButtons(SPLINEbuttons);
	    t = place(currentBrush,m,t,offset);
	    switch( currentBrush ) {
	    case TEXT:
	    case BOX:
	    case SPLINE:
	    case LINE:
	  	changeButtons(GLOBbuttons);
	  	break;
	    default:
	    	changeButtons(DRAWbuttons);
	  	break;
	    }
	} else {
	  for (; button2(); ) ; 
	}
      }


  /* BUTTON 3 PRESSED */

  } else if (button3()) {
     if (thingSelected) {
	t = displayThingMenu( m, t, offset );
     } else {
	t = displayCommandMenu( t, offset );
     }


  /* KEYBOARD INPUT RECEIVED */

  } else if (own() & KBD ) {
      if (thingSelected && (t->type==TEXT) ) {
        charPos = editText(t,offset,charPos);
        BoundingBox(t);
      }
  }

  return(t);
}


  struct thing *
place(b,p,h,os)
  register int b;
  Point p, os;
  struct thing *h;
{
  register struct thing *t;
  Point r;
  register Point *plist, *olist;
  register int i, used, room;
  struct thing dummy;
  register char *	s;

  p = align(p);

  switch (b) {
    case CIRCLE: {
      if ((t = newCircle(p)) != TNULL) {
	t->otherValues.radius = 1;
	draw(t,os);
	h = insert(t,h);
	track(p,os,GROWCIRCLE,t);
	BoundingBox(t);
      }
      break;	
    }
    case BOX: {
      h = insert(&dummy,h);
      r = track(p,os,BOX,h);
      if ((t = newBox(canon (p, r))) != TNULL) {
	h = remove (&dummy);
	h = insert(t,h);
      }
      else {
	draw (h, os);
	h = remove (&dummy);
      }
      break;	
    }
    case ELLIPSE: {
      if ((t = newEllipse(p)) != TNULL) {
	t->otherValues.ellipse.ht = 1;
	t->otherValues.ellipse.wid = 1;
	draw(t,os);
	h = insert(t,h);
	track(p,os,ELLIPSE,t);
	BoundingBox(t);
      }
      break;	
    }
    case LINE: {
      h = insert(&dummy,h);
      r = track(p,os,LINE,h);
      if ((t = newLine(p,r)) != TNULL) {
	h = remove (&dummy);
	h = insert(t,h);
      }
      else {
	draw (h, os);
	h = remove (&dummy);
      }
      break;	
    }
    case ARC: {
      h = insert(&dummy,h);
      r = track(p,os,ARC,h);
      if ((t = newArc(p,r)) != TNULL) {
	h = remove (&dummy);
	h = insert(t,h);
      }
      else {
	draw (h, os);
	h = remove (&dummy);
      }
      break;	
    }
    case SPLINE: {
      if ((plist = (Point *)getSpace(5*sizeof(Point)))!=(Point *)NULL) {
	h = insert(&dummy,h);
	plist[1]=p;
	used = 1; room = 3;
	do {
	  if (used==room) {
	    olist = plist;
	    room <<= 1;
	    plist = (Point *) getSpace((room+2)*sizeof(Point));
	    if (plist==(Point *)NULL) {
	      draw (&dummy, os);
	      h = remove (&dummy);
	      plist = olist;		/* Free list later */
	      used = 0;
	      break;
	    }
	    for (i=1; i<=used; i++) {
	      plist[i]=olist[i];
	    }
	    free(olist);
	  }
	  if (button2()) {
	    ++used;
	    plist[used]= track(plist[used-1],os,LINE,h);
	    xsegment(add(os,plist[used-1]),
	              add(os,plist[used]));
	  }
	} while (!button3());
	for (; button3(); ) 
	  nap(2);
	drawZigZag(os,plist,used);
	if (used>2) {
	  t = newSpline(++used,room,plist);
	}
	else {
	  t = TNULL;
	  free (plist);
	}
	h = remove(&dummy);
	h = insert(t,h);
      }
      break;
    }
    case TEXT: {
      int firstTime;
      int charPos;

      firstTime = 1;
      charPos   = 0;

      s = getSpace(1);
      s[0] = '\0';
      t = newText(p,s);
      if( t==TNULL )
	break;

      while( button123() )
	nap(2);
      cursswitch( &textCursor );
      changeButtons(EXITbuttons);
      clearKeyboard();


      while (1) {
	wait( KBD | MOUSE );
	if( own() & KBD ) {
	  if( firstTime ) {
	    cursinhibit();
	    firstTime = 0;
	  }
	  charPos = editText(t,os,charPos);
	}
	if( own() & MOUSE )
	  if( button123() )
	    break;
      }; 

      while( button123() )
	nap(2);
      
      if( !firstTime )
	cursallow();
      cursSwitch();

      h = insert(t,h);

      if( t->otherValues.text.s[0] == '\0' ) {
	h = deleteThing(t);
	t = TNULL;
      } else {
	BoundingBox(t);
      }

      break;
    }
  }


  if (t!=TNULL) {
    if( eqpt(t->bb.origin,t->bb.corner) )
      /* thing created has no size so remove it */
      h = deleteThing(t);
    else
      if( (t->type!=CIRCLE) && (t->type!=ELLIPSE) && (t->type!=TEXT) )
        draw(t,os);
  }

  return(h);
}


clearKeyboard()
{
  for (; kbdchar() != -1; ) ;
}


    Point
randPt()
{
    Point p;

    p.x = (rand() % 500) + 150;
    p.y = (rand() % 700) + 150;

    return p;
}
