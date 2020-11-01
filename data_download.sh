

wget --no-remove-listing ftp://ftp.ddbj.nig.ac.jp/ddbj_database/dra/fastq/DRA004/DRA004358/

# get list of ftp directory to be installed 
cat index.html|grep "ディレクトリ"|cut -d "\"" -f2|cut -d ">" -f1
wget -qO- --no-remove-listing ftp://ftp.ddbj.nig.ac.jp/ddbj_database/dra/fastq/DRA004/DRA004358/DRX048976/

# set file names
FileNames=`wget -qO- --no-remove-listing ftp://ftp.ddbj.nig.ac.jp/ddbj_database/dra/fastq/DRA004/DRA004358/DRX048976/|grep "ファイル"|cut -d "\"" -f2|cut -d ">" -f1`
for line in $FileNames;
do 
 aria2c -x5 --ftp-passwd= -c -l Log $line;
done

# download bz2 file
aria2c -x5 --ftp-passwd= -c -l Log ftp://ftp.ddbj.nig.ac.jp/ddbj_database/dra/fastq/DRA004/DRA004358/DRX048976/DRR054126_1.fastq.bz2


aria2c -x5 https://rapdb.dna.affrc.go.jp/download/archive/irgsp1/IRGSP-1.0_genome.fasta.gz
tar -zcvf IRGSP-1.0_genome.fasta.gz

# unzip and conver to gz file
line="ftp://ftp.ddbj.nig.ac.jp:21/ddbj_database/dra/fastq/DRA004/DRA004358/DRX048976/DRR054126_1.fastq.bz2"
fnamei=`echo $line|cut -d "/" -f10|cut -d "." -f1`

for line in $commands; do $line; done
for f in *.bz2; do bzcat "$f" | gzip -c >"${f%.*}.gz"; done
for f in *.bz2; do lbzip2 bzcat "$f" | pigz gzip -c >"${f%.*}.gz"; done


for f in *.bz2; do lbzcat "$f" | pigz -c >"${f%.*}.gz"; done

sudo apt-get install lbzip2 pigz
