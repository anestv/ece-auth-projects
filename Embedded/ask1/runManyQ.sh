
mkdir -p results

p=100
#for q in 20 40 60 80 100 120 150 170 200 230 260 300 350 400 450 500 550 600 700 800; do
for q in 320 350 370 400 420 `seq 450 10 590` 620 650 750 900 1000; do

	"./prod-cons" $p $q | tail -n2 > "results/$p-$q"

done

for f in `ls results`;do
	echo "$f" >> results/var.txt
	tail -n1 results/$f | grep -Po "\d+\.\d" >>results/var.txt
	echo "$f" >> results/mean.txt
	head -n1 results/$f | grep -Po "\d+\.\d" >>results/mean.txt
done

