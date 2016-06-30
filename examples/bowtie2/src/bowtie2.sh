OPTIONS="-s $Skip"
if [ "$OnlyAlign" != 'all' ]
then
	OPTIONS='$OPTIONS -u $OnlyAlign'
fi

OPTIONS="$OPTIONS -5 $TrimLeft"
OPTIONS="$OPTIONS -3 $TrimRight"

if [ $PhreadQuality == "64" ]
then
	OPTIONS="$OPTIONS --phread64-quals"
fi

if [ $AlignN = 1 ]
then
	OPTIONS='$OPTIONS -N 1'
fi

if [ $AlignL > 3 && $AlignL < 32 ]
then 
	OPTIONS='$OPTIONS -L $AlignL'
fi

OPTIONS= "$OPTIONS -I $AlignI"
OPTIONS= "$OPTIONS -X $AlignX"

if [ $Report = 'all' ]
then
	OPTIONS='$OPNTIONS --all'
else
	OPTIONS='$OPTIONS -k $Report'
fi

bowtie2-build example/reference/lambda_virus.fa lambda_virus
bowtie2 $OPTIONS -x lambda_virus -U $1 > result.txt
