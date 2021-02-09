/**
 * This is a driver routine that calls a Mandelbrot escape time compute function (f(z, ...)).
 *
 * In this repo it is linked against three different compute functions, using:
 * - std::complex<double>
 * - 2x2 real Pauli matrices (2D only).
 * - 2x2 complex Pauli matrices (3D).
 *
 * The first two compute functions ignore all but the x,y coordinates that are provided.
 */
#ifdef HAVE_IMAGEMAGICK
#include <Magick++.h>
#endif
#include <assert.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <netcdf>

using namespace netCDF;
using namespace netCDF::exceptions;

// The basic Mandelbrot algorithm can be found in:
//    https://en.wikipedia.org/wiki/Mandelbrot_set

int f( double x, double y, double z, int maxiter, double outofbounds );

enum class options : int {
    xmin,
    xmax,
    ymin,
    ymax,
    zmin,
    zmax,
    nx,
    ny,
    nz,
    filename,
    dirname,
    maxiter,
    thresh,
    asimage,
    netcdf,
    bw,
    binary,
    progress,
    help,
};

enum class blackorwhite : uint8_t {
    black = 0,
    white = 255
};

struct RGB {
    uint8_t c[3]{};

    RGB( enum blackorwhite c_ ) : c{(uint8_t)c_, (uint8_t)c_, (uint8_t)c_} {}
    RGB( uint8_t r, uint8_t g, uint8_t b ) : c{r,g,b} {}
    RGB( ) {}
};

struct option_values {
    int NX{100};
    int maxiter{30};
    int thresh{4};
    int NY{};
    int NZ{};
    double x0{-2.5};
    double x1{1};
    double y0{-1};
    double y1{1};
    double z0{0};
    double z1{0};
    std::string filename{};
    std::string dirname{};
    bool asimage{};
    bool netcdf{};
    bool bw{};
    bool binary{};
    int progress{};
} g_opts;

class IOstate {
    FILE *                  fp{};

    void writeimage( int * a, int k, double z, double dx, double dy, double dz );
    void writecdf( int * a );
    void writefile( int * a, int k, double z, double dx, double dy, double dz );
public:
    IOstate();
    void writeit( int * a, int k, double z, double dx, double dy, double dz );
};

void showHelpAndExit() {
    std::cerr <<
             "usage: mandlebrot\n"
             "\t--filename=path [--asimage | --binary | --netcdf] [--dirname=.../]\n"
             "\t[--xmin=-2.5]\n"
             "\t[--xmax=1]\n"
             "\t[--ymin=-1]\n"
             "\t[--ymax=1]\n"
             "\t[--zmin=0]\n"
             "\t[--zmax=0]\n"
             "\t[--nx=100]\n"
             "\t[--ny=N\n"
             "\t[--nz=N]\n"
             "\t[--maxiter=30]\n"
             "\t[--thresh=4]\n"
             "\t[--progress=N]\n"
             "\t[--bw]\n"
             "\t--help]\n"
             "\t--debug]\n"
             "\n"
             "This program computex the mandelbrot set for a plane or range of planes.\n";

    std::exit( 1 );
}

inline RGB mixer( double minimum, double maximum, double value ) {
    // https://stackoverflow.com/a/20792531/189270
    auto ratio = 2 * ( value - minimum ) / ( maximum - minimum );
    uint8_t r = (uint8_t)std::max( 0.0, 255 * ( ratio - 1 ) );
    uint8_t b = (uint8_t)std::max( 0.0, 255 * ( 1 - ratio ) );
    uint8_t g = 255 - b - r;
#if 1

    RGB rc{ r, g, b };
#elif 1
    // RGB rc{b, b, b};
    RGB rc{ g, g, g };
#elif 0
    // more like:
    // https://www.codingame.com/playgrounds/2358/how-to-plot-the-mandelbrot-set/mandelbrot-set
    RGB rc{ r, r, r };
#endif

    return rc;
}

void computeplane( int *a, int k, double z, double dx, double dy ) {
    int nx = g_opts.NX;
    int ny = g_opts.NY;
    int nz = g_opts.NZ;

    bool bwfilter = g_opts.netcdf && g_opts.bw;

    int i = 0;
    double x = g_opts.x0;
    for ( ; i < nx; x += dx, i++ ) {
        int j = 0;
        double y = g_opts.y0;
        for ( ; j < ny; y += dy, j++ ) {
            int n = f( x, y, z, g_opts.maxiter, g_opts.thresh );

            auto p = i + nx * (j + ny * k);
            assert( p < nx * ny * nz );

            int m = bwfilter ?  ( n == g_opts.maxiter ) : n;
            a[ p ] = m;
        }
    }
}

