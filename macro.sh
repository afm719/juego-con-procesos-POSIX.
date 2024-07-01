#makefile realizado
if ! make 
then
    echo "error en el make";
    exit
fi

aux=0
puntos=0
for ((i=0; i<4; i++)) 
    do 
        aux= ./war
        if [ $aux -le 0 ]
        then
            puntos=$((aux+puntos))
        fi
    done

if [ $puntos -gt 0 ]; then
    echo Ha ganado el pais A
elif [ $puntos -lt 0 ]; then
    echo Ha ganado el pais B
else
    echo Empate
fi