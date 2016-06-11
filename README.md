## LEGENDA AUTOMÁTICA PARA GOURCE EM C

### LICENÇA:
**MIT**
[Leia a licença](LICENSE)

### COMPILADOR UTILIZADO:
***gcc (Ubuntu 5.3.1-14ubuntu2.1) 5.3.1***

### OBJETIVO:
Criar um algoritmo que gere automaticamente a legenda para um vídeo do Gource.

### NOTAS:
- Esse algoritmo transforma os textos dos commits em legendas.
- Ele somente funciona em arquivos XML válidos. Vale ressaltar que só foram feitos testes com arquivos de LOGs providos do Subversion.
- O programa gera como saída um arquivo chamado *timestamp.txt*, portanto evite utilizar outro arquivo com o mesmo nome.

### COMO USAR:
- `int main( int argc, char const *argv[] )`:
  - `argv[1]`: nome do arquivo de LOG em XML.

### EXEMPLO:
`$ gcc Escritor.c -o prog`

`$ ./prog log.xml`
