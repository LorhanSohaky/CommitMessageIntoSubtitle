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

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define STRING_SIZE 3000

//Defines
static char *OPEN_MSG="<msg>";
static char *CLOSE_MSG="</msg>";
static char *OPEN_DATE="<date>";
static char *CLOSE_DATE="</date>";

typedef struct{
    int open_tag;
    int close_tag;
} _position;

typedef struct{
    _position date;
    _position msg;
} _subtitle;

_position search_open_and_close_tag_in_string(FILE *file,char *string, char *open_tag, char *close_tag);
int search_tag_in_string(char *tag, char *string);
int search_limit_in_string(char *string);
_subtitle search_tags_in_file(FILE *file);

void write_in_file(FILE *input_file,FILE *output_file, _subtitle sub);
void write_date_in_file(FILE *file, time_t date);
void write_msg_in_file(FILE *file, char *msg);
void write_break_line(FILE *file);

void prepare_date_to_write(char *str_date,_position pos);
void prepare_msg_to_write(char *str_msg, _position pos);

void replace_break_line_into_space( char *string);

void init__subtitle(_subtitle *sub);
void init__position(_position *pos);

bool is__position_OK(_position *pos);
bool is__subtitle_OK(_subtitle *sub);
bool is_valid_position(int position);

void set__position(_position *from, _position *to);

int calculate_index(char *initial_position,char *position);

void jump_to_end_of_tag(FILE *file,int position, char *tag);
void jump_to_position(FILE *file,int position,int origin);
int get_current_position_in_file(FILE *file);
int get_the_new_position(FILE *file, char *string);

time_t string_to_timet(const char *string);
