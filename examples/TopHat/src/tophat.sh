tophat ref/test_ref $1
samtools view -h -o result.sam tophat_out/accepted_hits.bam
