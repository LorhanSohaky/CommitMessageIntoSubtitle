## Commit Message Into Subtitle

### POR QUAL MOTIVO EU FIZ ISSO?
Embora o Gource facilite a visualização de todas as modificações que ocorreram em um projeto, dentro de um controle, sem legenda a tarefa ainda sim é um pouco complicada. Por isso criei esse programa, que ele pega os textos dos *commits*.

### LICENÇA:
**MIT**
[Leia a licença](LICENSE)

### COMPILADOR UTILIZADO:
***gcc (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0***

### OBJETIVO:
Criar um algoritmo que pegue os textos dos *commits* e gere um arquivo de legenda reconhecido pelo [Gource](https://github.com/acaudwell/Gource).

### NOTAS:
- Esse programa pega a mensagem dos *commits* e gera um arquivo de legenda com os textos dos *commits*.
- Ele funciona somente com arquivos XML válidos. Vale ressaltar que só foram feitos testes com arquivos de LOGs providos do Subversion e Git.
- O programa gera como saída um arquivo com as legendas.
- O programa procura pelas *tags* `<date>`,`</date>`,`<msg>` e `</msg>`.

#### COMO COMPILAR:
`$ make`

#### COMO EXECUTAR:
`$ ./escritor.exe <log_file> <subtitle_file>`

### COMO GERAR ARQUIVO DE LOG ACEITOS PELO PROGRAMA:

**Serve apenas para gerar a legenda, não para gerar o Gource.**

- Git: `git log --pretty=format:'<date>%cI</date>%n<msg>%B</msg>' --reverse > <output_file>`
- SVN: `svn log <repository> -r 1:HEAD --xml --verbose > <output_file>`
