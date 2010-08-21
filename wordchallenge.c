//Copyright 2010, Nathaniel Flath
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define true 1
#define false 0
#define NUM_WORDS 14243

typedef int bool;

char** dictionary;
Display* display;

void loop_through_dictionary( char* letters );
void send_key( KeyCode c, int t);
int load_dict();

//Driver for the Word Challenge bot.  Loads the dictionary, then loops forever
//performing the following process:
//  1. Take ascreenshot
//  2. Cut out the individual letters from the screenshot and run them through
//     Tesseract
//  3. Read the output of Tesseract and use these to send every word
//     combination using these letters to Word Challenge.
//After ten seconds have elapsed, the program kills itself.  If this does not happen,
//no progress is being made and should be restarted (by the perl script that keeps it running.)
//This program is very dependant on your resolution and location/size of the browser you have Word Challenge opened in.
int main(int argc, char *argv[]) {
  int i;
  time_t start, end;
  display = XOpenDisplay(NULL);
  dictionary = malloc( sizeof( char* ) * NUM_WORDS );
  load_dict();

  char* c = malloc( sizeof( char ) * 10 );
  char* old = malloc( 10 );
  start = time (NULL);
  while( true ) {
    end = time (NULL);
    send_key( XKeysymToKeycode( display, XK_Return ), 0);
    send_key( XKeysymToKeycode( display, XK_Control_L ), 0);

    sleep( 1 );
    system( "scrot screen.jpg" );
    system( "convert screen.jpg -crop 40x35+400+500 -type Grayscale letter0.tif" );
    system( "convert screen.jpg -crop 38x35+455+500 -type Grayscale letter1.tif" );
    system( "convert screen.jpg -crop 38x35+510+500 -type Grayscale letter2.tif" );
    system( "convert screen.jpg -crop 38x35+565+500 -type Grayscale letter3.tif" );
    system( "convert screen.jpg -crop 37x35+620+500 -type Grayscale letter4.tif" );
    system( "convert screen.jpg -crop 37x35+675+500 -type Grayscale letter5.tif" );
    system( "tesseract letter0.tif letter0" );
    system( "tesseract letter1.tif letter1" );
    system( "tesseract letter2.tif letter2" );
    system( "tesseract letter3.tif letter3" );
    system( "tesseract letter4.tif letter4" );
    system( "tesseract letter5.tif letter5" );
    FILE* l0 = fopen( "letter0.txt", "r" );
    FILE* l1 = fopen( "letter1.txt", "r" );
    FILE* l2 = fopen( "letter2.txt", "r" );
    FILE* l3 = fopen( "letter3.txt", "r" );
    FILE* l4 = fopen( "letter4.txt", "r" );
    FILE* l5 = fopen( "letter5.txt", "r" );
    fscanf( l0, "%c", c );
    fscanf( l1, "%c", c + 1 );
    fscanf( l2, "%c", c + 2 );
    fscanf( l3, "%c", c + 3 );
    fscanf( l4, "%c", c + 4 );
    fscanf( l5, "%c", c + 5 );
    system( "rm letter*.txt" );
    for( i = 0; c[i]; i++ ){
      if (c[i] == '0' ) c[i] = 'o';
      else if (c[i] < 'A' || c[i] > 'Z' ) c[i] = 'i';
      else c[i] += 32;
    }
    c[6] = 0;
    printf( "%s\n", c );
    if( strcmp(old,  c ) ) {
      start = time (NULL);
      loop_through_dictionary( c );
      strcpy( old, c );
    }
    if (end - start >= 10) {
      XCloseDisplay (display);
      return 0;
    }
  }

  XCloseDisplay( display );
  return 0;
}

//Simulates typing of a single key.
void send_key( KeyCode k, int time )
{
  XTestFakeKeyEvent( display, k, True, time );
  XTestFakeKeyEvent( display, k, False, time );
}

