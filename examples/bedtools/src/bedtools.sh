
if [ $COMMAND = "random" ]
then
    PARAMETER='-g'
else
    PARAMETER='-i'
fi

bedtools sort -i $1 > result.bed
#bedtools $COMMAND $PARAMETER $1
