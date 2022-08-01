#ifndef MATRIX
#define MATRIX

#include <vector>
#include <string>
#include <set>


typedef struct Ncrack{
    std::string str;
    std::set<int> s;
    unsigned n;
    int intensity;
} cell;

class Frame{
    public:
        Frame( int );
        void seed( );
        std::string getString( );
        void startCracking( );
        void updateNumbers( );
        void displayNumbers( );
        void genRand( );
        void reset( );
        int gouse_distribution( );
        void set_colors( );
        void rand_set( std::set<int> &  );
        void win_size( );
        int intensity_( );
        void border_check( );
        void matrix_resize( const unsigned &, const unsigned & );
        void cell_fill( cell &cl );

    private:
        bool border_stat;
        int X, Y;
        int type, dSize;
        std::vector<std::vector<cell>> c;
        unsigned vx, vy;
        std::vector<int> y1;
        std::vector<int> rnds;
        std::set<int>::iterator it;
        int col, row;
        const char d[ 10 ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
};

#endif
