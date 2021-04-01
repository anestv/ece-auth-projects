
mkdir -p results

for p in 5 10 15 20; do
	for i in 1 2 3 4 5 8 10; do
	
		q=$(( p * i ));
		
		"./prod-cons" $p $q | tail -n2 > "results/$p-$q"
	
	done
done

for f in `ls results`;do
	printf "$f\t" >> results/var.txt # using echo would add a newline
	tail -n1 results/$f | grep -Po "\d+\.\d" >>results/var.txt
	printf "$f\t" >> results/mean.txt
	head -n1 results/$f | grep -Po "\d+\.\d" >>results/mean.txt
done

