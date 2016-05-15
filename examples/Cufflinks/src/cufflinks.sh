INPUT_FILE=$1
OPTIONS='-p '$Threads' --seed '$Seed ' -u '$Rescue' -m '$FragLen_Mean' -s '$FragLen_Std
cufflinks $OPTIONS $INPUT_FILE