//Sends the given word to the Word Challenge player.
void send_word( char* word ) {
  char* c = word;
  int time = 0;
  send_key( XKeysymToKeycode( display, XK_Return), time );
  while( *c ) {
    time += 1;
    switch( *c ){
    case 'a': send_key( XKeysymToKeycode( display, XK_A ), time ); break;
    case 'b': send_key( XKeysymToKeycode( display, XK_B ), time ); break;
    case 'c': send_key( XKeysymToKeycode( display, XK_C ), time ); break;
    case 'd': send_key( XKeysymToKeycode( display, XK_D ), time ); break;
    case 'e': send_key( XKeysymToKeycode( display, XK_E ), time ); break;
    case 'f': send_key( XKeysymToKeycode( display, XK_F ), time ); break;
    case 'g': send_key( XKeysymToKeycode( display, XK_G ), time ); break;
    case 'h': send_key( XKeysymToKeycode( display, XK_H ), time ); break;
    case 'i': send_key( XKeysymToKeycode( display, XK_I ), time ); break;
    case 'j': send_key( XKeysymToKeycode( display, XK_J ), time ); break;
    case 'k': send_key( XKeysymToKeycode( display, XK_K ), time ); break;
    case 'l': send_key( XKeysymToKeycode( display, XK_L ), time ); break;
    case 'm': send_key( XKeysymToKeycode( display, XK_M ), time ); break;
    case 'n': send_key( XKeysymToKeycode( display, XK_N ), time ); break;
    case 'o': send_key( XKeysymToKeycode( display, XK_O ), time ); break;
    case 'p': send_key( XKeysymToKeycode( display, XK_P ), time ); break;
    case 'q': send_key( XKeysymToKeycode( display, XK_Q ), time ); break;
    case 'r': send_key( XKeysymToKeycode( display, XK_R ), time ); break;
    case 's': send_key( XKeysymToKeycode( display, XK_S ), time ); break;
    case 't': send_key( XKeysymToKeycode( display, XK_T ), time ); break;
    case 'u': send_key( XKeysymToKeycode( display, XK_U ), time ); break;
    case 'v': send_key( XKeysymToKeycode( display, XK_V ), time ); break;
    case 'w': send_key( XKeysymToKeycode( display, XK_W ), time ); break;
    case 'x': send_key( XKeysymToKeycode( display, XK_X ), time ); break;
    case 'y': send_key( XKeysymToKeycode( display, XK_Y ), time ); break;
    case 'z': send_key( XKeysymToKeycode( display, XK_Z ), time ); break;
    }
    c++;
  }
  send_key( XKeysymToKeycode( display, XK_Return ), time);
}

//Sends all words in the dictionary that can be formed by the given set of letters, then switches to the next
//set of letters.
void loop_through_dictionary( char* letters ) {
  int x;
  for( x = 0; x < NUM_WORDS; x++ ) {
    if( composed_of( dictionary[x], letters ) && strlen( dictionary[x]) < 6 ) {
      send_word( dictionary[x] );
    }
  }
  sleep( 1 );
  send_key( XKeysymToKeycode( display, XK_Control_L ) , 1);
}

//Loads the dictionary from file
int load_dict()
{
  int x;
  FILE* f = fopen( "corncob_lowercase.txt", "r" );
  for( x = 0; x < NUM_WORDS; x++ ) {
    char* c = malloc( 20 );
    fscanf( f, "%s", c );
    dictionary[x] = c;
  }
}

//Returns whether the given word can be created using the given letters.
//Assumes some incorrectness in the optical recognition; has several
//common mistakes hardcoded.
bool composed_of( char* word,  char* letters ) {
  char* c = word;
  bool unknown = false;
  while( *c ) {
    char* l = letters;
    bool found = false;
    while( *l ) {
      if( *l == *c ) {
        found = true;
        break;
      }
      if ( ( *l == 'y' && *c == 'p') ||
           ( *l == 'w' && *c == 'm') ||
           ( *l == 'i' && *c == 'r') ||
           ( *l == 'i' && *c == 's') ) {
        found = true;
        break;
      }

      l++;
    }

    if( !found ) return false;
    c++;
  }
  return true;
}
