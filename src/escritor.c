/*
The MIT License (MIT)

Copyright (c) 2016 Lorhan Sohaky

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <escritor.h>

int main( int argc, char *argv[] ) {
    // Verify if have a arguments
    if( argc < 3 ) {
        fprintf( stderr,
                 "\n\tERROR!\nInform the name of log file and the name of file to output the "
                 "subtitle.\nExample:\n\t%s logfile.xml output.txt\n",
                 argv[0] );
        fflush( stderr );
        return 0;
    }

    FILE *input_file, *output_file;
    _subtitle sub;
    init__subtitle( &sub );
    char string[STRING_SIZE];
    input_file = fopen( argv[1], "r" );
    output_file = fopen( argv[2], "w" );

    // Verify if can open the files
    if( input_file == NULL || output_file == NULL ) {
        if( input_file ) {
            fclose( input_file );
        }

        if( output_file ) {
            fclose( output_file );
        }
        printf( "\nERROR! Could open the files.\n" );
        return 0;
    }

    // Reads the input file and write the subtitles to output file
    do {
        sub = search_tags_in_file( input_file );
        if( is__subtitle_OK( &sub ) ) {
            write_in_file( input_file, output_file, sub );
        }
    } while( !feof( input_file ) );

    fclose( input_file );
    fclose( output_file );
    return 0;
}

_subtitle search_tags_in_file( FILE *file ) {
    static char string[STRING_SIZE];
    int count = 0;
    _subtitle sub;
    _position tmp;
    init__position( &tmp );
    init__subtitle( &sub );
    do {
        if( fgets( string, sizeof( char ) * STRING_SIZE, file ) == NULL ) {
            break;
        }
        tmp = search_open_and_close_tag_in_string( file, string, OPEN_DATE, CLOSE_DATE );
        set__position( &tmp, &sub.date );
        tmp = search_open_and_close_tag_in_string( file, string, OPEN_MSG, CLOSE_MSG );
        set__position( &tmp, &sub.msg );
        if( string[strlen( string ) - 1] != '\n' ) { // To solve a error when a tag is fragmented
            jump_to_position( file, -1, SEEK_CUR );
        }

    } while( !is__subtitle_OK( &sub ) );
    return sub;
}

_position
search_open_and_close_tag_in_string( FILE *file, char *string, char *open_tag, char *close_tag ) {
    int tmp_position;
    _position pos;
    init__position( &pos );
    tmp_position = get_current_position_in_file( file );
    pos.open_tag = search_tag_in_string( open_tag, string );
    if( is_valid_position( pos.open_tag ) ) {
        jump_to_position( file, pos.open_tag - strlen( string ), SEEK_CUR );
        pos.open_tag = get_current_position_in_file( file );
        jump_to_position( file, tmp_position, SEEK_SET );
    }
    pos.close_tag = search_tag_in_string( close_tag, string );
    if( is_valid_position( pos.close_tag ) ) {
        jump_to_position( file, pos.close_tag - strlen( string ), SEEK_CUR );
        pos.close_tag = get_current_position_in_file( file );
        jump_to_position( file, tmp_position, SEEK_SET );
    }
    return pos;
}

int search_tag_in_string( char *tag, char *string ) {
    char *position = strstr( string, tag );
    if( position == NULL ) {
        return -1;
    }
    return calculate_index( string, position );
}

void write_in_file( FILE *input_file, FILE *output_file, _subtitle sub ) {
    char string[STRING_SIZE];
    char *end_of_tag;

    jump_to_end_of_tag( input_file, sub.date.open_tag, OPEN_DATE );
    fgets( string, sizeof( char ) * STRING_SIZE, input_file );
    prepare_date_to_write( string, sub.date );
    time_t date = string_to_timet( string );
    write_date_in_file( output_file, date );

    jump_to_end_of_tag( input_file, sub.msg.open_tag, OPEN_MSG );
    do {
        fgets( string, sizeof( char ) * STRING_SIZE, input_file );
        replace_break_line_into_space( string );
        if( get_current_position_in_file( input_file ) >= sub.msg.close_tag ) {
            sub.msg.open_tag = get_the_new_position( input_file, string );
            prepare_msg_to_write( string, sub.msg );
            write_msg_in_file( output_file, string );
            write_break_line( output_file );
        } else {
            write_msg_in_file( output_file, string );
        }
    } while( get_current_position_in_file( input_file ) < sub.msg.close_tag );
}

void init__subtitle( _subtitle *sub ) {
    init__position( &sub->date );
    init__position( &sub->msg );
}

void init__position( _position *pos ) {
    pos->open_tag = -1;
    pos->close_tag = -1;
}

void set__position( _position *from, _position *to ) {
    if( from->open_tag != -1 ) {
        to->open_tag = from->open_tag;
    }
    if( from->close_tag != -1 ) {
        to->close_tag = from->close_tag;
    }
}

bool is_valid_position( int position ) {
    if( position == -1 ) {
        return false;
    }
    return true;
}

bool is__subtitle_OK( _subtitle *sub ) {
    if( is__position_OK( &sub->date ) && is__position_OK( &sub->msg ) ) {
        return true;
    }
    return false;
}

bool is__position_OK( _position *pos ) {
    if( pos->open_tag != -1 && pos->close_tag != -1 ) {
        return true;
    }
    return false;
}

int calculate_index( char *initial_position, char *position ) {
    return position - initial_position;
}

int search_limit_in_string( char *string ) {
    if( strchr( string, '.' ) != NULL ) { // Subversion
        return calculate_index( string, strchr( string, '.' ) );
    } else { // Git
        if( strrchr( string, '+' ) != NULL ) {
            return calculate_index( string, strrchr( string, '+' ) );
        } else if( strrchr( string, '-' ) != NULL ) {
            return calculate_index( string, strrchr( string, '-' ) );
        }
    }
}

void jump_to_end_of_tag( FILE *file, int position, char *tag ) {
    jump_to_position( file, position + strlen( tag ), SEEK_SET );
}

void jump_to_position( FILE *file, int position, int origin ) {
    fseek( file, position, origin );
}

int get_current_position_in_file( FILE *file ) {
    return ftell( file );
}

int get_the_new_position( FILE *file, char *string ) {
    int position = get_current_position_in_file( file );
    jump_to_position( file, -strlen( string ), SEEK_CUR );
    int initial_position = get_current_position_in_file( file );
    jump_to_position( file, position, SEEK_SET );
    return initial_position;
}

void replace_break_line_into_space( char *string ) {
    int position = strlen( string ) - 1;
    if( string[position] == '\n' ) {
        string[position] = ' ';
    }
}

void prepare_date_to_write( char *str_date,
                            _position pos ) { // Force a limit to the length of the date string, to
                                              // don't write the </ date> tag
    int end_of_tag = pos.close_tag - pos.open_tag + strlen( OPEN_DATE );
    str_date[end_of_tag] = '\0';
    end_of_tag = search_limit_in_string( str_date );
    str_date[end_of_tag] = '\0';
}

void prepare_msg_to_write(
    char *str_msg,
    _position pos ) { // Force a limit to msg string, to don't write the tag </msg>
    int end_of_tag = pos.close_tag - pos.open_tag;
    str_msg[end_of_tag] = '\0';
}

void write_break_line( FILE *file ) {
    fprintf( file, "\n" );
}

void write_date_in_file( FILE *file, time_t date ) {
    fprintf( file, "%ld|", date );
}

void write_msg_in_file( FILE *file, char *msg ) {
    fprintf( file, "%s", msg );
}

time_t string_to_timet( const char *string ) {
    printf( "%s\n", string );
    int year, month, day, hour, min, sec;
    struct tm data;
    sscanf( string, "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &min, &sec );
    data.tm_year = year - 1900;
    data.tm_mon = month - 1;
    data.tm_mday = day;
    data.tm_hour = hour;
    data.tm_min = min;
    data.tm_sec = sec;
    return mktime( &data );
}
