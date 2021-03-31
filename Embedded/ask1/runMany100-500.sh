
mkdir -p results100-500

for i in `seq 1 5`; do
	"./prod-cons" 100 500 > "results100-500/run$i"
done

