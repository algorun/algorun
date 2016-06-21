
if [ $COMMAND = "random" ]
then
    PARAMETER='-g'
else
    PARAMETER='-i'
fi

bedtools $COMMAND $PARAMETER $1 > result.txt
