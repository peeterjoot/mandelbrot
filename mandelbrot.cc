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
#include <Magick++.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <iostream>

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
    output,
    maxiter,
    thresh,
    asimage,
    binary,
    progress,
    help,
};

void showHelpAndExit() {
    std::cerr <<
             "usage: mandlebrot\n"
             "\t--output=path [--asimage | --binary]\n"
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
             "\t[--progress]\n"
             "\t--help]\n"
             "\t--debug]\n"
             "\n"
             "This program computex the mandelbrot set for a plane or range of planes.\n";

    std::exit( 1 );
}

struct RGB
{
    uint8_t c[3];
};

inline RGB mixer( double minimum, double maximum, double value ) {
// https://stackoverflow.com/a/20792531/189270
  auto ratio = 2 * (value - minimum) / (maximum - minimum);
  uint8_t r = (uint8_t) std::max(0.0, 255 * (ratio - 1));
  uint8_t b = (uint8_t) std::max(0.0, 255 * (1 - ratio));
  uint8_t g = 255 - b - r;
#if 1

  RGB rc{r, g, b};
#elif 1
  //RGB rc{b, b, b};
  RGB rc{g, g, g};
#elif 0
  // more like: https://www.codingame.com/playgrounds/2358/how-to-plot-the-mandelbrot-set/mandelbrot-set
  RGB rc{r, r, r};
#endif

  return rc;
}

int main( int argc, char ** argv )
{
    int c{ 0 };
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
    std::string output{};
    bool asimage{};
    bool binary{};
    bool progress{};

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
        { "output", 1, nullptr, (int)options::output },
        { "maxiter", 1, nullptr, (int)options::maxiter },
        { "thresh", 1, nullptr, (int)options::thresh },
        { "asimage", 0, nullptr, (int)options::asimage },
        { "binary", 0, nullptr, (int)options::binary },
        { "progress", 0, nullptr, (int)options::progress },
        { "help", 0, nullptr, (int)options::help },
        { nullptr, 0, nullptr, 0 }
    };

    while ( -1 !=
            ( c = getopt_long( argc, argv, "", longOptions, nullptr ) ) ) {
        switch ( (options)c ) {
            case options::output: {
                output = optarg;
                break;
            }
            case options::xmin: {
                x0 = std::strtod( optarg, nullptr );
                break;
            }
            case options::xmax: {
                x1 = std::strtod( optarg, nullptr );
                break;
            }
            case options::ymin: {
                y0 = std::strtod( optarg, nullptr );
                break;
            }
            case options::ymax: {
                y1 = std::strtod( optarg, nullptr );
                break;
            }
            case options::zmin: {
                z0 = std::strtod( optarg, nullptr );
                break;
            }
            case options::zmax: {
                z1 = std::strtod( optarg, nullptr );
                break;
            }
            case options::nx: {
                NX = std::atoi( optarg );
                break;
            }
            case options::ny: {
                NY = std::atoi( optarg );
                break;
            }
            case options::nz: {
                NZ = std::atoi( optarg );
                break;
            }
            case options::maxiter: {
                maxiter = std::atoi( optarg );
                break;
            }
            case options::thresh: {
                thresh = std::atoi( optarg );
                break;
            }
            case options::asimage: {
                asimage = true;
                break;
            }
            case options::binary: {
                binary = true;
                break;
            }
            case options::progress: {
                progress = true;
                break;
            }
            case options::help:
            default: {
                showHelpAndExit();
            }
        }
    }

    if ( output == "" ) {
        std::cerr << "--output option is required\n";
        showHelpAndExit();
    }

    double rx = x1 - x0;
    double ry = y1 - y0;
    double rz = z1 - z0;
    if ( !NY ) {
        NY = int(NX * (ry/rx));
    }
    if ( rz == 0 ) {
        if ( !NZ ) {
            NZ = 1;
        }
    } else {
        if ( !NZ ) {
            NZ = int(NX * (rz/rx));
        }
    }

    double dx = rx/(NX-1);
    double dy = ry/(NY-1);
    double dz{};
    if ( NZ > 1 ) {
        dz = rz/(NZ-1);
    }

    RGB pix[NX*NY];
    double a[3];

    FILE * fp{};
    if ( !asimage ) {
        fp = fopen( output.c_str(), "wb" );
        if ( !fp ) {
            std::cerr << "failed to open: '" << output.c_str() << "'\n";
            return 2;
        }
    }

    int NA = 2;
    if ( NZ > 1 ) {
        NA++;
    }

    int k = 0;
    double z = z0;
    for ( ; k < NZ ; z += dz, k++ ) {
        if ( progress && ((k % 30) == 0) ) {
            printf("%u/%u\n", k, NZ);
        }
        int i = 0;
        double x = x0;
        a[2] = z;
        for ( ; i < NX ; x += dx, i++ ) {
            int j = 0;
            double y = y0;
            a[0] = x;
            for ( ; j < NY ; y += dy, j++ ) {
                double n = f( x, y, z, maxiter, thresh );
                double color = n/maxiter;
                a[1] = y;

                if ( asimage ) {
                    pix[j * NX + i] = mixer( 0, 1.0, color );
                } else {
                    if ( binary ) {
                        if ( color == 1 ) {
                            fwrite( a, NA, sizeof(double), fp );
                        }
                    }
                    else {
                        if ( NZ > 1 ) {
                            fprintf( fp, "%g %g %g %g\n", x, y, z, color );
                        } else {
                            fprintf( fp, "%g %g %g\n", x, y, color );
                        }
                    }
                }
            }
        }
    }

    if ( asimage ) {
        // Initialise ImageMagick library
        Magick::InitializeMagick( "" ); // https://legacy.imagemagick.org/discourse-server/viewtopic.php?t=9581

        // Create Image object and read in from pixel data above
        Magick::Image image;
        image.read( NX, NY, "RGB", Magick::CharPixel, (unsigned char *)pix );

        image.write( output.c_str() );
    }

    return 0;
}
