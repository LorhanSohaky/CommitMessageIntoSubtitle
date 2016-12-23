## Commit Message Into Subtitle

###Why I did this?
Although Gource makes it easy to view all of the modifications that have run in a project, within a version control, without caption the task is still complicated. That is why I created this program, that it take the texts of *commits*.

### LICENSE:
**MIT**
[Read the license](LICENSE)

### COMPILER USED:
***gcc (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0***

### OBJECTIVE:
Create an algorithm that takes the *commits* texts and generates a subtitle file recognized by [Gource](https://github.com/acaudwell/Gource).

### NOTES:
- This program takes the message from the *commits* and generates a caption file with the *commits* texts.
- It works only with valid XML files. It should be noted that only tests with LOG files provided by Subversion and Git.
- The program generates as output a file with the subtitles.
- The program looks for *tags* `<date>`, `</ date>`, `<msg>` and `</ msg>`.

### HOW TO USE:
 `int main( int argc, char const *argv[] )`:
  - `argv[1]`: name of the XML LOG file.
  -`argv[2]`: name of the file in which the subtitles should be written.

#### HOW TO COMPILE:
`$ make`

#### HO TO RUN:
`$ ./escritor.exe <log_file> <subtitle_file>`

### HOW TO GENERATE THE LOG FILES ACCEPTED BY THE PROGRAM:

**It serves only to generate the caption, not to generate the Gource.**

- Git: `git log --pretty=format:'<date>%cI</date>%n<msg>%b</msg>' > <output_file>`
- SVN: `svn log <repository> -r 1:HEAD --xml --verbose > <output_file>`