int main( int argc, char ** argv ) {
    int c{ 0 };

    constexpr struct option longOptions[] {
        { "xmin", 1, nullptr, (int)options::xmin },
        { "xmax", 1, nullptr, (int)options::xmax },
        { "ymin", 1, nullptr, (int)options::ymin },
        { "ymax", 1, nullptr, (int)options::ymax },
        { "zmin", 1, nullptr, (int)options::zmin },
        { "zmax", 1, nullptr, (int)options::zmax },
        { "nx", 1, nullptr, (int)options::nx },
        { "ny", 1, nullptr, (int)options::ny },
        { "nz", 1, nullptr, (int)options::nz },
        { "filename", 1, nullptr, (int)options::filename },
        { "dirname", 1, nullptr, (int)options::dirname },
        { "maxiter", 1, nullptr, (int)options::maxiter },
        { "thresh", 1, nullptr, (int)options::thresh },
#ifdef HAVE_IMAGEMAGICK
        { "asimage", 0, nullptr, (int)options::asimage },
#endif
        { "netcdf", 0, nullptr, (int)options::netcdf },
        { "bw", 0, nullptr, (int)options::bw },
        { "binary", 0, nullptr, (int)options::binary },
        { "progress", 1, nullptr, (int)options::progress },
        { "help", 0, nullptr, (int)options::help },
        { nullptr, 0, nullptr, 0 }
    };

    while ( -1 !=
            ( c = getopt_long( argc, argv, "", longOptions, nullptr ) ) ) {
        switch ( (options)c ) {
            case options::filename: {
                g_opts.filename = optarg;
                break;
            }
            case options::dirname: {
                g_opts.dirname = optarg;
                break;
            }
            case options::xmin: {
                g_opts.x0 = std::strtod( optarg, nullptr );
                break;
            }
            case options::xmax: {
                g_opts.x1 = std::strtod( optarg, nullptr );
                break;
            }
            case options::ymin: {
                g_opts.y0 = std::strtod( optarg, nullptr );
                break;
            }
            case options::ymax: {
                g_opts.y1 = std::strtod( optarg, nullptr );
                break;
            }
            case options::zmin: {
                g_opts.z0 = std::strtod( optarg, nullptr );
                break;
            }
            case options::zmax: {
                g_opts.z1 = std::strtod( optarg, nullptr );
                break;
            }
            case options::nx: {
                g_opts.NX = std::atoi( optarg );
                break;
            }
            case options::ny: {
                g_opts.NY = std::atoi( optarg );
                break;
            }
            case options::nz: {
                g_opts.NZ = std::atoi( optarg );
                break;
            }
            case options::maxiter: {
                g_opts.maxiter = std::atoi( optarg );
                break;
            }
            case options::thresh: {
                g_opts.thresh = std::atoi( optarg );
                break;
            }
#ifdef HAVE_IMAGEMAGICK
            case options::asimage: {
                g_opts.asimage = true;
                break;
            }
#endif
            case options::netcdf: {
                g_opts.netcdf = true;
                break;
            }
            case options::binary: {
                g_opts.binary = true;
                break;
            }
            case options::progress: {
                g_opts.progress = std::atoi( optarg );
                break;
            }
            case options::bw: {
                g_opts.bw = true;
                break;
            }
            case options::help:
            default: {
                showHelpAndExit();
            }
        }
    }

    if ( g_opts.filename == "" ) {
        std::cerr << "--filename option is required\n";
        showHelpAndExit();
    }

    double rx = g_opts.x1 - g_opts.x0;
    double ry = g_opts.y1 - g_opts.y0;
    double rz = g_opts.z1 - g_opts.z0;
    if ( !g_opts.NY ) {
        g_opts.NY = int(g_opts.NX * (ry/rx));
    }
    if ( rz == 0 ) {
        if ( !g_opts.NZ ) {
            g_opts.NZ = 1;
        }
    } else {
        if ( !g_opts.NZ ) {
            g_opts.NZ = int(g_opts.NX * (rz/rx));
        }
    }

    double dx = rx/(g_opts.NX-1);
    double dy = ry/(g_opts.NY-1);
    double dz{};
    if ( g_opts.NZ > 1 ) {
        dz = rz/(g_opts.NZ-1);
    }

    int zs = ( g_opts.netcdf ) ? g_opts.NZ : 1;
    int * iterations = (int *)malloc(sizeof(int)*(g_opts.NX * g_opts.NY * zs));
    assert( iterations );
    IOstate io;

    int k = 0;
    double z = g_opts.z0;

    for ( ; k < g_opts.NZ ; z += dz, k++ ) {
        int kp = ( g_opts.netcdf ) ? k : 0;

        if ( g_opts.progress && ((k % g_opts.progress) == 0) ) {
            printf("%u/%u\n", k, g_opts.NZ);
        }

        computeplane( iterations, kp, z, dx, dy );

        if ( !g_opts.netcdf ) {
            io.writeit( iterations, k, z, dx, dy, dz );
        }
    }
    if ( g_opts.netcdf ) {
        io.writeit( iterations, 0, 0, dx, dy, dz );
    }

    return 0;
}

