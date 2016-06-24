OPTIONS='-'

if [ $ORDER = 'decending' ]
then
    ORD='D'
else
	ORD='A'
fi

if [ $SORT != 'Size' || $SORT != 'Score']
then
	SORT='Size'
fi

if [ $CHROM = 'true' ]
then
	OPTIONS='-chrThen'
else
	SORT='size'
fi

OPTIONS=$OPTIONS$SORT$ORD
		
bedtools sort $OPTIONS -i $1 > result.txt

