#!/bin/sh
# Abre as janelas de quadros (bin/quadros_gui) num display virtual e salva um PNG de cada.
# Uso (na raiz do projeto, depois de make gui):
#   scripts/capturar_quadros.sh "<entrada do programa>" <prefixo>
#   ex.: scripts/capturar_quadros.sh "1\n1\n1e-4\n" a1
# Precisa de: xvfb, xdotool, imagemagick
set -e

ENTRADA="$1"
PREFIXO="$2"
mkdir -p graficos

Xvfb :99 -screen 0 1600x4000x24 >/dev/null 2>&1 &
XPID=$!
export DISPLAY=:99
export NO_AT_BRIDGE=1
sleep 1

printf "$ENTRADA" | ./bin/quadros_gui >/dev/null &
APID=$!
sleep 3

for janela in "QUADROS RESPOSTA:quadro_resposta" "QUADRO COMPARATIVO:quadro_comparativo"; do
    titulo=${janela%%:*}
    arquivo=${janela##*:}
    id=$(xdotool search --name "$titulo" | head -1)
    import -window "$id" "graficos/${arquivo}_${PREFIXO}.png"
    echo "gerado: graficos/${arquivo}_${PREFIXO}.png"
done

kill $APID $XPID
