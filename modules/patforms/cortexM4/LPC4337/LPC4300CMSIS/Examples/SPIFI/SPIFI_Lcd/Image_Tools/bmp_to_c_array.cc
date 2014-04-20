#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <memory>
#include <algorithm>

#define DFLT_C_HDR_FILENAME "graphic_resources.h"
#define DLFT_C_SRC_FILENAME "graphic_resources.c"
#define DFLT_PALETTE_NAME   "master_palette"
#define NUM_PALETTE_COLORS  256

using namespace std;

typedef struct
{
    unsigned char blue,
                  green,
                  red;
} color_24_bit_t;

typedef struct
{
    color_24_bit_t palette[NUM_PALETTE_COLORS];
    string name,
           path;
    int width,
        height,
        actual_width,
        num_colors,
        flt_specified,
        slt_specified;
    unsigned char *data;
} bitmap_t;

void fatal_error(char *err, bool initial_newline)
{
    if (initial_newline)
        cout << endl;

    if (err)
        cout << "Error: " << err << endl << endl;
    cout << "Aborting..." << endl << endl;

    exit(EXIT_FAILURE);
}

void validate_tiling(int flt, int slt)
{
    if (slt != 0)
    {
        if (flt == 0)
            fatal_error("Second-level tiling requires first-level tiling.", false);
        if (slt >= flt)
            fatal_error("Second-level tiling value must be less than first-level tiling value.", false);
        if ((flt % slt) != 0)
            fatal_error("First-level tiling value must be divisible by second-level tiling value.", false);
    }
}

