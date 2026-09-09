date
whoami
pwd

echo "=== build ==="
make clean
make

echo "=== c2: exemplo do enunciado (voo.txt) ==="
printf '100\nATT 20 12 8\nNAV 50 30 15\n' > voo.txt
./scheduler rate voo.txt && cat rate_lagi.out
./scheduler edf voo.txt && cat edf_lagi.out

echo "=== c1: caso folgado do checker do professor ==="
printf '60\nATT 15 15 5\nFCS 30 30 8\n' > c1_folgado.txt
./scheduler rate c1_folgado.txt && cat rate_lagi.out
./scheduler edf c1_folgado.txt && cat edf_lagi.out

echo "=== empate de prioridade ==="
printf '20\nX 10 10 5\nY 10 10 5\n' > tie.txt
./scheduler rate tie.txt && cat rate_lagi.out
./scheduler edf tie.txt && cat edf_lagi.out

echo "=== contador KILLED ==="
printf '5\nZ 100 50 10\n' > killed.txt
./scheduler rate killed.txt && cat rate_lagi.out

echo "=== overflow de int ==="
printf '100\nATT 99999999999 12 8\n' > overflow.txt
./scheduler rate overflow.txt
echo "codigo de saida: $?"

echo "=== linha em branco estilo Windows (\\r\\n) ==="
printf '100\r\nATT 20 12 8\r\nNAV 50 30 15\r\n\r\n' > crlf.txt
./scheduler rate crlf.txt
echo "codigo de saida: $?"

echo "=== stdout/stderr silenciosos ==="
./scheduler rate voo.txt > stdout_check.txt 2> stderr_check.txt
echo "stdout bytes: $(wc -c < stdout_check.txt)"
echo "stderr bytes: $(wc -c < stderr_check.txt)"

echo "=== limpando .out antes dos casos de erro, para a checagem final ser inequivoca ==="
rm -f rate_lagi.out edf_lagi.out

echo "=== casos de erro exigidos ==="
./scheduler rate
echo "codigo de saida: $?"

./scheduler foo voo.txt
echo "codigo de saida: $?"

./scheduler rate nao_existe.txt
echo "codigo de saida: $?"

printf '100\nATT 20 12\n' > malformado1.txt
./scheduler rate malformado1.txt
echo "codigo de saida: $?"

printf '100\nATT 20abc 12 8\n' > malformado2.txt
./scheduler rate malformado2.txt
echo "codigo de saida: $?"

printf '100\nATT 20 0 8\n' > malformado3.txt
./scheduler rate malformado3.txt
echo "codigo de saida: $?"

printf '100\nATT 10 15 8\n' > malformado4.txt
./scheduler rate malformado4.txt
echo "codigo de saida: $?"

printf '100\nATT 20 12 15\n' > malformado5.txt
./scheduler rate malformado5.txt
echo "codigo de saida: $?"

echo "=== nenhum .out criado pelos casos de erro acima ==="
ls *.out

echo "=== limpeza ==="
make clean
rm -f voo.txt c1_folgado.txt tie.txt killed.txt overflow.txt crlf.txt malformado*.txt stdout_check.txt stderr_check.txt
