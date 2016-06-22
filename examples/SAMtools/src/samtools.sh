if [ $COMMAND = rmdup -o $COMMAND = fixmate ]
then
    samtools $COMMAND $1 result.txt
elif [ $COMMAND = faidx ]
then
    samtools faidx $1
    FILENAME="$1.fai"
    cat $FILENAME > result.txt
else
    samtools $COMMAND $1 > result.txt
fi
