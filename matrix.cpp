#include <thread>
#include <chrono>
#include <random>
#include <curses.h>
#include <string>
#include <set>
#include <sys/ioctl.h>  // get window size   ioctl( )
#include <unistd.h>     // miscellaneous symbolic constants   STDOUT_FILENO

#include "matrix.h"

void Frame::startCracking( ){
    seed( );
    set_colors( );
    noecho( );
    crmode( );
    curs_set( 0 );

    while( 1 ){
        if( border_stat == false ){
            clear( );
            border_stat = true;
        }
        border_check( );
        updateNumbers( );
        genRand( );
        displayNumbers( );
        std::this_thread::sleep_for( std::chrono::milliseconds( 80 ) );
    }
}

void Frame::updateNumbers( ){
    std::default_random_engine gen( (std::random_device( ) ) ( ) );
    std::uniform_int_distribution<int> dig( 0, 9 );
    std::uniform_int_distribution<int> prob( 0, 99 );
    for( auto &y: c ){
        for( auto &x: y ){
            int n = prob( gen );
            if( n > 97 ){
                rand_set( x.s );
            }
            for( it = x.s.begin( ); it != x.s.end( ); ++it ){
                x.str[ *it ] = d[ dig( gen ) ];
            }
        }
    }
}

int Frame::gouse_distribution( ){
    int n = -1;
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> d{5,2};
    while( n < 0 || n > 9 )
        n = std::round(d(gen));
    return n;
}

void Frame::rand_set( std::set<int> &s ){
    s.clear( );
    std::default_random_engine gen( (std::random_device( ) ) ( ) );
    std::uniform_int_distribution<unsigned> num( 0, 9 );
    while( s.size( ) < num( gen ) ){
        s.insert( num( gen ) );
    }
}

void Frame::genRand( ){
    std::default_random_engine gen( ( std::random_device( ) ) ( ) );
    std::uniform_int_distribution<int> nmb( 8700, 9999 );
    for( unsigned i = 0; i < vx; i++ ){
        if( rnds[ i ] < nmb( gen ) ){
            y1[ i ]++;
            if( y1[ i ] == 10 ) y1[ i ] = 0;
        }
    }
}

void Frame::displayNumbers( ){
    std::vector<int> g1( vx, 0 );
    for( unsigned i =0; i < vx; i++ ){
        g1[ i ] = y1[ i ];
    }
    for( unsigned i = 0; i < vy -1; i++ ){
        int q = 0;
        for( unsigned x = 0; x < vx; x++ ){
            if( c[ g1[ x ] ][ x ].s.size( ) == 0 ){
                color_set( 8, NULL );
            }else
                color_set( c[ g1[ x ] ][ x ].intensity, NULL );
            move( i, q );
            addstr( c[ g1[ x ] ][ x ].str.c_str( ) );
            q += 12;
        }
        for( unsigned k = 0; k < vx; k++ ){
            g1[ k ]--;
            if( g1[ k ] <= -1 ) g1[ k ] = vy -1;
        }
    }
    refresh( );
}

void Frame::set_colors( ){
    for( int i = 0; i < 8; i++ ){
        init_pair( i + 1, i, COLOR_BLACK );
    }
    init_pair( 8, COLOR_BLACK, 7 );
    for( int i = 0; i <= 5; i++ ){
        init_color( i, i * 20, i * 150, i * 200 );
    }
    init_color( 6, 100, 800, 1000 );
    init_color( 7, 100, 800, 1000 );
}

int Frame::intensity_( ){
    std::default_random_engine gen( ( std::random_device( ) ) ( ) );
    std::uniform_int_distribution<int> rng( 3, 7 );
    return rng( gen );
}

void Frame::seed( ){
    std::default_random_engine gen( (std::random_device( ) ) ( ) );
    std::uniform_int_distribution<int> dig( 0, 9 );
    std::uniform_int_distribution<int> rng( 3, 7 );
    std::uniform_int_distribution<int> spd( 1, 9 );
    std::uniform_int_distribution<int> set_seed( 9900, 9930 );

    border_stat = false;
    for( unsigned i = 0; i < vy; i++ ){
        for( unsigned x = 0; x < vx; x++ ){
            c[ i ][ x ].n = dig( gen );
            c[ i ][ x ].intensity = rng( gen );
            if( c[ i ][ x ].n < 4 || c[ i ][ x ].n > 7 ) c[ i ][ x ].n = 4;
            while( c[ i ][ x ].s.size( ) <  c[ i ][ x ].n )
                c[ i ][ x ].s.insert( dig( gen ) );
            c[ i ][ x ].str = getString( );
        }
    }
    for( unsigned i = 0; i < vy; i++ ){
        rnds.push_back( set_seed( gen ) );
        y1.push_back( 0 );
    }
}

std::string Frame::getString( ){
    std::default_random_engine gen( (std::random_device( ) ) ( ) );
    std::uniform_int_distribution<long> lng( 1000000000, 9999999999 );
    std::string str = std::to_string( lng( gen ) );
    return str;
}

void Frame::win_size( ){
    struct winsize w;
    ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );
    vx = w.ws_row / 12;
    vy = w.ws_col - 1;
}

Frame::Frame( int a){
    win_size( );
    if( a == 1 ){
        dSize = 15;
    } else dSize = 9;

    for( unsigned j = 0; j < vy; j++ ){
        std::vector<cell> cx;
        for( unsigned i = 0; i < vx; i++ ){
            cell cl;
            cx.push_back( cl );
        }
        c.push_back( cx );
    }
}

void Frame::border_check( ){
    struct winsize w;
    ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );
    unsigned y = w.ws_row ;
    unsigned x = w.ws_col;
    unsigned dx = x / 12;
    if( y - 1 != vy || dx != vx ){
        matrix_resize( y, x );
    }
}

void Frame::cell_fill( cell &cl ){
    std::default_random_engine gen( ( std::random_device( ) )( ) );
    std::uniform_int_distribution<unsigned> dig( 0, 9 );
    unsigned i = dig( gen );
    while( cl.s.size( ) < i ){
        cl.s.insert( dig( gen ) );
    }
    cl.str = getString( );
    cl.intensity = intensity_( );
}

void Frame::matrix_resize( const unsigned &y, const unsigned &x ){
    if( y > vy ){
        border_stat=false;
        while( c.size( ) < y -1 ){
            std::vector<cell> cx;
            for( unsigned i = 0; i <  vx; i++ ){
                cell cl;
                cell_fill( cl );
                cx.push_back( cl );
            }
            c.push_back( cx );
        }
    }

    if( x / 12 > vx ){
        border_stat = false;
        for( unsigned i = 0 ; i < y -1; i++ ){
            while( c[ i ].size( ) < x ){
                cell cl;
                cell_fill( cl );
                c[ i ].push_back( cl );
            }
        }
    }
    vy = y - 1;
    vx = x / 12;
}
