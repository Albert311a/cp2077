#include <unistd.h>
#include "matrix.cpp"
#include "ui.cpp"


int main( int argc, char *argv[ ] ){


    int a;
    while( ( a = getopt( argc, argv, "h" ) ) != -1 ){
        switch( a ){
            case 'h':
                a = 1;
                break;
        }
    }
    cp2077( );

    Frame P( a );
    P.startCracking( );

    cleanup( );
    return 0;
}