IOstate::IOstate( ) {
    if ( g_opts.netcdf ) {
#ifdef HAVE_IMAGEMAGICK
    } else if ( g_opts.asimage ) {
        // https://legacy.imagemagick.org/discourse-server/viewtopic.php?t=9581
        Magick::InitializeMagick( "" );
#endif
    } else {
        fp = fopen( g_opts.filename.c_str(), "wb" );
        if ( !fp ) {
            std::cerr << "failed to open: '" << g_opts.filename.c_str() << "'\n";
            std::exit(3);
        }
    }
}

void IOstate::writeimage( int * iterations, int k, double z, double dx, double dy, double dz ) {
#ifdef HAVE_IMAGEMAGICK
    RGB pix[g_opts.NX*g_opts.NY];

    for ( int i = 0 ; i < g_opts.NX * g_opts.NY ; i++ ) {
        int n = iterations[i];
        double color = ((double)n)/g_opts.maxiter;

        if ( g_opts.bw ) {
            blackorwhite tone = ( n < g_opts.maxiter ) ? blackorwhite::white : blackorwhite::black;

            pix[i] = RGB{tone};
        } else {
            pix[i] = mixer( 0, 1.0, color );
        }
    }

    std::string name = g_opts.dirname;

    if ( dz ) {
        name += std::to_string(k) + ".";
    }
    name += g_opts.filename;

    Magick::Image image{};

    // Create Image object and read in from pixel data above
    image.read( g_opts.NX, g_opts.NY, "RGB", Magick::CharPixel, (unsigned char *)pix );

    image.write( name.c_str() );
#endif
}

void IOstate::writefile( int * iterations, int k, double z, double dx, double dy, double dz ) {
    double a[3]{0,0,z};
    int NA = 2;
    if ( g_opts.NZ > 1 ) {
        NA++;
    }

    int i = 0;
    double x = g_opts.x0;
    for ( ; i < g_opts.NX; x += dx, i++ ) {
        a[0] = x;
        int j = 0;
        double y = g_opts.y0;
        for ( ; j < g_opts.NY; y += dy, j++ ) {
            a[1] = x;
            int n = iterations[ j * g_opts.NX + i ];

            double color = ((double)n)/g_opts.maxiter;

            if ( g_opts.binary ) {
                blackorwhite tone = ( n < g_opts.maxiter ) ? blackorwhite::white : blackorwhite::black;

                // --binary implies --bw: just the interior points:
                if ( tone != blackorwhite::white ) {
                    fwrite( a, NA, sizeof(double), fp );
                }
            }
            else {
                if ( g_opts.NZ > 1 ) {
                    fprintf( fp, "%g %g %g %g\n", x, y, z, color );
                } else {
                    fprintf( fp, "%g %g %g\n", x, y, color );
                }
            }
        }
    }
}

void IOstate::writecdf( int * a ) {
    try {
        NcFile dataFile( g_opts.filename, NcFile::replace ) ;

        size_t nx = g_opts.NX;
        size_t ny = g_opts.NY;
        size_t nz = g_opts.NZ;

        // Create the dimensions.
        auto xDim = dataFile.addDim( "x", nx ) ;
        auto yDim = dataFile.addDim( "y", ny ) ;
        auto zDim = dataFile.addDim( "z", nz ) ;

        std::vector < NcDim >   dims{ xDim, yDim, zDim };

        // Create the data variable.
        auto data = dataFile.addVar( "data", ncInt, dims ) ;

        std::vector<size_t>     startp  { 0, 0, 0 };
        std::vector<size_t>     countp  { nx, ny, nz };
        std::vector<ptrdiff_t>  stridep { 1, 1, 1 };
        std::vector<ptrdiff_t>  imapp   { 1, (ptrdiff_t)nx, (ptrdiff_t)(nx * ny) };

        data.putVar( startp, countp, stridep, imapp, a ) ;

        //dataFile.putAtt( "Version info:", "blah" ) ;
    } catch ( exceptions::NcException & e ) {
        std::cout << "netcdf creation failed:" << e.what() << "\n";

        std::exit(3);
    }
}

void IOstate::writeit( int * iterations, int k, double z, double dx, double dy, double dz ) {

    if ( g_opts.asimage ) {
        writeimage( iterations, k, z, dx, dy, dz );
    } else if ( g_opts.netcdf ) {
        writecdf( iterations );
    } else {
        writefile( iterations, k, z, dx, dy, dz );
    }
}
