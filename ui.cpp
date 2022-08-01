#include <curses.h>
#include "ui.h"

WINDOW *uiwindow = NULL;

bool cp2077( ){
    uiwindow = initscr( );
    if( uiwindow == NULL ) return false;

    start_color( );
    if( !has_colors( ) || !can_change_color( ) ){
       printf( "warning. your terminal can't handle this program.\n" );
       return false;
    }

    clear( );
    noecho( );
    curs_set( 0 );
    crmode( );

    return true;
}

void cleanup( ){
    delwin( uiwindow );
    endwin( );
    refresh( );
}
