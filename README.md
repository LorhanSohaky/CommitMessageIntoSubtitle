## Commit Message Into Subtitle

### LICENÇA:
**MIT**
[Leia a licença](LICENSE)

### COMPILADOR UTILIZADO:
***gcc (Ubuntu 5.3.1-14ubuntu2.1) 5.3.1***

### OBJETIVO:
Criar um algoritmo que gere a legenda com base nos textos dos commits.

### NOTAS:
- Esse algoritmo pega a mensagem dos commits e gera um arquivo de legenda com os textos dos commits.
- Ele somente funciona em arquivos XML válidos. Vale ressaltar que só foram feitos testes com arquivos de LOGs providos do Subversion.
- O programa gera como saída um arquivo chamado *timestamp.txt*, portanto evite utilizar outro arquivo com o mesmo nome.
- O program procura pelas tags `<date>`,`</date>`,`<msg>` e `</msg>`.

### COMO USAR:
- `int main( int argc, char const *argv[] )`:
  - `argv[1]`: nome do arquivo de LOG em XML.

### EXEMPLO:
`$ gcc Escritor.c -o prog`

`$ ./prog log.xml`