int main(int argc, char **argv)
{
    vector<bitmap_t> bitmaps;
    vector<bitmap_t>::iterator bitmaps_iterator;
    color_24_bit_t joint_palette[NUM_PALETTE_COLORS] = {{0}};
    ifstream bmp;
    ofstream c_hdr,
             c_src;
    string c_hdr_filename = DFLT_C_HDR_FILENAME,
           c_src_filename = DLFT_C_SRC_FILENAME,
           palette_name =   DFLT_PALETTE_NAME,
           c_hdr_name,
           uppercase_name,
           err;
    string::iterator string_iterator;
    bitmap_t bitmap;
    color_24_bit_t color_24_bit;
    char bitmap_id[2];
    int first_file_arg_index,
        flt_current, //first-level tiling value
        slt_current, //second-level tiling value
        bitmap_data_offset,
        bitmap_compression,
        bitmap_num_unique_colors,
        joint_num_unique_colors,
        palette_index,
        num_chrs_read,
        column,
        i, j, k, l;
    short bitmap_color_depth,
          transparent_color,
          color_15_bit;
    bool color_present_in_palette,
         transparency_specified,
         gen_transparent_color,
         transparent_color_present_in_bitmaps,
         unique_colors,
         abort;
    unsigned char *p_buff0,
                  *p_buff1,
                  *p_buff_ref;
    char *p_chr,
         *p_chr2;

    cout << endl << "------------------" << endl <<
                    "| bmp_to_c_array |" << endl <<
                    "------------------" << endl << endl;
    if (argc == 1)
    {
        cout << "Usage: bmp_to_c_array [-pal PALETTE_NAME] [-hdr HEADER_FILENAME]" << endl <<
                "       [-src SOURCE_FILENAME] [-flt FIRST_LEVEL_TILING_VALUE]" << endl <<
                "       [-slt SECOND_LEVEL_TILING_VALUE] [-trans_col TRANSPARENT_COLOR|gen] BITMAP1" << endl <<
                "       [[-flt FIRST_LEVEL_TILING_VALUE] [-slt SECOND_LEVEL_TILING_VALUE] BITMAP2" << endl <<
                "        ..." << endl <<
                "       ]" << endl << endl;

        exit(EXIT_SUCCESS);
    }
    else
        cout << "Note: Execute without any arguments for usage details." << endl << endl;

    first_file_arg_index = 1;
    flt_current = 0;
    slt_current = 0;
    transparency_specified = false;
    gen_transparent_color = false;
    transparent_color_present_in_bitmaps = false;
    abort = false;

    for (i = 1; (i < (argc - 1)) && (*(argv[i]) == '-'); i += 2)
    {
        if (strcmp(argv[i], "-pal") == 0)
            palette_name = argv[i + 1];
        else if (strcmp(argv[i], "-hdr") == 0)
            c_hdr_filename = argv[i + 1];
        else if (strcmp(argv[i], "-src") == 0)
            c_src_filename = argv[i + 1];
        else if (strcmp(argv[i], "-flt") == 0)
        {
            flt_current = strtol(argv[i + 1], NULL, 10);
            if (flt_current == 0)
                fatal_error("Numerical/positive flt values only.", false);
        }
        else if (strcmp(argv[i], "-slt") == 0)
        {
            slt_current = strtol(argv[i + 1], NULL, 10);
            if (slt_current == 0)
                fatal_error("Numerical/positive slt values only.", false);
        }
        else if (strcmp(argv[i], "-trans_col") == 0)
        {
            transparency_specified = true;
            if (strlen(argv[i + 1]) != 4)
            {
                if (strcmp(argv[i + 1], "gen") == 0)
                    gen_transparent_color = true;
                else
                    abort = true;
            }
            else
                for (j = 0; j < (int)(strlen(argv[i + 1])); ++j)
                {
                    if (((*(argv[i + 1] + j) < 48) || (*(argv[i + 1] + j) > 57)) &&
                        ((*(argv[i + 1] + j) < 65) || (*(argv[i + 1] + j) > 70)) &&
                        ((*(argv[i + 1] + j) < 97) || (*(argv[i + 1] + j) > 102)))
                        abort = true;
                }
            if (abort)
                fatal_error("Transparent color (15-bit) needs to be specified in hexadecimal 0x0000 BGR format (minus the 0x preface).", false);
            if (!gen_transparent_color)
            {
                transparent_color = strtol(argv[i + 1], NULL, 16);
                joint_palette[0].blue = (unsigned char)(transparent_color >> 10);
                joint_palette[0].green = (unsigned char)((transparent_color << 22) >> 27);
                joint_palette[0].red = (unsigned char)((transparent_color << 27) >> 27);
            }
        }
    }
    first_file_arg_index = i;

    if (i == argc)
        fatal_error("No images specified for conversion.", false);

    validate_tiling(flt_current, slt_current);

    cout << "Palette name: '" << palette_name << "'" << endl <<
            "Header filename: '" << c_hdr_filename << "'" << endl <<
            "Source filename: '" << c_src_filename << "'" << endl << endl;

    for (i = first_file_arg_index; i < argc; ++i)
    {
        if (i < (argc - 1))
            if (strcmp(argv[i], "-flt") == 0)
            {
                flt_current = strtol(argv[i + 1], NULL, 10);
                if (flt_current == 0)
                    fatal_error("Numerical/positive flt values only.", false);
                ++i;
                continue;
            }
            else if (strcmp(argv[i], "-slt") == 0)
            {
                slt_current = strtol(argv[i + 1], NULL, 10);
                if (slt_current == 0)
                    fatal_error("Numerical/positive slt values only.", false);
                ++i;
                continue;
            }

        validate_tiling(flt_current, slt_current);
        bitmap.path = argv[i];
        bitmap.flt_specified = flt_current;
        bitmap.slt_specified = slt_current;

        bmp.open(argv[i], ios::in | ios::binary);
        if (!bmp.is_open())
        {
            cout << "Error: '" << argv[i] << "' could not be opened." << endl;
            abort = true;
            bmp.clear();
            continue;
        }
        bmp.read(bitmap_id, 2);
        if (memcmp(bitmap_id, "BM", 2) != 0)
        {
            cout << "Error: '" << argv[i] << "' is not a valid bmp file." << endl;
            bmp.close();
            abort = true;
            continue;
        }
        bmp.seekg(18);
        bmp.read((char *)&bitmap.width, 4);
        bmp.seekg(22);
        bmp.read((char *)&bitmap.height, 4);

        if (flt_current != 0)
        {
            if ((bitmap.width == 0) || ((bitmap.width % flt_current) != 0) ||
                (bitmap.height == 0) || ((bitmap.height % flt_current) != 0))
            {
                cout << "Error: '" << argv[i] << "' (" << bitmap.width << "x" << bitmap.height << ") has a width" << endl <<
                        "       and/or height indivisible by specified flt value." << endl;
                bmp.close();
                abort = true;
                continue;
            }
        }

        //calculate actual width (row data is always a multiple of four bytes in bmp format and is zero-padded to meet this req.)
        bitmap.actual_width = bitmap.width;
        while ((bitmap.actual_width % 4) != 0)
            ++bitmap.actual_width;

        bmp.seekg(28);
        bmp.read((char *)&bitmap_color_depth, 2);
        if (bitmap_color_depth != 8)
        {
            cout << "Error: '" << argv[i] << "' is not an 8-bits-per-pixel image." << endl;
            bmp.close();
            abort = true;
            continue;
        }
        bmp.seekg(30);
        bmp.read((char *)&bitmap_compression, 4);
        if (bitmap_compression != 0)
        {
            cout << "Error: '" << argv[i] << "' is a compressed image." << endl;
            bmp.close();
            abort = true;
            continue;
        }

        p_chr = strstr(argv[i], ".bmp");
        if (!p_chr)
            p_chr = strstr(argv[i], ".BMP");
        p_chr2 = strrchr(argv[i], '/');
        if (!p_chr2)
            p_chr2 = strrchr(argv[i], '\\');
        if (p_chr2)
            ++p_chr2;
        else
            p_chr2 = argv[i];
        bitmap.name = "";
        if (p_chr)
        {
            j = p_chr - p_chr2;
            for (k = 0; k < j; ++k)
                bitmap.name += *(p_chr2 + k);
        }
        else
            bitmap.name = p_chr2;

        bitmap.data = (unsigned char *)(malloc(bitmap.actual_width * bitmap.height));
        if (!bitmap.data)
            fatal_error("Could not allocate memory for image data.", abort);

        bmp.seekg(54);
        for (j = 0; j < NUM_PALETTE_COLORS; ++j)
        {
            bmp.read((char *)&bitmap.palette[j], 3);
            bmp.seekg(1, ios::cur);
        }
        bmp.seekg(10);
        bmp.read((char *)&bitmap_data_offset, 4);
        bmp.seekg(bitmap_data_offset);
        bmp.read((char *)bitmap.data, bitmap.actual_width * bitmap.height);

        bmp.close();
        bitmaps.push_back(bitmap);
    }
    if (abort)
    {
        bitmaps.clear();
        fatal_error(NULL, true);
    }

    c_hdr.open(c_hdr_filename.c_str(), ios::out | ios::trunc);
    if (!c_hdr.is_open())
    {
        err = "Could not open/create '";
        err += c_hdr_filename;
        err += "'.";
        fatal_error((char *)(err.c_str()), false);
    }
    c_src.open(c_src_filename.c_str(), ios::out | ios::trunc);
    if (!c_src.is_open())
    {
        err = "Could not open/create '";
        err += c_src_filename;
        err += "'.";
        fatal_error((char *)(err.c_str()), false);
    }

    c_hdr_name = c_hdr_filename;
    for (string_iterator = c_hdr_name.begin(); string_iterator != c_hdr_name.end(); ++string_iterator)
    {
        if (*string_iterator == '.')
            *string_iterator = '_';
    }
    transform(c_hdr_name.begin(), c_hdr_name.end(), c_hdr_name.begin(), (int(*)(int))toupper);

    c_hdr << "#ifndef " << c_hdr_name << endl;
    c_hdr << "#define " << c_hdr_name << endl << endl;

    c_hdr << "/* Background/sprite/palette resources declared in this file:" << endl;
    c_src << "/* Background/sprite/palette resources defined/encoded in this file:" << endl;
    for (bitmaps_iterator = bitmaps.begin(); bitmaps_iterator != bitmaps.end(); ++bitmaps_iterator)
    {
        c_hdr << " *    '" << bitmaps_iterator->name << "'" << endl;
        c_src << " *    '" << bitmaps_iterator->name << "'" << endl;
    }
    c_hdr << " *    '" << palette_name << "'" << endl;
    c_src << " *    '" << palette_name << "'" << endl;
    c_hdr << " *" << endl <<
             " * Associated palette: '" << palette_name << "'" << endl <<
             " *" << endl <<
             " * Generated w/ bmp_to_c_array via:" << endl <<
             " *    \"";
    c_hdr << "bmp_to_c_array ";
    for (i = 1; i < argc; ++i)
    {
        c_hdr << argv[i];
        if (i != (argc - 1))
            c_hdr << " ";
    }
    c_hdr << "\"" << endl << " */" << endl << endl;
    c_src << " *" << endl <<
             " * Associated palette: '" << palette_name << "'" << endl <<
             " *" << endl <<
             " * Generated w/ bmp_to_c_array via:" << endl <<
             " *    \"";
    c_src << "bmp_to_c_array ";
    for (i = 1; i < argc; ++i)
    {
        c_src << argv[i];
        if (i != (argc - 1))
            c_src << " ";
    }
    c_src << "\"" << endl << " */" << endl << endl;

    for (bitmaps_iterator = bitmaps.begin(); bitmaps_iterator != bitmaps.end(); ++bitmaps_iterator)
    {
        uppercase_name = bitmaps_iterator->name;
        transform(uppercase_name.begin(), uppercase_name.end(), uppercase_name.begin(), (int(*)(int))toupper);
        c_hdr << "#define " << uppercase_name << "_SIZE " << (bitmaps_iterator->width * bitmaps_iterator->height) << endl;
    }
    c_hdr << endl;

    for (bitmaps_iterator = bitmaps.begin(); bitmaps_iterator != bitmaps.end(); ++bitmaps_iterator)
        c_hdr << "extern const unsigned char " << bitmaps_iterator->name << "[];" << endl;
    c_hdr << "extern const unsigned short " << palette_name << "[];" << endl << endl;

    c_hdr << "#endif /* " << c_hdr_name << " */" << endl;
    c_hdr.close();

    if (transparency_specified)
        joint_num_unique_colors = 1;
    else
        joint_num_unique_colors = 0;
    for (bitmaps_iterator = bitmaps.begin(); bitmaps_iterator != bitmaps.end(); ++bitmaps_iterator)
    {
        bitmap_num_unique_colors = 0;
        column = 0;

        cout << "'" << bitmaps_iterator->path << "'" << endl << endl;
        cout << "\tWidth: " << bitmaps_iterator->width << endl;
        cout << "\tHeight: " << bitmaps_iterator->height << endl;
        c_src << "const unsigned char " << bitmaps_iterator->name << "[" << (bitmaps_iterator->width * bitmaps_iterator->height) << "] =" << endl <<
                 "{" << endl;

        p_buff0 = (unsigned char *)(malloc(bitmaps_iterator->width * bitmaps_iterator->height));
        if (!p_buff0)
            fatal_error("Could not allocate memory for first buffer.", true);
        if (bitmaps_iterator->flt_specified != 0)
        {
            p_buff1 = (unsigned char *)(malloc(bitmaps_iterator->width * bitmaps_iterator->height));
            if (!p_buff1)
                fatal_error("Could not allocate memory for second buffer.", true);
        }

        for (i = 0; i < bitmaps_iterator->height; ++i)
        {
            for (j = 0; j < bitmaps_iterator->width; ++j)
            {
                color_present_in_palette = false;
                palette_index = *(bitmaps_iterator->data + ((i * bitmaps_iterator->actual_width) + j));
                color_24_bit = bitmaps_iterator->palette[palette_index];
                if (gen_transparent_color)
                    k = 1;
                else
                    k = 0;
                for (; k < joint_num_unique_colors; k++)
                {
                    if (((color_24_bit.blue >> 3) == (joint_palette[k].blue >> 3)) &&
                        ((color_24_bit.green >> 3) == (joint_palette[k].green >> 3)) &&
                        ((color_24_bit.red >> 3) == (joint_palette[k].red >> 3)))
                    {
                        color_present_in_palette = true;

                        //remember that rows are inverted in a bmp file
                        *(p_buff0 + (bitmaps_iterator->width * (bitmaps_iterator->height - i - 1)) + j) = k;
                        break;
                    }
                }
                if (!color_present_in_palette)
                {
                    if (joint_num_unique_colors > (NUM_PALETTE_COLORS - 1))
                        fatal_error("Color palette full (there are already 256 unique colors in the palette).", true);
                    joint_palette[joint_num_unique_colors] = color_24_bit;
                    *(p_buff0 + (bitmaps_iterator->width * (bitmaps_iterator->height - i - 1)) + j) = joint_num_unique_colors;
                    ++bitmap_num_unique_colors;
                    ++joint_num_unique_colors;
                }
                if (!transparent_color_present_in_bitmaps)
                {
                    if ((color_24_bit.blue == joint_palette[0].blue) &&
                        (color_24_bit.green == joint_palette[0].green) &&
                        (color_24_bit.red == joint_palette[0].red))
                    {
                        transparent_color_present_in_bitmaps = true;
                    }
                }
            }
        }
        p_buff_ref = p_buff0;

        if (bitmaps_iterator->flt_specified != 0)
        {
            num_chrs_read = 0;
            for (i = 0; i < bitmaps_iterator->height; i += bitmaps_iterator->flt_specified)
            {
                for (j = 0; j < bitmaps_iterator->width; j += bitmaps_iterator->flt_specified)
                {
                    p_buff_ref = p_buff0 + ((i * bitmaps_iterator->width) + j);
                    for (k = 0; k < bitmaps_iterator->flt_specified; ++k)
                    {
                        for (l = 0; l < bitmaps_iterator->flt_specified; ++l)
                        {
                            *(p_buff1 + num_chrs_read) = *(p_buff_ref + ((k * bitmaps_iterator->width) + l));
                            ++num_chrs_read;
                        }
                    }
                }
            }
            p_buff_ref = p_buff1;
        }

        if (bitmaps_iterator->slt_specified != 0)
        {
            num_chrs_read = 0;
            for (i = 0; i < ((bitmaps_iterator->width * bitmaps_iterator->height) / bitmaps_iterator->flt_specified); i += bitmaps_iterator->slt_specified)
            {
                for (j = 0; j < bitmaps_iterator->flt_specified; j += bitmaps_iterator->slt_specified)
                {
                    p_buff_ref = p_buff1 + ((i * bitmaps_iterator->flt_specified) + j);
                    for (k = 0; k < bitmaps_iterator->slt_specified; ++k)
                    {
                        for (l = 0; l < bitmaps_iterator->slt_specified; ++l)
                        {
                            *(p_buff0 + num_chrs_read) = *(p_buff_ref + ((k * bitmaps_iterator->flt_specified) + l));
                            ++num_chrs_read;
                        }
                    }
                }
            }
            p_buff_ref = p_buff0;
        }

        for (j = 0; j < (bitmaps_iterator->width * bitmaps_iterator->height); ++j)
        {
            c_src << setw(3) << (unsigned int)*(p_buff_ref + j);
            if (j == ((bitmaps_iterator->width * bitmaps_iterator->height) - 1))
                c_src << endl << "};" << endl << endl;
            else
            {
                c_src << ",";
                ++column;
                if (column != 16)
                    c_src << " ";
                else
                {
                    c_src << endl;
                    column = 0;
                }
            }
        }

        cout << "\tFirst-level tiling value used: ";
        if (bitmaps_iterator->flt_specified != 0)
            cout << bitmaps_iterator->flt_specified << endl;
        else
            cout << "n/a" << endl;
        cout << "\tSecond-level tiling value used: ";
        if (bitmaps_iterator->slt_specified != 0)
            cout << bitmaps_iterator->slt_specified << endl << endl;
        else
            cout << "n/a" << endl << endl;
        cout << "\tNumber of unique colors (in the combined set of colors" << endl <<
                "\tfrom all images processed thus far): " << bitmap_num_unique_colors << endl << endl;

        free(p_buff0);
        if (bitmaps_iterator->flt_specified != 0)
            free(p_buff1);
    }

    if (transparency_specified)
    {
        if (gen_transparent_color)
        {
            srand(time(NULL));
            do
            {
                transparent_color = rand() % 32768;
                unique_colors = true;
                for (i = 0; i < NUM_PALETTE_COLORS; ++i)
                {
                    color_15_bit = ((joint_palette[i].blue >> 3) << 10) +
                                   ((joint_palette[i].green >> 3) << 5) +
                                   (joint_palette[i].red >> 3);
                    if (transparent_color == color_15_bit)
                    {
                        unique_colors = false;
                        break;
                    }
                }
            } while (!unique_colors);
            joint_palette[0].blue = (unsigned char)(transparent_color >> 10);
            joint_palette[0].green = (unsigned char)((transparent_color << 22) >> 27);
            joint_palette[0].red = (unsigned char)((transparent_color << 27) >> 27);
        }
        else if (!transparent_color_present_in_bitmaps)
            fatal_error("Transparent color specified was not present in any of the bitmaps processed.", false);
    }

    c_src << "const unsigned short " << palette_name << "[256] =" << endl <<
             "{" << endl << endl <<
             "/* Free palette entries (colors) remaining: " << (NUM_PALETTE_COLORS - joint_num_unique_colors) << endl <<
             " * Note that palette colors have been converted from 24 bits to 15 bits each." << endl <<
             " *" << endl <<
             " *        R    G    B" << endl <<
             " *      -------------*/" << endl;
    for (i = 0; i < NUM_PALETTE_COLORS; ++i)
    {
        color_15_bit = ((joint_palette[i].blue >> 3) << 10) +
                       ((joint_palette[i].green >> 3) << 5) +
                       (joint_palette[i].red >> 3);
        c_src << "/* " << setw(3) << right << i << ") " <<
                 setw(3) << (unsigned int)joint_palette[i].red << ", " <<
                 setw(3) << (unsigned int)joint_palette[i].green << ", " <<
                 setw(3) << (unsigned int)joint_palette[i].blue << " -> */ 0x" << hex << setfill('0') <<
                 setw(4) << color_15_bit << dec << setfill(' ');
        if (i < (NUM_PALETTE_COLORS - 1))
            c_src << "," << endl;
        else
            c_src << endl;
    }
    c_src << "};" << endl;
    c_src.close();

    for (bitmaps_iterator = bitmaps.begin(); bitmaps_iterator != bitmaps.end(); ++bitmaps_iterator)
        free(bitmaps_iterator->data);

    bitmaps.clear();

    return 0;
}
