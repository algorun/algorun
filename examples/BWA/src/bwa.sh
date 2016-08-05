
if [[ "$COMMAND" = "*2*" ]]
then
    bwa $COMMAND $1 result
else
    bwa $COMMAND $1 > result.txt
fi
